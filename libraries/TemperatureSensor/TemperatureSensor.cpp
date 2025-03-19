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

#include "TemperatureSensor.h"


// -----------------------------------------------------------------------------
TemperatureSensorClass::TemperatureSensorClass ( const unsigned char sensorPin,
    	             				 unsigned long serviceInterval, 
    	             				 unsigned long updateInterval,
    	             				 IFTTTMessageClass*  theIFTTTSender,
    	             				 SensorConfigClass*  theConfiguration) 
	: PeriodicService (serviceInterval, updateInterval), 
	  TheSensor (sensorPin, DHT11)
{
  SensorPin = sensorPin;   // Don't need this with DHT11 driverf'
  TheConfiguration = theConfiguration;

  IFTTTSender = theIFTTTSender;
  
  LastValue = TEMPERATURE_INVALID;
  
  TheSensor.begin();
}

// -----------------------------------------------------------------------------
void TemperatureSensorClass::Update (void)
{
   static bool outOfRangeDetectedPreviously = false;
   static bool outOfRangeDetected = false;
    
    static char msgString[40];   // keep it off the stack
      
    // Read temperature sensor
    float temp_f = TheSensor.readTemperature();

       
    // Sensor read successfully
    unsigned long temp = (unsigned long)temp_f;
    LastValue = temp;
    
	Serial.print ("Temperature Sensor Reading (deg C): "); Serial.println (temp);
	
    // Sensor driver returns float but everything else is done in int
	if (temp > TheConfiguration->GetTemperatureHighThreshold() || 
		temp < TheConfiguration->GetTemperatureLowThreshold())
	    outOfRangeDetected = true;
	else
	    outOfRangeDetected = false;


    // Now run through the FSM. We don't need to do anything if the state hasn't changed since last time
    if (outOfRangeDetected != outOfRangeDetectedPreviously)
    {
    	if (outOfRangeDetected == false)
    	    sprintf (msgString, "Temperature in range (%d deg C)", temp);
    	else
     	    sprintf (msgString, "Temperature out of range (%d deg C)", temp);
   	
    	IFTTTSender->Send (msgString);
    	outOfRangeDetectedPreviously = outOfRangeDetected;
    }

}
