#ifndef _PROFILEBASE_H
#define _PROFILEBASE_H

/*
Copyright � 2012 Scott Henwood/shensicle photographic. All Rights Reserved.

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

#include <TwoStateOutput.h>
#include <CmdParser.h>

class ProfileBase
{
protected:
	
	TwoStateOutput* TheRelay;
	unsigned short  OffsetInEEPROM;
	unsigned long   UpdateInterval;   // milliseconds
	
	// Read this profile's configuration from EEPROM and validate the
	// checksum. Return true if checksum okay and false otherwise
	bool ReadConfig (void* ptrToConfig, int configSize);
	
	// Write this profile's configuration to EEPROM, calculate the checksum
	// and write the checksum following the configuration data
	void WriteConfig (void* ptrToConfig, int configSize);
	
	// A flag which, when set, indicates we are currently running
	bool IsRunning;
	
public:
	ProfileBase (TwoStateOutput* theRelay,
		         unsigned short  offsetInEEPROM,
		         unsigned long   updateInterval);
	
	// Set the configuration from the string passed in. Returns 0
	// if configuration is valid. If configuration is not valid, original
	// configuration remains in effect and -1 is returned.
	virtual int SetConfig (CmdParser* theParser) = 0;
	
	// Write the configuration in human readable format to the
	// serial port.
	virtual void GetConfig (void) = 0;
	
	// Called to make this profile run
	virtual void Run (void) = 0;
	
	// Called to make this profile stop running
	virtual void Stop (void);
	
	// Make this program the current profile
	void SetCurrent (void);
	
	// Called when this profile is no longer the current profile
	void UnsetCurrent (void);
	
	// Method called by the controlling program every UpdateInterval 
	// milliseconds to execute the profile. Returns false if profile is
	// done running and true if profile can continue to run.
	virtual bool Update (void) = 0;
};

#endif

	
