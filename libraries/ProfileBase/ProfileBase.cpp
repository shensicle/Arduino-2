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
#include <eeprom.h>

// --------------------------------------------------------------
// Read this program's configuration from EEPROM and validate the
// checksum. Return true if checksum okay and false otherwise
bool ProfileBase::ReadConfig (void* ptrToConfig, int configSize)
{
	bool returnValue = true;
	
	char* nextByte = (char*)ptrToConfig;
	byte checksum = 0;
	
	for (int i = 0; i < configSize; i++)
	{
		*nextByte = EEPROM.read(OffsetInEEPROM+i);
		checksum += *nextByte;
		nextByte ++;
	}
	
	// Read the checksum value stored in EEPROM
	checksum += EEPROM.read (OffsetInEEPROM+configSize);
	
	if (checksum != 0xff)
		returnValue = false;
	
	return (returnValue);
}
	
// --------------------------------------------------------------
// Write this profile's configuration to EEPROM, calculate the checksum
// and write the checksum following the configuration data
void ProfileBase::WriteConfig (void* ptrToConfig, int configSize)
{
	
	char* nextByte = (char*)ptrToConfig;
	byte checksum = 0;
	
	for (int i = 0; i < configSize; i++)
	{
		EEPROM.write(OffsetInEEPROM+i, *nextByte);
		checksum += *nextByte;
		nextByte ++;
	}
	
	checksum = 0xff - checksum;
	
	// Write the checksum value to EEPROM as well
	EEPROM.write (OffsetInEEPROM+configSize, checksum);
	EEPROM.commit();
}
	

// --------------------------------------------------------------
ProfileBase::ProfileBase (TwoStateOutput* theRelay,
		          unsigned short  offsetInEEPROM,
		          unsigned long   updateInterval)
{
	TheRelay = theRelay;
	OffsetInEEPROM = offsetInEEPROM;
	UpdateInterval = updateInterval;
	
	IsRunning = false;
}
	
// --------------------------------------------------------------
// Make this profile the current profile
void ProfileBase::SetCurrent (void)
{
	Stop();
}

// --------------------------------------------------------------
// Called when this profile is no longer the current profile
void ProfileBase::UnsetCurrent (void)
{
	Stop();
}


// --------------------------------------------------------------
// Called to make this profile stop running
void ProfileBase::Stop (void)
{
	// Turn off the relay if it's on
	TheRelay->SetOff();
	
	// Update our status
	IsRunning = false;
	
}
