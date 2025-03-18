/*
Copyright 2025 Scott Henwood All Rights Reserved.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products 
   derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY CANARIE Inc. "AS IS" AND 
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

#include <LidarConfig.h>
#include <Arduino.h>

// ----------------------------------------------------------------------
// Return the one's complement checksum of the configuration structure. This
// checksum is stored in non-volatile storage along with the configuration itself.
unsigned char LidarConfigClass::CalculateChecksum (void)
{
	unsigned char* configurationBytes = (unsigned char*)&TheConfiguration;
	
	unsigned char returnValue = 0;
	
	for (int i = 0; i < sizeof(TheConfiguration)-1; i++)
	{
		returnValue += *configurationBytes++;
	}
	
	return (0xff - returnValue);
}


// ----------------------------------------------------------------------
// Constructor
LidarConfigClass::LidarConfigClass ()
{
	// Clear the configuration structure
	memset (&TheConfiguration, 0, sizeof (TheConfiguration));
}
		

// ----------------------------------------------------------------------
// Write configuration information to non-volatile storage, adding a checksum
void LidarConfigClass::Write (void)
{
    unsigned writeAddr = 0;
	
    TheConfiguration.checksum = CalculateChecksum ();
	
    // Write the data
    TheStorage.putBytes("LidarConfig", (byte*)(&TheConfiguration), sizeof(TheConfiguration));
}
		

// ----------------------------------------------------------------------
// Read configuration information from non-volatile storage and validate the checksum
// Returns true if configuration is valid and false otherwise
bool LidarConfigClass::Read(void)
{
    bool returnValue = true;
    unsigned readAddr = 0;

    // Zero out configuration structure. Helps to null-terminate strings
    memset (&TheConfiguration, 0, sizeof (TheConfiguration));
	
    // Read the data
    TheStorage.getBytes("LidarConfig", &TheConfiguration, sizeof(TheConfiguration));
	
    // Calculate the checksum of this data
    unsigned char checksum  = CalculateChecksum ();

	// If non-volatile storage checksum is wrong ...
    if (checksum != TheConfiguration.checksum)
    {
    	Serial.printf ("Read: Checksum failure\n");
        returnValue = false;
	}
	
    return (returnValue);
}

// ----------------------------------------------------------------------
// Load the configuration from EEPROM. This must be called after the object is
// created but before any of the other methods can be used. f the eeprom checksum
// test fails, it is re-initialized to defaults
void LidarConfigClass::Load (void)
{
	// Initialize the non-volatile storage driver
	TheStorage.begin ("LidarConfig");

    // Read our configuration
    bool returnValue = Read();
    
    // Configuration is bad so re-initiailze to defaults
    if (returnValue == false)
    {
    	TheConfiguration.ObjectDetectionThreshold = DEFAULT_OBJECT_DETECT_THRESHOLD;
    	TheConfiguration.PollingIntervalMSecs     = DEFAULT_POLLING_INTERVAL_MSECS;
    	TheConfiguration.BeepDurationMSecs        = DEFAULT_BEEP_DURATION_MSECS;
    	TheConfiguration.BackoffThreshold         = DEFAULT_BACKOFF_THRESHOLD;

    	// And save
    	Write();
    }

}

// ------------------------------------------------------------------------------
// Change the object detect threshold.
bool LidarConfigClass::SetObjectDetectionThreshold(unsigned short newThreshold)
{
	bool returnValue = false;
		
	// New value must be in range, not the same as the current value, and greater
	// than the current backoff.
	if ((newThreshold >= MIN_OBJECT_DETECT_THRESHOLD) 
		&& (newThreshold <= MAX_OBJECT_DETECT_THRESHOLD) 
		&& (newThreshold != TheConfiguration.ObjectDetectionThreshold)
		&& (newThreshold >  TheConfiguration.BackoffThreshold))
	{
		TheConfiguration.ObjectDetectionThreshold = newThreshold;
		Write();
		returnValue = true;
	}
	return (returnValue);
}
 
// ------------------------------------------------------------------------------
// Change the polling interval, specified in milliseconds
bool LidarConfigClass::SetPollingInterval(unsigned short newInterval)
{
	bool returnValue = false;
		
	// New value must be within range and different than curret value
	if ((newInterval >= MIN_POLLING_INTERVAL_MSECS) 
		&& (newInterval <= MAX_POLLING_INTERVAL_MSECS) 
		&& (newInterval != TheConfiguration.PollingIntervalMSecs))
	{
		TheConfiguration.PollingIntervalMSecs = newInterval;
		Write();
		returnValue = true;
	}
	return (returnValue);
}

// ------------------------------------------------------------------------------
// Determine how long a beep lasts for
bool LidarConfigClass::SetBeepDuration (unsigned short newDuration)
{
	bool returnValue = false;
	
	// New value must be within range and different than curret value
	if ((newDuration >= MIN_BEEP_DURATION_MSECS) 
		&& (newDuration <= MAX_BEEP_DURATION_MSECS) 
		&& (newDuration != TheConfiguration.BeepDurationMSecs))
	{
		TheConfiguration.BeepDurationMSecs = newDuration;
		Write();
		returnValue = true;
	}
	return (returnValue);
}

// ------------------------------------------------------------------------------
// Set the backoff threshold. If an object is in range, but it's distance increases by
// at least this value, assume a second ojbect and alert.
bool LidarConfigClass::SetBackoffThreshold (unsigned short newBackoff)
{
	bool returnValue = false;
	// New value must be in range, different than current value and greater than the
	// current detection threshold
	if ((newBackoff >= MIN_OBJECT_DETECT_THRESHOLD) 
		&& (newBackoff <= MAX_OBJECT_DETECT_THRESHOLD) 
		&& (newBackoff != TheConfiguration.BackoffThreshold)
		&& (newBackoff <  TheConfiguration.ObjectDetectionThreshold))
		{
			TheConfiguration.BackoffThreshold = newBackoff;
			Write();
			returnValue = true;
		}
		return (returnValue);
}		
