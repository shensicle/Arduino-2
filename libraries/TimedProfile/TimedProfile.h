#ifndef _TIMEDPROFILE_H
#define _TIMEDPROFILE_H

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


#include <ProfileBase.h>
#include <CmdParser.h>

class TimedProfile : public ProfileBase
{
private:
	typedef struct 
	{
		unsigned long  ShutterOpenInterval;   // milliseconds
		unsigned long  ShutterClosedInterval; // milliseconds
		unsigned long  StartupDelay;          // milliseconds
		unsigned short Repeats;
	} timed_profile_config_struct;
	
	// The various states this profile can be in once running
	typedef enum {STARTUP_DELAY, SHUTTER_OPEN, SHUTTER_CLOSED} state_type;

protected:
	// Stores our configuration in RAM
	timed_profile_config_struct Configuration;

	// The state we are in when we are running
	state_type CurrentState;
	
	// Number of milliseconds remaining until the next event - ie. until
	// we need to do something.
	unsigned long TimeToNextEvent;
	
	// Number of repeats we have left in the current run cycle
	unsigned short RepeatsLeft;
	
	// Used by GetStatusString() to determine whether or not the status string has
	// changed since the last time it was called. This is used to return a flag to the
	// caller to indicate whether or not a display needs to be updated.
	unsigned short OldRepeatsLeft;

	// Set the default configuration values
	void SetDefaultConfiguration (void);
	
	// Return a boolean indicating whether or not the current configuration
	// is valid
	bool ConfigurationIsValid (timed_profile_config_struct* theConfig);
		
public:
	TimedProfile (
		TwoStateOutput* theRelay,
		unsigned short	offsetInEEPROM,
		unsigned long   updateInterval); // in milliseconds
	
	// Set the configuration from the string passed in. Returns 0
	// if configuration is valid. If configuration is not valid, original
	// configuration remains in effect and -1 is returned.
	virtual int SetConfig (CmdParser* theParser);
	
	// Set configuration items individually
	void SetRepeats (unsigned long theRepeats);
	void SetStartupDelay (unsigned long theDelay);
	void SetShutterOpenInterval (unsigned long theInterval);
	void SetShutterClosedInterval (unsigned long theInterval);
	
	// Get configuration items individually
	unsigned long inline GetRepeats (void) { return (Configuration.Repeats); }
	unsigned long inline GetStartupDelay (void) { return (Configuration.StartupDelay); }
	unsigned long inline GetShutterOpenInterval (void) { return (Configuration.ShutterOpenInterval); }
	unsigned long inline GetShutterClosedInterval (void) { return (Configuration.ShutterClosedInterval); }
	
	// Write the configuration in human readable format to the
	// serial port
	virtual void GetConfig (void);
	
	// Called to make this profile run
	virtual void Run (void);
	
	// Called to make this profile stop
	virtual void Stop (void);
		
	// Method called by the controlling program every UpdateInterval 
	// milliseconds to execute the profile. Returns false if profile is
	// done running and true if profile can continue to run.
	// Copy a status string into returnString (maxLen-1 characters) while running so it can be
	// displayed on whatever output device. Return string is set to Null if profile is not running
	virtual bool Update (void);

	// Copy a status string into returnString (maxLen-1 characters) while running so it can be
	// displayed on whatever output device. It is up to the caller to decide whether or not to use
	// returnString if the profile is not currently running.
	// Returns true if returnString has changed since the last time this function wass
	// called and false otherwise.
	virtual bool GetStatusString (char* returnString, unsigned char maxLen);
};

#endif

