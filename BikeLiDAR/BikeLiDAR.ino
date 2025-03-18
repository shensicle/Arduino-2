/*
Copyright  2025 Scott Henwood/shensicle photographic. All Rights Reserved.

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

#include "arduino.h"
#include "LiDAR.h"
#include "LidarConfig.h"
#include "LidarTerminal.h"

// Serial communications
#define BAUD_RATE 115200

#define FIRMWARE_VERSION "V1.1"

// Lidar object
Lidar TheLidar;

// Configuration of this unit - saved in non-volatile storage
LidarConfigClass TheConfig;

// Object that manages the terminal interface
LidarTerminal TheTerminalInterface(&TheConfig, FIRMWARE_VERSION);


// ------------------------------------------------------------------------
void setup() 
{
   // Initialize serial communications
  Serial.begin(BAUD_RATE);

  // Output pin for buzzer
  digitalWrite (23, LOW);
  pinMode (23, OUTPUT);

  // Load the configuration from non-volatile memory and reset in the case of a checksum failure
  TheConfig.Load();

    // This is how we tell the user we've started
    SoundBeeper (); 
    SoundBeeper (); 
    SoundBeeper ();

    // Create a task to poll the lidar hardware
    xTaskCreate(
        LidarPoll,      // Function name of the task
        "Lidar Poll",   // Name of the task (e.g. for debugging)
        2048,           // Stack size (bytes)
        NULL,           // Parameter to pass
        1,              // Task priority
        NULL            // Task handle
    );
    
    // Create a task to monitor the range values returned by the lidar hardware and
    // take appropriate action
    xTaskCreate(
        RangeCheck,     // Function name of the task
        "Range Ck",     // Name of the task (e.g. for debugging)
        2048,           // Stack size (bytes)
        NULL,           // Parameter to pass
        1,              // Task priority
        NULL            // Task handle
    );


}

// ------------------------------------------------------------------------
// Mainline for the task that periodically polls the lidar hardware
void LidarPoll (void *parameter) 
{
    while (1)
    {
 //     Serial.println ("Lidar Poll");
      TheLidar.UpdateDistance();
      delay (TheConfig.GetPollingInterval());  
    }    

}

// ------------------------------------------------------------------------
// Mainline for the task that manages the serial terminal interface
void loop()
{

  // Process any characters received on terminal interface
  // Check the serial interface for a complete command and, if there is one, execute it
  SerialEvent();

  // Check to see if a complete command has been received, and if so, execute it
  TheTerminalInterface.Update();

  delay (100);   // Check for characters every 100 msecs
}

// -------------------------------------------------------
// This function is called during each repetition of loop()
// and processes characters received from the serial terminal interface
void SerialEvent() 
{
  while (Serial.available()) 
  {
    // Get the new character
    char inChar = (char)Serial.read(); 

    // And add it to the command processor
    TheTerminalInterface.Add(inChar);  
  }
}

// --------------------------------------------------------------------
// Activate the beeper for a period of time specified in configuration. You might be
// thinking here that this should be run in its own thread to avoid holding up 
// future detections. But, the purpose of this device is to beep when an object is 
// detected and if this is called we have one, so can afford to delay a bit before
// reading from the lidar again.
void SoundBeeper (void)
{
    digitalWrite (23, HIGH);
    delay (TheConfig.GetBeepDuration());
    digitalWrite (23, LOW);
    delay (TheConfig.GetBeepDuration());  // This stops collisions if another beep comes right after this one
}


// -------------------------------------------------------
// This method, which should be run in its own thread, monitors
// the current distance and triggers the buzzer when required.
void RangeCheck (void *parameter)
{
  bool objectInRange = false;
  unsigned short lastDistance, currDistance = 0;

  while (1)
  {
    // Get the last distance read from the lidar hardware
    TheLidar.GetDistance (currDistance);

    // A currDistance of 0 indicates a lidar detection error, so ignore
    // Otherwise, if there is an object within range ... 
    if ((currDistance <= TheConfig.GetObjectDetectionThreshold()) && (currDistance != 0))
    {
      // Object detected - If we have not beeped before, beep now
      if (objectInRange == false)
      {
          objectInRange = true;
          SoundBeeper();
      }
      else
      {
          // Beeper has already been triggered
          
          // If an object is still in range, but the distance has increased, could
          // be that one car passed us but there is another right behind - beep again
          // to be on the safe side.
          if (currDistance > (lastDistance + TheConfig.GetBackoffThreshold()))
          {
             SoundBeeper();
          }
      }

    }
    else
    {
      // Nothing in range
      objectInRange = false;
    }

    lastDistance = currDistance;
    delay (TheConfig.GetPollingInterval());

  }
}
