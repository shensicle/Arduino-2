#ifndef _LCDUNSIGNEDITEM_H
#define _LCDUNSIGNEDITEM_H

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

#include <LCDItemBase.h>

// ===========================================================================
class LCDUnsignedItem : public LCDItemBase
{
protected:

	// Storage for our  value
	unsigned long TheValue;
	
	// The minimum allowable value
	unsigned long MinValue;
	
	// The maximum allowable value
	unsigned long MaxValue;
	
	// The number of columns to use to display this value
	unsigned char NumColumns;
	
	// The offset of our string from the leftmost column of the LCD (in characters)
	int StringOffset;
	
	//Storage for the value we're going to display on the LCD
	char TheString [MAX_LCD_COLUMNS];
	
	// Pointer to the character of TheString we are currently editing
	// while in edit mode.
	int EditorIndex;

	// Function that returns the x value of the current character (in pixels) from the leftmost side of the display
	int CharXPosPixels(void)
	{ return ((StringOffset+EditorIndex)*LCD_CHAR_WIDTH_PIXELS+LCD_TEXT_COLUMN); }	
	
	// Turn the cursor on. Cursor is a horizontal line under the character of interest
	void CursorOn (void);
	
	// Turn the cursor off. Current bruit-force method is to clear the entire cursor area
	void CursorOff (void);
	
	// Set the character at the current position to the value provided. This requires drawing a rectangle in the background colour to erase to previous character, if any
	void SetCharacter (char theChar);

	// Returns a flag which, when set, indicates that the results of an edit
	// are valid
	virtual bool IsValid (void);
	
	// Must be defined in derived classes. Should change the display so that
	// there is a visual indication that value is being edited.
	virtual void OnEditModeEntry (void);
	
	// Called when edit mode has been terminated and the results of the edit
	// have been determined to be valid. In most cases, this will be
	// overriden in derived classes to take some action based on the results
	// of the edit (like saving the new configuration). On its own, this
	// method restores the non-edit display and sets IsInEditMode to false.
	virtual void ApplyEdit (void);
	
	// Display the current value of this LCD item on the second line of
	// the display.
	virtual void DisplayCurrentValue (void);
		
public:
	// Constructor
	LCDUnsignedItem  (shen_LCD*            theLCD,
		          	  char*  	           theLabel, 
		              LCDApplication*      theApp,
		              unsigned long        minValue, 
		              unsigned long        maxValue,
		              unsigned char        numChars)
	   : LCDItemBase (theLCD, theLabel, theApp) 
	      { MinValue = minValue; MaxValue = maxValue; NumColumns = numChars;} 
	
	// Handle a left button press. This should only be called when we are
	// in edit mode. This button moves to the left (with wrap-around) so
	// we can change the previous digit with the up/down keys.
	virtual void OnLeft (void);
	
	// Handle a right button press. This should only be called when we are
	// in edit mode. This button moves to the right (with wrap-around) so
	// we can change the next digit with the up/down keys.
	virtual void OnRight (void);
	
	// Handle an up button press. This should only be called when we are
	// in edit mode. It increments the digit currently being edited by 1.
	virtual void OnUp (void);
	
	// Handle a down button press. This should only be called when we are
	// in edit mode. It decrements the digit currently being edited by 1.
	virtual void OnDown (void);
		
	// Handle an Enter button press. This should only be called when we are
	// in edit mode. Base class functionality is mostly fine here. We just
	// have to turn off the cursor as well.
	virtual void OnEnter (void);
};

#endif

