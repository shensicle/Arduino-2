#ifndef _LCDITEMBASE_H
#define _LCDITEMBASE_H

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

// @@@ is this defined elsewhere??
#define MAX_LCD_COLUMNS 12  // This should be used in constructors of derived classes for default values

// Make it easier to support LCD modules from different manufacturers
#include <Wire.h>

// @@@ 
#include <LCDApp.h>

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
                                  
// ==================================================================
class LCDItemBase
{
protected:
	bool IsInEditMode;
	bool IsInErrorMode;
	
	// Storage for our label
	char TheLabel[MAX_LCD_COLUMNS];
		
	// Returns a flag which, when set, indicates that the results of an edit
	// are valid.
	virtual bool IsValid (void) = 0;
	
	// Pointer to the application
	LCDApplication* TheApp;
	
	// Erase the specified text line by drawing a rectangle in the current background colour
	void EraseTextLine (unsigned lineNumber);
	
	// Called when edit mode has been terminated and the results of the edit
	// have been determined to be valid. In most cases, this will be
	// overriden in derived classes to take some action based on the results
	// of the edit (like saving the new configuration). On its own, this
	// method restores the non-edit display and sets IsInEditMode to false.
	virtual void ApplyEdit (void);
	
	// Display the current value of this LCD item on the second line of
	// the display.
	virtual void DisplayCurrentValue (void) = 0;
	
	// Render the specified line of the display right-justified. Leading 
	// blanks are added as necessary. Parameter is a null-terminated 
	// string.Line number defaults to 1 (ie. the second line).
	// Returns the display column (starting at 0) of the first character in the string.
	int RenderLineRightJustified (char* theString, 
		                      unsigned short lineNum = 1);
	
	// Render the specified line of the display center-justified. Leading 
	// and trailing blanks are added as necessary. Parameter is a 
	// null-terminated string.Line number defaults to 0 (ie. the first line).
	// Returns the display column (starting at 0) of the first character in the string.
	int RenderLineCenterJustified (char* theString, 
	   	                       unsigned short lineNum = 0);

		
	// Display an error when editing fails. Ideally, derived classes will
	// override.
	virtual void DisplayError (void) {RenderLineRightJustified ("ERROR"); }
	
	// Fetch data that this object will display on the second line and load
	// object attributes accordingly. This is intended to be called from the
	// OnEntry() and Update() methods of derived classes. This method returns
	// a flag which, when set, indicates that the data value has changed since
	// the last time FetchData() was called. We do this because LCD updates 
	// are fairly time-consuming and we only want to do them when necessary.
	virtual int FetchData (void) = 0;
	
public:
	// Constructor
	LCDItemBase (char*                theLabel,
		         LCDApplication*      theApp);
	
	// Handle a left button press. This should only be called when we are
	// in edit mode.
	virtual void OnLeft (void) = 0;
	
	// Handle a right button press. This should only be called when we are
	// in edit mode.
	virtual void OnRight (void) = 0;
	
	// Handle an up button press. This should only be called when we are
	// in edit mode.
	virtual void OnUp (void) = 0;
	
	// Handle a down button press. This should only be called when we are
	// in edit mode.
	virtual void OnDown (void) = 0;
	
	// Handle an Enter button press. This should only be called when we are
	// in edit mode.
	virtual void OnEnter (void);
	
	// Handle a cancel button press. This should only be called when we are
	// in edit mode.
	virtual void OnCancel (void);
	
	// Called when this item becomes active on the display. This method
	// displays the item title passed into the constructor on line 1 and
	// Calls DisplayCurrentValue() to display the item's value on line 2.
	virtual void OnEntry (void);
	
	// Returns a flag which, when set, indicates that this item is being
	// edited
	bool IsBeingEdited (void) { return (IsInEditMode); }
	
	// Must be defined in derived classes. Should change the display so that
	// there is a visual indication that value is being edited.
	virtual void OnEditModeEntry (void);
	
	// Called periodically by application to update the display (in case values
	// have been changed by the terminal interface or other system components.
	// In this default version, DisplayCurrentValue() is called as long as
	// we are not in error mode or edit mode.
	virtual void Update (void);
};

#endif

