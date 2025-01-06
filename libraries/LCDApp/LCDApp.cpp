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

#include <LCDApp.h>
#include <EEPROM.h>

// --------------------------------------------------------------------------- 
//Set our default configuration in case EEPROM is un-initialized or
// corrupted.
void LCDApplication::SetDefaultConfiguration (void)
{
	  LCDAppConfig.VersionNumber = 1;
	  LCDAppConfig.BackgroundColour = LCD_COLOUR_INDEX_BLACK;
	  LCDAppConfig.ForegroundColour = LCD_COLOUR_INDEX_RED;
	  
	  LCDAppConfig.Spare[0] = 0;
	  LCDAppConfig.Spare[1] = 0;
	  LCDAppConfig.Spare[2] = 0;
	  LCDAppConfig.Spare[3] = 0;
}
	
// --------------------------------------------------------------------------- 
// Constructor
LCDApplication::LCDApplication (
	unsigned short configOffset,
	unsigned short supportedProfiles,
	ProfileBase**  programList,
    unsigned       chipSelect,
        unsigned       dc,
        unsigned       reset)
	  : ApplicationBase (configOffset, 
                         (char*)&LCDAppConfig,
                         sizeof(LCDAppConfig),
                         supportedProfiles, 
                         programList),
        Adafruit_ST7735 (chipSelect,dc,reset)
{
	  // Read application configuration settings from EEPROM  
	  if (ReadConfiguration() == -1)
	  {
	  	  // Something went wrong - revert to defaults
	  	  SetDefaultConfiguration();
	  	  WriteConfiguration();
	  }
}

// --------------------------------------------------------------------------- 
void LCDApplication::SetBackgroundColour (char theColour)
{
	LCDAppConfig.BackgroundColour = theColour;
	WriteConfiguration();
}

	
// --------------------------------------------------------------------------- 
char LCDApplication::GetBackgroundColour (void)
{
	return (LCDAppConfig.BackgroundColour);
}

/// --------------------------------------------------------------------------- 
void LCDApplication::SetForegroundColour (char theColour)
{
	LCDAppConfig.ForegroundColour = theColour;
	WriteConfiguration();
}

	
// --------------------------------------------------------------------------- 
char LCDApplication::GetForegroundColour (void)
{
	return (LCDAppConfig.ForegroundColour);
}

// --------------------------------------------------------------------------- 
short LCDApplication::GetForegroundColourHW (void)
{
		return (LCDColourSettings[LCDAppConfig.ForegroundColour]);
}

// --------------------------------------------------------------------------- 
short LCDApplication::GetBackgroundColourHW(void)
{
		return (LCDColourSettings[LCDAppConfig.BackgroundColour]);		
}

// --------------------------------------------------------------------------- 
// Initialize the LCD display
void LCDApplication::InitializeDisplay (void)
{
    initR(INITR_BLACKTAB);
    fillScreen(GetBackgroundColourHW());
    setRotation(1);  // Landscape - change 3 to 1 if display is upside down
    setTextWrap(false);
    setTextColor(GetForegroundColourHW());
    setTextSize(LCD_DEFAULT_TEXT_SIZE);
}
				
// --------------------------------------------------------------------------- 
// Set text on the title line (first row) of the display
void LCDApplication::SetTitleLine (char* theString)
{
	// @@@ Need to do an erase here
	
    setCursor(LCD_TEXT_COLUMN, LCD_TEXT_ROW_1);
    print(theString);
}
	
// --------------------------------------------------------------------------- 
// Set text on the status line (row 2) of the display
void LCDApplication::SetStatusLine (char* theString)
{
	// @@@ Need to do an erase here

	// Status line text is smaller
    setTextSize(1);
    print(theString);
    
    // Put the text size back to default so we don't have to set it every time we do an LCD update'
    setTextSize(LCD_DEFAULT_TEXT_SIZE);
}
