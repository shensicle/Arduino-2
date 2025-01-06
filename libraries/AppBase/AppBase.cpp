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

#include <AppBase.h>
#include <EEPROM.h>
	
// --------------------------------------------------------------------------- 
// Read our configuration from EEPROM. Return 0 on success, -1 on error
int ApplicationBase::ReadConfiguration (void)
{
	byte checksum = 0;
	char* dataPtr = ConfigurationPtr;
	
	// ConfigurationSize-1 because the last byte of the configuration
	// is reserved for a checksum which has to be handled differently.
	for (unsigned short i = 0; i < ConfigurationSize-1; i++)
	{
		*dataPtr = EEPROM.read (ConfigurationOffset+i);
		checksum += *dataPtr++;
	}
   
	// And read the checksum too
	checksum += EEPROM.read (ConfigurationOffset+ConfigurationSize-1);
   
	if (checksum == 0xff)
		return (0);
	else
		return (-1);
}
	
// --------------------------------------------------------------------------- 
// Write our configuration to EEPROM.
void ApplicationBase:: WriteConfiguration (void)
{
	byte checksum = 0;
	char* dataPtr = ConfigurationPtr;
	for (unsigned short i = 0; i < ConfigurationSize-1; i++)
	{
		EEPROM.write (ConfigurationOffset+i, *dataPtr);
		checksum += *dataPtr++;
	}
   
	// And write the checksum too
	EEPROM.write (ConfigurationOffset+ConfigurationSize-1, (0xff-checksum));
	EEPROM.commit();
}

	
// --------------------------------------------------------------------------- 
// Constructor
ApplicationBase::ApplicationBase (
	unsigned short configOffset,
	char*          configPtr,
	unsigned short configSize,
	unsigned short supportedProfiles,
	ProfileBase**  programList
	)
{
	  // Save the offset of our configuration in EEPROM
	  ConfigurationOffset = configOffset;
	
	  // Store a pointer to the structure used to store
	  // persistent application-level configuration by derived classes.
	  ConfigurationPtr = configPtr;
	
	  // Also save the size of the configuration structure.
	  ConfigurationSize = configSize;
		  
	  // We don't save CurrentProfile to EEPROM as it may be updated
	  // frequently and we don't want to wear out the device.
	  CurrentProfile = 0;
	  
	  // Save number of supported programs
	  NumProfiles = supportedProfiles;

	  // Save program list
	  ProfileList = programList;
	  	  
	  // We don't auto run on startup
	  IsRunning = false;
	  
	  // Notify the current profile of its status
	  ProfileList[CurrentProfile]->SetCurrent();
}

// --------------------------------------------------------------------------- 
// Make the specified program current. If application is running, it is
// stopped
void ApplicationBase::SetCurrentProfile (unsigned short theProfile)
{
	// Don't do anything if the new program is not valid
	if (theProfile < NumProfiles)
	{
		ProfileList[CurrentProfile]->UnsetCurrent();
		IsRunning = false;
        
		CurrentProfile = theProfile;        
		ProfileList[CurrentProfile]->SetCurrent();
	}
}

// --------------------------------------------------------------------------- 
// Run the current program. If it is already running, it is stoppped and
// restarted
void ApplicationBase::Run (void)
{
	    ProfileList[CurrentProfile]->Stop();
        IsRunning = true;
        ProfileList[CurrentProfile]->Run();
}
	
// --------------------------------------------------------------------------- 
// Stop running the current program. If it's not running, nothing is
// done.
void ApplicationBase::Stop (void)
{
	ProfileList[CurrentProfile]->Stop();
        IsRunning = false;
}
	
// --------------------------------------------------------------------------- 
// Toggle the current state - Run -> Stop and Stop -> run. This feature
// allows for a single button press to stop or start.
void ApplicationBase::ToggleRunStopState (void)
{
	if (IsRunning)
		Stop();
	else
		Run();
}	

// --------------------------------------------------------------------------- 
// Called by main loop to update us
void ApplicationBase::Update (void)
{
	// If we're running, update the current program
	if (IsRunning)
	{
		IsRunning = ProfileList[CurrentProfile]->Update();
	}
}

// --------------------------------------------------------------------------- 
// Prints our current configuration to the serial port
void ApplicationBase::GetConfig(void)
{
	
	// First, print the system configuration
	Serial.print ("Active Profile: "); Serial.print (GetCurrentProfile()+1); 
          
	if (ProfileIsRunning())
            Serial.print (F(" (Running)"));
        Serial.print ("\n\n");
                  
        // Now print details of each program
        for (int i = 0; i < NumProfiles; i++)
        {
            Serial.print (F("Profile "));Serial.println (i+1);
            ProfileList[i]->GetConfig();
            Serial.println(F(""));
        }
}

// --------------------------------------------------------------------------- 
// Return a pointer to the profile whose number is passed in (first one
// is zero. If profile number is invalid, return NULL.
ProfileBase* ApplicationBase::GetProfile (unsigned short profileNum)
{
	if (profileNum < NumProfiles)
		return (ProfileList[profileNum]);
	else
		return (NULL);
}

