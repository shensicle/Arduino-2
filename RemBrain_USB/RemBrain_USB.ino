/*
Copyright © 2011 Scott Henwood/shensicle photographic. All Rights Reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

3. The name of the author may not be used to endorse or promote products 
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Scott Henwood/shensicle photographic "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <EEPROM.h>
#include <Button.h>
#include <TwoStateOutput.h>
#include <SimpleLED.h>
#include <ProfileBase.h>
#include <TimedProfile.h>
#include <CmdParser.h>
#include <VCC.h>
#include <AppBase.h>
#include <LEDApp.h>
#include <SerialInterface.h>

#define RELAY_PIN 4

#define RUNSTOP_PIN 5
#define PROFILE_PIN 6

#define RUNSTOP_LED 12  
#define PROFILE1_LED 11
#define PROFILE2_LED 10
#define PROFILE3_LED 9

#define LOW_VCC_THRESHOLD_MV  2500

#define DEBOUNCE_INTERVAL 100
#define UPDATE_INTERVAL   50

// Serial communications
#define BAUD_RATE 115200

// The persistent configuration data for this application
#define CONFIG_OFFSET 0

// Create the push buttons we need
static Button RunStopButton (RUNSTOP_PIN, DEBOUNCE_INTERVAL, UPDATE_INTERVAL);
static Button ProfileButton (PROFILE_PIN, DEBOUNCE_INTERVAL, UPDATE_INTERVAL);

// Also create the LEDs we need
static SimpleLED RunStopLED (RUNSTOP_LED, true, UPDATE_INTERVAL);
static SimpleLED Profile1LED    (PROFILE1_LED, true, UPDATE_INTERVAL);
static SimpleLED Profile2LED    (PROFILE2_LED, true, UPDATE_INTERVAL);
static SimpleLED Profile3LED    (PROFILE3_LED, true, UPDATE_INTERVAL);

// And a relay
static TwoStateOutput Relay (RELAY_PIN, true);

// And an input voltage monitor
static Vcc VoltageMonitor (UPDATE_INTERVAL);

// Now create 3 profiles
static TimedProfile Profile1 (&Relay, 100, UPDATE_INTERVAL);
static TimedProfile Profile2 (&Relay, 200, UPDATE_INTERVAL);
static TimedProfile Profile3 (&Relay, 300, UPDATE_INTERVAL);

// Array of profiles - to make iteration easier
#define NUM_PROGRAMS 3

// Lists of profiles, buttons and LEDs
static ProfileBase* ProfileList[NUM_PROGRAMS] = { &Profile1, &Profile2, &Profile3 };
static SimpleLED* LEDList[NUM_PROGRAMS] = { &Profile1LED, &Profile2LED, &Profile3LED };


// Create the object that manages this application
static LEDApplication TheApp (CONFIG_OFFSET, NUM_PROGRAMS, ProfileList, LEDList, &RunStopLED);

// Make a serial interface so user can communicate with us from a computer
static SerialInterface TheSerialInterface ((ApplicationBase*)&TheApp, NUM_PROGRAMS);

// ----------------------------------------------------------------------------------------------------------------------------------
static void ServiceButtons (void)
{

  // Update the buttons
  ProfileButton.Update();
  RunStopButton.Update();

  // If the profile button was pressed, deal with it by moving to the next profile
  if (ProfileButton.WasPressed())
  {
    // Move to the next profile
    unsigned short curProfile = TheApp.GetCurrentProfile();
    if (++curProfile >= NUM_PROGRAMS)
    {
      curProfile = 0;
    }
    TheApp.SetCurrentProfile (curProfile);
  } 

  // Now deal with the run/stop button
  if (RunStopButton.WasPressed() == true)
  {
    // If we are running now, stop
    if (TheApp.ProfileIsRunning())
        TheApp.Stop();
    else
        TheApp.Run();
  }
}

// ----------------------------------------------------------------------------------------------------------------------------------
static void ServiceSerialInterface (void)
{
  TheSerialInterface.Update();
}

// ----------------------------------------------------------------------------------------------------------------------------------
void ServiceInputs(void)
{

  ServiceButtons ();  

  // Next service the serial port. It is conceivable that buttons will be pressed at the same time as a serial command is issued.
  // If this happens, the buttons will be processed first followed by the serial commands.
  ServiceSerialInterface ();

  // Finally, check battery status
  long millivolts = VoltageMonitor.Get();

  if ((millivolts < LOW_VCC_THRESHOLD_MV) && (millivolts >= 0))
  {

    RunStopLED.SetFlashMode (true); 
  }

}

// ----------------------------------------------------------------------------------------------------------------------------------
void setup()
{  
  // Initialize serial communications
  Serial.begin(BAUD_RATE);
}

// ----------------------------------------------------------------------------------------------------------------------------------
void loop()
{
   
 TheApp.Update(); 
  
 // Update the Vcc monitor
 VoltageMonitor.Update (); 
 
 // Check all inputs to see if user is requesting we do something
 ServiceInputs();
 
 // And wait for the next time
 delay (UPDATE_INTERVAL);
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

