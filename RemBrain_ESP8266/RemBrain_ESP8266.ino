#include <Arduino.h>
//#include <arduino.h>
#include <EEPROM.h>
#include <SH_EEPROM.h>
#include <TwoStateOutput.h>
#include <AnalogKeypad.h>

#include <Wire.h>

// Includ wifi, just so we can disable the hardware to save power
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

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

#include <LCDItemBase.h>
#include <LCDItemCollection.h>
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
// shen_LCD TheLCD = shen_LCD(TFT_CS, TFT_DC, TFT_RST);

// Create the object that manages this application
//static 
LCDApplication TheApp(CONFIG_OFFSET, NUM_PROFILES, ProfileList, TFT_CS, TFT_DC, TFT_RST);

// Make a serial interface so user can communicate with us from a computer
//static 
SerialInterface TheSerialInterface(&TheApp, NUM_PROFILES);

// Interface to our buttons
AnalogKeypad TheKeypad;

// Create LCD items to control the application
LCDRunStopItem LCDRunStop("Run/Stop", &TheApp);

LCDBackgroundColourItem LCDBackgroundColour("Screen Colr", &TheApp);
LCDForegroundColourItem LCDForegroundColour("Text Colr", &TheApp);

// Create LCD items to configure each of the programs
LCDProfileRepeatsItem Profile1Repeats("Repeats", &Profile1, &TheApp);
LCDProfileStartupDelayItem Profile1StartupDelay("Start Delay", &Profile1, &TheApp);
LCDProfileShutterOpenIntervalItem Profile1ShutterOpenInterval("Shutter Open", &Profile1, &TheApp);
LCDProfileShutterClosedIntervalItem Profile1ShutterClosedInterval("Shutter Cls", &Profile1, &TheApp);


// Create a top-level menu for the LCD
LCDItemBase* MenuList[] = { &LCDRunStop,
                            &Profile1Repeats, &Profile1StartupDelay, &Profile1ShutterOpenInterval, &Profile1ShutterClosedInterval,
                            &LCDBackgroundColour, &LCDForegroundColour };
LCDItemCollection TheMenu(&TheApp, MenuList, 7, "\0", false); // @@@ 3rd param is number of menu items - could be more elegant @@@

// ----------------------------------------------------------------------------------------------------------------------------------
static void ServiceSerialInterface(void) 
{
  TheSerialInterface.Update();
}

// ----------------------------------------------------------------------------------------------------------------------------------
void ServiceInputs(void) 
{
  static unsigned char oldButtons = 0x00;
  
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
      TheApp.ToggleRunStopState();
    }
    else
    {
      Serial.println ("Unknown key !!");
    }

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

  TheApp.InitializeDisplay ();

  TheApp.SetTitleLine(" RemBrain II");
  TheApp.SetStatusLine ("   Firmware v 2.0");

  TheMenu.OnEntry();
}

// ----------------------------------------------------------------------------------------------------------------------------------
void loop() {

  // To keep the timing consistent, we subtract the time each loop takes to execute from the
  // delay until the beginning of the next loop.
  unsigned long startTime = millis();

  // Tell the application to update its processing elements.
  TheApp.Update();

  // Check all inputs to see if user is requesting we do something
  ServiceInputs();

  unsigned long endTime = millis();

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
