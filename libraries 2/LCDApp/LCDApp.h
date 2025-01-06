#ifndef _LCDAPP_H
#define _LCDAPP_H

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
#include <AppBase.h>
#include <shen_LCD.h>

// ==========================================================
class LCDApplication : public ApplicationBase
{
protected:

	// Structure used to store the application's current configuration in EEPROM
	typedef struct
	{
		byte  VersionNumber;
		short BackgroundColour;
		short ForegroundColour;
		long  Spare[4];
		byte  Checksum;
	} lcd_app_config_struct;
	lcd_app_config_struct LCDAppConfig;
	
	// Set our default configuration in case EEPROM is un-initialized or
	// corrupted.
	virtual void SetDefaultConfiguration (void);
	
	// A pointer to the LCD object
	shen_LCD* TheLCD;
	
public:
	// Constructor
	LCDApplication (
		unsigned short configOffset,
		unsigned short supportedProfiles,
		ProfileBase**  profileList,
		shen_LCD*      theLCD
		);
	
	// Set/Get background colour
	void SetBackgroundColour (char theColour);
	char GetBackgroundColour (void);
	
	// Set/Get foreground colour
	void SetForegroundColour (char theColour);
	char GetForegroundColour (void);
	
	// Return the hardware setting value for the current foreground and background colours
	short GetForegroundColourHW (void);
	short GetBackgroundColourHW(void);
				
	// Called by main loop to update us, mostly so the LCD display gets 
	// updated
	virtual void Update (void);
};

#endif

