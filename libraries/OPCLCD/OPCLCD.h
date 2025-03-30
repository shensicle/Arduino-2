#ifndef _OPCLCD_H
#define _OPCLCD_H

/*
Copyright  2025 Scott Henwood/shensicle photographic. All Rights Reserved.

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

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Physical device
#define LCD_SCREEN_WIDTH 128 // OLED display width, in pixels
#define LCD_SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// One-pixel camera LCD manager

class OPCLCD : protected Adafruit_SSD1306
{
public:
	
    OPCLCD ()
            : Display (LCD_SCREEN_WIDTH, LCD_SCREEN_HEIGHT, &Wire, OLED_RESET),
            BannerOriginX  (0),
            BannerOriginY ( 0),
            HeaderOriginX  (0),
            HeaderOriginY (30),
            StatusOriginX  (0),
            StatusOriginY (40),
            BannerTextSize (2),
            HeaderTextSize (1),
            StatusTextSize (2)
            {};
    
    // Initialize the display and return a flag indicating whether or not
    // initialization was successful
    bool Begin (void);
    
    // Banner text goes at the top of the display
    void DisplayBannerLine (char* theTitle);
    
    // Header text introduces the status
    void DisplayHeadingLine (char* theHeading = "Status:");
    
    // What's currently going on
    void DisplayStatusLine (char* theStatus); 
    void DisplayStatusPercent (int statusVal, char* suffix = " %");
	
	
protected:

    Adafruit_SSD1306 Display;
    
    int BannerOriginX, BannerOriginY, HeaderOriginX, HeaderOriginY, StatusOriginX, StatusOriginY,
        BannerTextSize, HeaderTextSize, StatusTextSize;
    
	// Helper to print a single line of text
	void DisplayTextLine (char* theText,
	                      int   textSize,
	                      int   textColour,
	                      int   x,
	                      int   y);

};

#endif
