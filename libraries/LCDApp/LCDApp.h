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

// For graphical LCD display
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
//#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>

#include <ProfileBase.h>
#include <AppBase.h>

// These are the colours that the LCD foreground and background can be set to
static const char* LCDColourLabels[] = {"Black", "White", "Red", "Green", "Blue", "Cyan", "Magenta", "Yellow", "Orange" };
static const short  LCDColourSettings[] = {ST77XX_BLACK , ST77XX_WHITE, ST77XX_RED, ST77XX_GREEN, ST77XX_BLUE, ST77XX_CYAN, ST77XX_MAGENTA, ST77XX_YELLOW, ST77XX_ORANGE };

// These are the indices of the colours that the LCD foreground and background can be set to. Must match LCDColourLabels and LCDColourSettings define in ino file @@@ Clean up, this is crap
#define LCD_COLOUR_INDEX_BLACK   0
#define LCD_COLOUR_INDEX_WHITE   1
#define LCD_COLOUR_INDEX_RED     2
#define LCD_COLOUR_INDEX_GREEN   3
#define LCD_COLOUR_INDEX_BLUE    4
#define LCD_COLOUR_INDEX_CYAN    5
#define LCD_COLOUR_INDEX_MAGENTA 6
#define LCD_COLOUR_INDEX_YELLOW  7
#define LCD_COLOUR_INDEX_ORANGE  8

// LCD Row and column starts for text display
#define LCD_DEFAULT_TEXT_SIZE 2

// Where we want text to start
#define LCD_TEXT_COLUMN 8
#define LCD_TEXT_ROW_1 5
#define LCD_TEXT_ROW_2 35
#define LCD_TEXT_ROW_3 75
#define LCD_TEXT_ROW_4 105

// Number of pixels between the bottom of a row of text and the cursor
#define LCD_CURSOR_GAP 2

// This is hard-coded for a text size of 3 - should probably be in LCDApp @@@
#define LCD_CHAR_WIDTH_PIXELS 12
#define LCD_CHAR_HEIGHT_PIXELS 16
#define LCD_DISPLAY_WIDTH_PIXELS 160 - LCD_TEXT_COLUMN
#define LCD_DISPLAY_HEIGHT_PIXELS 120

const unsigned LCDRowOffsets[] = {LCD_TEXT_ROW_1, 
                                  LCD_TEXT_ROW_2, 
                                  LCD_TEXT_ROW_3,
                                  LCD_TEXT_ROW_4};
                                  
// ==========================================================
class LCDApplication : public ApplicationBase, public Adafruit_ST7735
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

	
public:
	// Constructor
	LCDApplication (
		unsigned short configOffset,
		unsigned short supportedProfiles,
		ProfileBase**  profileList,
		unsigned       chipSelect,
        unsigned       dc,
        unsigned       reset
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
			
    // Initialize the LCD display
    void InitializeDisplay (void);
				
	// Set text on the title line (first row) of the display
	void SetTitleLine (char* theString);
	
	// Set text on the status line (row 2) of the display
	void SetStatusLine (char* theString);
	
};

#endif
