#ifndef _APPBASE_H
#define _APPBASE_H

/*
Copyright � 2024 Scott Henwood/shensicle photographic. All Rights Reserved.

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
#include <ProfileBase.h>

class ApplicationBase
{
protected:

	// Index of the current profile - the first one is 0
	unsigned short CurrentProfile;
	
	// Indicates the number of profiles supported by this application
	unsigned short NumProfiles;

	// List of the profiles defined in the system
	ProfileBase** ProfileList;
	
	// A flag which, when set, indicates the current profile is actively
	// running.
	bool IsRunning;
	
	// The offset of our configuration from the beginning of EEPROM
	unsigned short ConfigurationOffset;
	
	// Pointer to the configuration structure. This is passed in by derived
	// classes as they are responsible for allocating the space.The last byte of this 
	//structure must be reserved to store a checksum.
	char* ConfigurationPtr;
	
	// The number of bytes in the structure pointed to by ConfigurationPtr;
	unsigned short ConfigurationSize;
	
	// Read our configuration from EEPROM. Return 0 on success, -1 on error
	virtual int ReadConfiguration (void);
	
	// Write our configuration to EEPROM.
	virtual void WriteConfiguration (void);
	
	//Set our default configuration in case EEPROM is un-initialized or
	// corrupted.
	virtual void SetDefaultConfiguration (void) = 0;
	
public:
	// Constructor
	ApplicationBase (
		unsigned short configOffset,
		char*          configPtr,
		unsigned short configSize,
		unsigned short supportedProfiles,
		ProfileBase**  profileList
		);
	
	// Make the specified profile current. If application is running, it is
	// stopped.
	virtual void SetCurrentProfile (unsigned short theProfile);
	
	// Return the index of the current profile; first one is 0.
	unsigned short inline GetCurrentProfile (void)
	  { return (CurrentProfile); }
	
	// Returns a flag which, when set, indicates we are runnig
	bool inline ProfileIsRunning (void) { return (IsRunning); }
	
	// Run the current profile. If it is already running, it is stoppped and
	// restarted
	virtual void Run (void);
	
	// Stop running the current profile. If it's not running, nothing is
	// done.
	virtual void Stop (void);
	
	// Toggle the current state - Run -> Stop and Stop -> run. This feature
	// allows for a single button press to stop or start.
	void ToggleRunStopState (void);	

	// Called by main loop to update us - specifically so LEDs can be updated
	virtual void Update (void);
	
	// Prints our current configuration to the serial port
	void GetConfig(void);
	
	// Return a pointer to the profile whose number is passed in (first one
	// is zero. If profile number is invalid, return NULL.
	ProfileBase* GetProfile (unsigned short profileNum);
};

#endif

