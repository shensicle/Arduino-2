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

#include "OPCLCD.h"

// ---------------------------------------------------------------------------
bool OPCLCD::Begin (void)
{
  bool returnValue = false;
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(Display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { 
      returnValue = true;
      Display.clearDisplay();
  }
  
  return (returnValue);
	
}

// ---------------------------------------------------------------------------
void OPCLCD::DisplayTextLine (char* theText,
	                          int   textSize,
	                          int   textColour,
	                          int   x,
	                          int   y)
{
    Display.setTextSize(textSize);          
    Display.setTextColor(textColour);
    Display.setCursor(x,y);  
    
    // Clear text area
    Display.fillRect(x, y, textSize*10,LCD_SCREEN_WIDTH,BLACK);
    Display.println (theText); 
    Display.display();
}

// ---------------------------------------------------------------------------
// Banner text goes at the top of the display
void OPCLCD::DisplayBannerLine (char* theTitle)
{
	DisplayTextLine (theTitle, BannerTextSize, WHITE, BannerOriginX, BannerOriginY);
}
    
// ---------------------------------------------------------------------------
// Header text introduces the status
void OPCLCD::DisplayHeadingLine (char* theHeading)
{
	DisplayTextLine (theHeading, HeaderTextSize, WHITE, HeaderOriginX, HeaderOriginY);
}
    
// ---------------------------------------------------------------------------
// What's currently going on
void OPCLCD::DisplayStatusLine (char* theStatus)
{
	DisplayTextLine (theStatus, StatusTextSize, WHITE, StatusOriginX, StatusOriginY);
}

// ---------------------------------------------------------------------------
void OPCLCD::DisplayStatusPercent (int statusVal, char* suffix)
{
	static char displayStr[14];
	
	sprintf(displayStr,"%d  %c",statusVal, suffix[0]);
	DisplayStatusLine (displayStr);
}

