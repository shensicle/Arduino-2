#include <Arduino.h>
#include <arduino.h>
#include <EEPROM.h>
#include <SH_EEPROM.h>
#include <TwoStateOutput.h>
#include <AnalogKeypad.h>

#include <Wire.h>

// Includ wifi, just so we can disable the hardware to save power
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

// For graphical LCD display
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
//#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>

#include <ProfileBase.h>
#include <TimedProfile.h>
#include <CmdParser.h>

#include <AppBase.h>
#include <LCDApp.h>
#include <SerialInterface.h>
#include <LCDUnsignedItem.h>
#include <LCDProfileRepeatsItem.h>
#include <LCDProfileStartupDelayItem.h>
#include <LCDProfileShutterOpenIntervalItem.h>
#include <LCDProfileShutterClosedIntervalItem.h>
#include <LCDListItem.h>

#include <LCDRunStopItem.h>
#include <LCDBackgroundColourItem.h>
#include <LCDForegroundColourItem.h>
#include <LCDCurrentProfileItem.h>
#include <LCDTimeItem.h>
//#include <LCDBatteryItem.h> // not supported in ESP8266

#include <LCDItemBase.h>
#include <LCDItemCollection.h>
#include <shen_LCD.h>
#include <LCDBackgroundColourItem.h>
#include <LCDForegroundColourItem.h>

#define RELAY_PIN 16        //  D0
#define SIDE_BUTTON_PIN 12  // D6

// Pins for LCD graphics display
#define TFT_CS  15
#define TFT_RST  4
#define TFT_DC   5

// Delay between each processing loop, in milliseconds
#define UPDATE_INTERVAL 50

// Serial communications
#define BAUD_RATE 115200

// The persistent configuration data for this application
#define CONFIG_OFFSET 0

// Number of static (ie. header) strings in LCD
#define LCD_TABLE_ITEMS 9

// Analog input readings for each of the 5 buttons, determined experimentally
#define BUTTON_RIGHT_MAX     50
#define BUTTON_DOWN_MAX     570
#define BUTTON_LEFT_MAX     750
#define BUTTON_UP_MAX       840
#define BUTTON_SELECT_MAX   900

// Button identifiers
#define BUTTON_RIGHT   0x01
#define BUTTON_DOWN    0x02
#define BUTTON_LEFT    0x04
#define BUTTON_UP      0x08
#define BUTTON_SELECT  0x0A



// Make our own eeprom object, which really just sets the size of the system eeprom emulation
SH_EEPROM EEPROMInitializer;

// And a relay
//static 
TwoStateOutput Relay(RELAY_PIN, true);

// Now create profile
//static 
TimedProfile Profile1(&Relay, 100, UPDATE_INTERVAL);  // the 100 is offset in eeprom!

// Array of profiles - to make iteration easier. This is for ease of future
// expandability as currently we only support one profile
#define NUM_PROFILES 1

// Lists of profiles
//static 
ProfileBase* ProfileList[NUM_PROFILES] = { &Profile1 };

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
shen_LCD TheLCD = shen_LCD(TFT_CS, TFT_DC, TFT_RST);

// Create the object that manages this application
//static 
LCDApplication TheApp(CONFIG_OFFSET, NUM_PROFILES, ProfileList, &TheLCD);

// Make a serial interface so user can communicate with us from a computer
//static 
SerialInterface TheSerialInterface(&TheApp, NUM_PROFILES);

// Interface to our buttons
AnalogKeypad TheKeypad;

/*
const char string00[] = { "Run/Stop" };
const char string01[] = { "Screen Colr" };
const char string02[] = { "Text Colr" };
const char string03[] = { "Repeats" };
const char string04[] = { "Start Delay" };
const char string05[] = { "Shutter Open" };
const char string06[] = { "Shutter Cls" };
const char string07[] = { "Low Power Mode" };
const char string08[] = { "Battery Voltage" };
*/

// Was this - PROGMEM char* const LCDStringTable[LCD_TABLE_ITEMS] =
/*
 @@@ This shouldn't be required. Left over from when these strings were stored in flash - need to clean up LCDItemBase.cpp and hpp   */
/*char* LCDStringTable[] =  // 16 is number of characters in one row of LCD display @@@ 
  {
    (char*)string00,
    (char*)string01,
    (char*)string02,
    (char*)string03,
    (char*)string04,
    (char*)string05,
    (char*)string06,
    (char*)string07,
    (char*)string08
  }; 
*/

// Create LCD items to control the application
LCDRunStopItem LCDRunStop(&TheLCD, "Run/Stop", &TheApp);

LCDBackgroundColourItem LCDBackgroundColour(&TheLCD, "Screen Colr", &TheApp);
LCDForegroundColourItem LCDForegroundColour(&TheLCD, "Text Colr", &TheApp);

// Create LCD items to configure each of the programs
LCDProfileRepeatsItem Profile1Repeats(&TheLCD, "Repeats", &Profile1, &TheApp);
LCDProfileStartupDelayItem Profile1StartupDelay(&TheLCD, "Start Delay", &Profile1, &TheApp);
LCDProfileShutterOpenIntervalItem Profile1ShutterOpenInterval(&TheLCD, "Shutter Open", &Profile1, &TheApp);
LCDProfileShutterClosedIntervalItem Profile1ShutterClosedInterval(&TheLCD, "Shutter Cls", &Profile1, &TheApp);


// Create a top-level menu for the LCD
LCDItemBase* MenuList[] = { &LCDRunStop,
                            &Profile1Repeats, &Profile1StartupDelay, &Profile1ShutterOpenInterval, &Profile1ShutterClosedInterval,
                            &LCDBackgroundColour, &LCDForegroundColour };
LCDItemCollection TheMenu(&TheLCD, &TheApp, MenuList, 7, "\0", false); // @@@ 3rd param is number of menu items - could be more elegant @@@

// ----------------------------------------------------------------------------------------------------------------------------------
static void ServiceSerialInterface(void) 
{
  TheSerialInterface.Update();
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Read the buttons and return a value representing the button pressed, if any
unsigned char ReadButtons(void)
{
  unsigned char returnValue = 0;

  unsigned short analogValue = analogRead(A0);


  if (analogValue < BUTTON_RIGHT_MAX)
  {
      Serial.printf ("Right %d\n", analogValue);
      returnValue = BUTTON_RIGHT;
  }
  else if (analogValue < BUTTON_DOWN_MAX)
  {
    Serial.printf ("Down %d\n", analogValue);
     returnValue = BUTTON_DOWN;
  }
  else if (analogValue < BUTTON_LEFT_MAX)
  {
    Serial.printf ("Left %d\n", analogValue);
    returnValue = BUTTON_LEFT;
  }
  else if (analogValue < BUTTON_UP_MAX)
  {
     Serial.printf ("Up %d\n", analogValue);
     returnValue = BUTTON_UP;
  }
  else if (analogValue < BUTTON_SELECT_MAX)
  {
      Serial.printf ("Select %d\n", analogValue);
      returnValue = BUTTON_SELECT;
  }
  else
  {
    if (analogValue != 1024) Serial.printf("Null %d\n", analogValue);
  }
    
  return (returnValue);
}

// ----------------------------------------------------------------------------------------------------------------------------------
void ServiceInputs(void) 
{
  static unsigned char oldButtons = 0x00;
  
  /*
    // Service buttons
    unsigned char buttons = ReadButtons();
    
    // Valid presses are those that are in buttons but not in old buttons
    unsigned char newPresses = buttons & ~oldButtons;
    oldButtons = buttons;
    */

    char keyPress = TheKeypad.GetKey();

    // Do this first as the most common situation is no key press
    if (keyPress == KeypadBase::KEYPAD_NOKEY)
    {
    }
    else if (keyPress == KeypadBase::KEYPAD_UP)
    {
      TheMenu.OnUp();
    }
    else if (keyPress == KeypadBase::KEYPAD_DOWN)
    {
      TheMenu.OnDown();
    }
    else if (keyPress == KeypadBase::KEYPAD_LEFT)
    {
      TheMenu.OnLeft();
    }
    else if (keyPress == KeypadBase::KEYPAD_RIGHT)
    {
      TheMenu.OnRight();
    }
    else if (keyPress == KeypadBase::KEYPAD_SELECT)
    {
      TheMenu.OnEnter();
    }
    else if (keyPress == KeypadBase::KEYPAD_OTHER)
    {
      // No function defined for this key yet
    }
    else
    {
      Serial.println ("Unknown key !!");
    }
    
    /*
    if (newPresses & BUTTON_UP) 
    {
      TheMenu.OnUp();
    }
    else if (newPresses & BUTTON_DOWN) 
    {
      TheMenu.OnDown();
    }
    else if (newPresses & BUTTON_LEFT) 
    {
      TheMenu.OnLeft();
    }
    else if (newPresses & BUTTON_RIGHT) 
    {
      TheMenu.OnRight();
    }
    else if (newPresses & BUTTON_SELECT) 
    {
      TheMenu.OnEnter();
    }
*/

   // Next service the serial port. It is conceivable that buttons will be pressed at the same time as a serial command is issued.
   // If this happens, the buttons will be processed first followed by the serial commands.
   ServiceSerialInterface();

   // Update the active item on the display, just in case the terminal interface changed something or the
   // current profile execution completed.
   TheMenu.Update();
}

// ----------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  // Initialize serial communications
  Serial.begin(BAUD_RATE);

  // Turn off the Wifi to save power
  #ifdef ESP8266
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  delay( 1 );  // CPU give up to allow the above to take effect
  #endif

  // Programmaable button on side of case
  //pinMode(SIDE_BUTTON_PIN, INPUT);

  TheLCD.initR(INITR_BLACKTAB);
  TheLCD.fillScreen(TheApp.GetBackgroundColourHW());
  TheLCD.setRotation(1);  // Landscape - change 3 to 1 if display is upside down
  TheLCD.setTextWrap(false);
  TheLCD.setTextColor(TheApp.GetForegroundColourHW());
  TheLCD.setTextSize(LCD_DEFAULT_TEXT_SIZE);

  //TheLCD.clear(); // Replace with  TheLCD.fillScreen(ST77XX_BLACK);
  TheLCD.setCursor(LCD_TEXT_COLUMN, LCD_TEXT_ROW_1);
  TheLCD.print(F("RemBrain II"));
  TheLCD.setCursor(LCD_TEXT_COLUMN, LCD_TEXT_ROW_2);

  TheLCD.setTextSize(1);
  TheLCD.print(F("       Firmware V1.0"));

  TheLCD.setTextSize(LCD_DEFAULT_TEXT_SIZE);

  TheMenu.OnEntry();
}

// ----------------------------------------------------------------------------------------------------------------------------------
void loop() {

  // To keep the timing consistent, we subtract the time each loop takes to execute from the
  // delay until the beginning of the next loop.
  unsigned long startTime = millis();

  // Tell the application to update its processing elements.
  TheApp.Update();

  // Update the Vcc monitor
  // VoltageMonitor.Update ();

  // Check all inputs to see if user is requesting we do something
  ServiceInputs();

  // Analog read test only
  //Serial.println(analogRead(A0));

  unsigned long endTime = millis();

/*
  // Something in the startup code after Setup() is setting SIDE_BUTTON_PIN to be an output, so change it here
  static int firstpass = 1;
  if (firstpass)
  {
    pinMode(SIDE_BUTTON_PIN, INPUT);
    firstpass = 0;
  }
  */

  //int button = digitalRead(SIDE_BUTTON_PIN);
  //if (button) Serial.printf ("One %d\n", button); else Serial.printf ("Zero %d\n", button);

  // And wait for the next time
  if ((endTime - startTime) < UPDATE_INTERVAL)
    delay(UPDATE_INTERVAL - (endTime - startTime));
}

// ----------------------------------------------------------------------------------------------------------------------------------
//This function is automatically called between repetitions of loop()
void serialEvent() {

  while (Serial.available()) {
    // Get the new character
    char inChar = (char)Serial.read();

    // And add it to the command processor
    TheSerialInterface.Add(inChar);
  }
}
