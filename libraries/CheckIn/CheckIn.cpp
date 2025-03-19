/*
Copyright 2023 Scott Henwood/shensicle photographic. All Rights Reserved.

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


#include <Arduino.h>

#include "CheckIn.h"


// -----------------------------------------------------------------------------
CheckInClass::CheckInClass ( unsigned long serviceInterval, 
    	             			 unsigned long updateInterval,
    	             		     IFTTTMessageClass*  theIFTTTSender,
    	             		     SensorConfigClass*  theConfiguration,
    	             		     TemperatureSensorClass* theTemperatureSensor,
    	                         WaterSensorClass*       theWaterSensor) 
	: PeriodicService (serviceInterval, updateInterval)
{
  TheConfiguration = theConfiguration;

  IFTTTSender = theIFTTTSender;
  
  TheTemperatureSensor = theTemperatureSensor;
  TheWaterSensor       = theWaterSensor;
  
  Serial.print ("CHECKIN - Update interval is "); Serial.println(updateInterval);
}

// -----------------------------------------------------------------------------
void CheckInClass::Update (void)
{   
    static char msgString[60];   // keep it off the stack
        
    strcpy (msgString, "Check in - ");
    
    if (TheConfiguration->HasTemperatureSensor())
    {
    	strcat (msgString, "Temp: "); 
    	if (TheTemperatureSensor->GetLastReading() == TEMPERATURE_INVALID)
    		strcat (msgString, "None ");
    	else
    		sprintf (&msgString[strlen(msgString)], "%lu ", TheTemperatureSensor->GetLastReading());
    }
      
    if (TheConfiguration->HasWaterSensor())
    {
    	strcat (msgString, "Water: ");
    	
    	if (TheWaterSensor->WaterDetected())
    		strcat (msgString, "Yes");
    	else
    		strcat (msgString, "No");
    	
    }	
    
    IFTTTSender->Send (msgString);
}
