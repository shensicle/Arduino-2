
#include <Arduino.h>
#include <EEPROM.h>
#include <TwoStateOutput.h>

#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

#include <avr/pgmspace.h>

#include <LCDButtons.h>
#include <ProfileBase.h>
#include <TimedProfile.h>
#include <CmdParser.h>
#include <VCC.h>
#include <AppBase.h>
#include <LCDApp.h>
#include <SerialInterface.h>
#include <LCDUnsignedItem.h>
#include <LCDProfileRepeatsItem.h>
#include <LCDProfileStartupDelayItem.h>
#include <LCDProfileShutterOpenIntervalItem.h>
#include <LCDProfileShutterClosedIntervalItem.h>
#include <LCDListItem.h>
#include <LCDLowPowerItem.h>
#include <LCDRunStopItem.h>
#include <LCDBacklightColourItem.h>
#include <LCDCurrentProfileItem.h>
#include <LCDTimeItem.h>
#include <LCDBatteryItem.h>

#include <LCDItemBase.h>
#include <LCDItemCollection.h>

#define RELAY_PIN 4

#define LOW_VCC_THRESHOLD_MV  2500

// Delay between each processing loop, in milliseconds
#define UPDATE_INTERVAL   50

// Serial communications
#define BAUD_RATE 115200

// The persistent configuration data for this application
#define CONFIG_OFFSET 0

// Number of static (ie. header) strings in LCD
#define LCD_TABLE_ITEMS 9

// And a relay
static TwoStateOutput Relay (RELAY_PIN, true);

// And an input voltage monitor
static Vcc VoltageMonitor (UPDATE_INTERVAL);

// Now create profile
static TimedProfile Profile1 (&Relay, 100, UPDATE_INTERVAL);

// Array of profiles - to make iteration easier. This is for ease of future
// expandability as currently we only support one profile
#define NUM_PROFILES 1

// Lists of profiles
static ProfileBase* ProfileList[NUM_PROFILES] = { &Profile1 };

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
shen_LCD TheLCD = shen_LCD();

// Create the object that manages this application
static LCDApplication TheApp (CONFIG_OFFSET, NUM_PROFILES, ProfileList, &TheLCD);

// Make a serial interface so user can communicate with us from a computer
static SerialInterface TheSerialInterface (&TheApp, NUM_PROFILES);

//----------------------------------------------------------------------------------------
// Create string table for LCD - these need to be stored in flash because we have limited RAM
PGM_P const string00[] PROGMEM = {"Run/Stop"};  
PGM_P const string01[] PROGMEM = {"Backlight Colour"};
PGM_P const string02[] PROGMEM = {"Current Profile"};  
PGM_P const string03[] PROGMEM = {"Repeats"};  
PGM_P const string04[] PROGMEM = {"Startup Delay"};  
PGM_P const string05[] PROGMEM = {"Shutter Open"};  
PGM_P const string06[] PROGMEM = {"Shutter Cls"};  
PGM_P const string07[] PROGMEM = {"Low Power Mode"};
PGM_P const string08[] PROGMEM = {"Battery Voltage"};


// Was this - PROGMEM char* const LCDStringTable[LCD_TABLE_ITEMS] = 	  
const char* LCDStringTable[] =     // 16 is number of characters in one row of LCD display @@@
{   
  (const char*)string00,
  (const char*)string01,
  (const char*)string02,
  (const char*)string03,
  (const char*)string04,
  (const char*)string05,
  (const char*)string06,
  (const char*)string07,
  (const char*)string08
};


// Create LCD items to control the application
LCDRunStopItem LCDRunStop (&TheLCD, 0, &TheApp);
LCDBacklightColourItem LCDBacklightColour (&TheLCD, 1, &TheApp);
LCDLowPowerItem LCDLowPower (&TheLCD, 7, &TheApp);
LCDBatteryItem LCDBattery (&TheLCD, 8, &VoltageMonitor);
//LCDCurrentProfileItem LCDCurrentProfile (&TheLCD, 2, &TheApp);


// Create LCD items to configure each of the programs
LCDProfileRepeatsItem Profile1Repeats (&TheLCD, 3, &Profile1);
LCDProfileStartupDelayItem Profile1StartupDelay (&TheLCD, 4, &Profile1);
LCDProfileShutterOpenIntervalItem Profile1ShutterOpenInterval (&TheLCD, 5, &Profile1);
LCDProfileShutterClosedIntervalItem Profile1ShutterClosedInterval (&TheLCD, 6, &Profile1);


// Create a top-level menu for the LCD
LCDItemBase *MenuList [] = { &LCDRunStop,
                             &Profile1Repeats, &Profile1StartupDelay, &Profile1ShutterOpenInterval, &Profile1ShutterClosedInterval, 
                             &LCDBacklightColour, &LCDLowPower, &LCDBattery}; 
LCDItemCollection TheMenu (&TheLCD, MenuList, 8, 0, false);

// ----------------------------------------------------------------------------------------------------------------------------------
static void ServiceSerialInterface (void)
{
  TheSerialInterface.Update();
}

// ----------------------------------------------------------------------------------------------------------------------------------
void ServiceInputs(void)
{
   static unsigned char oldButtons = 0x00;
  
    // Service buttons
    unsigned char buttons = TheLCD.readButtons();
    
    // Valid presses are those that are in buttons but not in old buttons
    unsigned char newPresses = buttons & ~oldButtons;
    oldButtons = buttons;
    
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


    // Next service the serial port. It is conceivable that buttons will be pressed at the same time as a serial command is issued.
    // If this happens, the buttons will be processed first followed by the serial commands.
    ServiceSerialInterface ();

    // Update the active item on the display, just in case the terminal interface changed something or the
    // current profile execution completed.
    TheMenu.Update();

    // Finally, check battery status
    long millivolts = VoltageMonitor.Get();

    if ((millivolts < LOW_VCC_THRESHOLD_MV) && (millivolts >= 0))
    {
      TheLCD.setBacklight(YELLOW);
    }
}

// ----------------------------------------------------------------------------------------------------------------------------------
void setup()
{  
  // Initialize serial communications
  Serial.begin(BAUD_RATE);  
  
  // set up the LCD's number of rows and columns: 
  TheLCD.begin(16, 2);
  
  char backlight = TheApp.GetBacklightColour();
  LCDBacklightColour.SetBacklightColour (backlight);
  
  TheLCD.clear();
  TheLCD.setCursor (0,0);
  TheLCD.print (F("  RemBrain-LCD  "));
  TheLCD.setCursor (0, 1);
  TheLCD.print (F(" Firmware V1.1  "));
  
  // Wait 2 seconds so users can see startup message
  delay (2000);
  
  TheMenu.OnEntry();  

}

// ----------------------------------------------------------------------------------------------------------------------------------
void loop()
{
   
  // To keep the timing consistent, we subtract the time each loop takes to execute from the
  // delay until the beginning of the next loop.
  unsigned long startTime = millis();

  // Tell the application to update its processing elements.
  TheApp.Update(); 
  
  // Update the Vcc monitor
  VoltageMonitor.Update (); 
 
  // Check all inputs to see if user is requesting we do something
  ServiceInputs(); 
  
  unsigned long endTime = millis();  
  
  // And wait for the next time
  if ((endTime - startTime) < UPDATE_INTERVAL)
      delay (UPDATE_INTERVAL - (endTime - startTime));
}

// ----------------------------------------------------------------------------------------------------------------------------------
//This function is automatically called between repetitions of loop()
void serialEvent() 
{

  while (Serial.available()) 
  {
    // Get the new character
    char inChar = (char)Serial.read(); 

    // And add it to the command processor
    TheSerialInterface.Add(inChar);  
  }
}

