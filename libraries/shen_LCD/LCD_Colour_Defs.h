#ifndef LCD_COLOUR_DEFS
#define LCD_COLOUR_DEFS

/*
Copyright  2024 Scott Henwood/shensicle photographic. All Rights Reserved.

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

// These are the colours that the LCD foreground and background can be set to
const char* LCDColourLabels[] = {"Black", "White", "Red", "Green", "Blue", "Cyan", "Magenta", "Yellow", "Orange" };
const short  LCDColourSettings[] = {ST77XX_BLACK , ST77XX_WHITE, ST77XX_RED, ST77XX_GREEN, ST77XX_BLUE, ST77XX_CYAN, ST77XX_MAGENTA, ST77XX_YELLOW, ST77XX_ORANGE };

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

#endif
