/*
Copyright � 2011 Scott Henwood/shensicle photographic. All Rights Reserved.

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

#include <TimedProfile.h>

// --------------------------------------------------------------
// Set the default configuration values
void TimedProfile::SetDefaultConfiguration (void)
{
	Configuration.ShutterOpenInterval = 350;    // milliseconds
	Configuration.ShutterClosedInterval = 1000; // milliseconds
	Configuration.StartupDelay = 3000;          // milliseconds
	Configuration.Repeats = 3;
}

// --------------------------------------------------------------
// Return a boolean indicating whether or not the current configuration
// is valid
bool TimedProfile::ConfigurationIsValid (timed_profile_config_struct* theConfig)
{
	// Invalid conditions:
	// - repeats are 0
	// - both shutter open time and shutter closed time are < 250 ms
	if ((theConfig->Repeats == 0) ||
		((theConfig->ShutterOpenInterval < 250) &&
			 (theConfig->ShutterClosedInterval < 250)))
	{
		return false;
	}
	else
	{
		return true;
	}		
}

// --------------------------------------------------------------
TimedProfile::TimedProfile (
		TwoStateOutput* theRelay,
		unsigned short	offsetInEEPROM,
		unsigned long   updateInterval) // in milliseconds
: ProfileBase (theRelay, offsetInEEPROM, updateInterval)
{
	// Read our configuration from EEPROM
	if (ReadConfig (&Configuration, sizeof (Configuration)) == false)
	{
		// Something went wrong, so use defaults
		SetDefaultConfiguration();
		
		// And update EEPROM
		WriteConfig (&Configuration, sizeof (Configuration));
	}
	else
	{
		// Validate the configuration
		
		// If configuration is not valid ...
		if (ConfigurationIsValid(&Configuration) == false)
		{
			SetDefaultConfiguration();
			
			// And update EEPROM
			WriteConfig (&Configuration, sizeof (Configuration));
		}
	}
	
	// Value to force GetStatusString() to return a new value first time it is called
	OldRepeatsLeft = 0;
}

	
// --------------------------------------------------------------
// Set the configuration from the string passed in. Returns 0
// if configuration is valid. If configuration is not valid, original
// configuration remains in effect and -1 is returned.
int TimedProfile::SetConfig (CmdParser* theParser)
{
	int returnValue = 0;
	
	// Temporary storage for the new configuration, until we make sure
	// it's okay.
	timed_profile_config_struct newConfig;
	
	// Start with current configuration
	memcpy (&newConfig, &Configuration, sizeof (Configuration));
	
	// Configuration string that comes in from controlling program should
	// be in the following format
	// -o <shutter open msec> -c <shutter close msec> -r <repeats> -s <startup delay msec>
	//
	// These values can be supplied in any order and if some are left out, the
	// existing configuration items are used for those values.
	
	char cmd;
	while (((cmd = theParser->GetCommand()) != 0x00) & (returnValue == 0))
	{
		// All commands currently take an unsigned long parameter, so
		// let's recover it first
		unsigned long value = theParser->GetUnsignedLong();
		
		switch (toupper(cmd))
		{
		case 'O':
			newConfig.ShutterOpenInterval = value;
			break;
			
		case 'C':
			newConfig.ShutterClosedInterval = value;
			break;
			
		case 'R':
			newConfig.Repeats = value;
			break;
			
		case 'S':
			newConfig.StartupDelay = value;
			break;
			
		default:
			Serial.println (F("Bad command code"));
			returnValue = -1;
			break;
		}
	}
	
	// If no errors were encountered during parsing ...
	if (returnValue == 0)
	{
		if (ConfigurationIsValid(&newConfig) == false)
		{
			SetDefaultConfiguration();
			WriteConfig(&Configuration, sizeof (Configuration));
			returnValue = -1;
		}
		else
		{
			memcpy (&Configuration, &newConfig, sizeof(Configuration));
			WriteConfig(&Configuration, sizeof (Configuration));
		}
	}
	return (returnValue);
}
	
// --------------------------------------------------------------
// Write the configuration in human readable format to the
// supplied stream.
void TimedProfile::GetConfig (void)
{

	Serial.print   (F("Startup Delay: "));
	Serial.print   (Configuration.StartupDelay);
	Serial.println (F(" msec"));
	
	Serial.print   (F("Shutter Open Interval: "));
	Serial.print   (Configuration.ShutterOpenInterval);
	Serial.println (F(" msec"));
	
	Serial.print   (F("Shutter Close Interval: "));
	Serial.print   (Configuration.ShutterClosedInterval);
	Serial.println (F(" msec"));
	
	Serial.print   (F("Repeats: "));
	Serial.println (Configuration.Repeats);
}
	
// --------------------------------------------------------------
// Called to make this profile run
void TimedProfile::Run (void)
{
	// If we are already running, stop first
	Stop();
	
	// If we have a startup delay
	if (Configuration.StartupDelay)
	{
		CurrentState = STARTUP_DELAY;
		TimeToNextEvent = Configuration.StartupDelay;
	}
	else
	{
		CurrentState = SHUTTER_CLOSED;
		TimeToNextEvent = 0;
	}

	RepeatsLeft = Configuration.Repeats;
	IsRunning = true;
}
	
	
// --------------------------------------------------------------
// Method called by the controlling program every UpdateInterval 
// milliseconds to execute the profile. Returns false if profile is
// done running and true if profile can continue to run.
bool TimedProfile::Update (void)
{
	bool returnValue = IsRunning;

	// If we're not currently running, no need to continue
	if (IsRunning)
	{
		// If it's time to do something ...
		if (TimeToNextEvent < UpdateInterval)
		{
			// Figure out what to do
			switch (CurrentState)
			{
			  case STARTUP_DELAY:
			  case SHUTTER_CLOSED:
			  	  
			  	  // Open the shutter
			  	  TheRelay->SetOn();

			  	  // If the shutter open time is zero, close
			  	  // it again immediately
			  	  if (Configuration.ShutterOpenInterval == 0)
			  	  {
			  	  	  // Close the shutter
			  	  	  TheRelay->SetOff();

			  	  	  CurrentState = SHUTTER_CLOSED;
			  	  	  TimeToNextEvent = Configuration.ShutterClosedInterval;
			  	  	  
			  	  }
			  	  else // open time is not zero
			  	  {
			  	  	  CurrentState = SHUTTER_OPEN;
			  	  	  TimeToNextEvent = Configuration.ShutterOpenInterval;
			  	  }
			  	  break;
			  	  
			  case SHUTTER_OPEN:
			  	  // Close the shutter
			  	  TheRelay->SetOff();
			  	  
			  	  // At this point, we've completed a cycle. If
			  	  // we have a non-infinite repeat
			  	  if (RepeatsLeft != -1)
			  	  {
			  	  	  RepeatsLeft --;
			  	  	  if (RepeatsLeft == 0)
			  	  	  {
			  	  	  	  returnValue = false;
			  	  	  }
			  	  	  else
			  	  	  {
			  	  		// If the shutter close time is zero, open
			  	  		// it again immediately
			  	  		if (Configuration.ShutterClosedInterval == 0)
			  	  		{
			  	  			// Open the shutter
			  	  			TheRelay->SetOn();
			  	  			CurrentState = SHUTTER_OPEN;
			  	  			TimeToNextEvent = Configuration.ShutterOpenInterval;
			  	  	  
			  	  		}
			  	  		else // closed time is not zero
			  	  		{
			  	  			CurrentState = SHUTTER_CLOSED;
			  	  			TimeToNextEvent = Configuration.ShutterClosedInterval;
			  	  		}
			  	  	  }
			  	  }
			  	  break;
			}
		}
		else  // not time to do anything yet
		{
			TimeToNextEvent -= UpdateInterval;
		}
	}
	
	return (returnValue);		
}	

// --------------------------------------------------------------
// Set configuration items individually
void TimedProfile::SetRepeats (unsigned long theRepeats)
{
	Configuration.Repeats = theRepeats;
	WriteConfig(&Configuration, sizeof (Configuration));
}

// --------------------------------------------------------------
void TimedProfile::SetStartupDelay (unsigned long theDelay)
{
	Configuration.StartupDelay = theDelay;
	WriteConfig(&Configuration, sizeof (Configuration));
}

// --------------------------------------------------------------
void TimedProfile::SetShutterOpenInterval (unsigned long theInterval)
{
	Configuration.ShutterOpenInterval = theInterval;
	WriteConfig(&Configuration, sizeof (Configuration));
}

// --------------------------------------------------------------
void TimedProfile::SetShutterClosedInterval (unsigned long theInterval)
{
	Configuration.ShutterClosedInterval = theInterval;
	WriteConfig(&Configuration, sizeof (Configuration));
}
	
// --------------------------------------------------------------
// Copy a status string into returnString (maxLen-1 characters) while running so it can be
// displayed on whatever output device. It is up to the caller to decide whether or not to use
// returnString if the profile is not currently running.
// Returns true if returnString has changed since the last time this function wass
// called and false otherwise.
bool TimedProfile::GetStatusString (char* returnString, unsigned char maxLen)
{
	bool returnValue = false;

	
	// Create the status string. Do this regardless of whether or not it has changed since last time
	// for scrolling devices such as the serial terminal
	if (IsRunning)
		snprintf (returnString, maxLen-1, "%u", RepeatsLeft);
	else
		snprintf (returnString, maxLen-1, "Stand by");
	
	// if the number of repeats left (that's what we return) has changed since last time, update
	// the return string 
	if (RepeatsLeft != OldRepeatsLeft)
	{
		OldRepeatsLeft = RepeatsLeft;
		returnValue = true;
	}
	return (returnValue);
}
