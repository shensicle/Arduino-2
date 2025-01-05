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

#include <Arduino.h>
#include <LCDUnsignedItem.h>	


// --------------------------------------------------------------------------- 
// Turn the cursor on. Cursor is a horizontal line under the character of interest
void LCDUnsignedItem::CursorOn (void)
{
	// Clear any old cursors
	CursorOff();
	
	// Draw a line 
	TheApp->drawFastHLine (CharXPosPixels(), LCD_TEXT_ROW_4+LCD_CHAR_HEIGHT_PIXELS+LCD_CURSOR_GAP, LCD_CHAR_WIDTH_PIXELS, TheApp->GetForegroundColourHW());
}
	
// --------------------------------------------------------------------------- 
// Turn the cursor off. Current bruit-force method is to clear the entire cursor area
void LCDUnsignedItem::CursorOff (void)
{
	TheApp->fillRect(0, LCD_TEXT_ROW_4+LCD_CHAR_HEIGHT_PIXELS+LCD_CURSOR_GAP, 160, 2, TheApp->GetBackgroundColourHW());
}

// --------------------------------------------------------------------------- 
// Set the character at the current position to the value provided. This requires drawing a rectangle in the background colour to erase to previous character, if any
void LCDUnsignedItem::SetCharacter (char theChar)
{
	// Erase the current character
	int x = CharXPosPixels();
	
	TheApp->fillRect (x, LCD_TEXT_ROW_4, LCD_CHAR_WIDTH_PIXELS, LCD_CHAR_HEIGHT_PIXELS, TheApp->GetBackgroundColourHW());
	TheApp->setCursor (CharXPosPixels(), LCD_TEXT_ROW_4);
	TheApp->print (theChar);
}

// --------------------------------------------------------------------------- 
// Returns a flag which, when set, indicates that the results of an edit
// are valid
bool LCDUnsignedItem::IsValid (void)
{
	if ((TheValue >= MinValue) && (TheValue <= MaxValue))
		return true;
	else
		return false;
}
	
// --------------------------------------------------------------------------- 
// Must be defined in derived classes. Should change the display so that
// there is a visual indication that value is being edited.
void LCDUnsignedItem::OnEditModeEntry (void)
{
	LCDItemBase::OnEditModeEntry();
	
	// Enable editing of the first character in the string
	EditorIndex = 1;
	
	Serial.printf ("Cursor X is %d\n", StringOffset+EditorIndex);
	
	// Make the cursor visible to show which character can be edited.
	CursorOn();
	
}

// --------------------------------------------------------------------------- 
// Called when edit mode has been terminated and the results of the edit
// have been determined to be valid. In most cases, this will be
// overriden in derived classes to take some action based on the results
// of the edit (like saving the new configuration). On its own, this method 
// updates TheTime with the value justed entered via the display. The base
// class method restores the non-edit display and sets IsInEditMode to false.
void LCDUnsignedItem::ApplyEdit (void)
{
	// We limit range via the editor, so no additional range checking
	// required.
	
	CursorOff();
	
	LCDItemBase::ApplyEdit();
}
	
// --------------------------------------------------------------------------- 
// Display the current value of this LCD item on the second line of
// the display.
void LCDUnsignedItem::DisplayCurrentValue (void)
{
	
	// String index
	int index = 0;
	
	int numColumns = NumColumns;
		
	// If we are in edit mode, display the value with square brackets around
	// it; otherwise display normally.
	if (IsInEditMode)
	{
		// Account for '[' and ']'
		numColumns += 2;
		TheString[index++] = '[';
	}
	
	// Next, convert our value to a string for display.
	snprintf (&TheString[index], numColumns+1, "%04d", TheValue);	// @@@	
	
	// Just in case ...
	TheString[numColumns] = 0x00;

	// Make sure we haven't gone too long
	index = strlen (TheString);
	
	if (IsInEditMode == true)
	{
		if (index >= (numColumns - 1))
			index = numColumns - 1;			

		TheString[index++] = ']';
	}
	else // Not in edit mode
	{
		if (index >= (numColumns))
			index = numColumns;
	}
		
	TheString[index] = 0x00;
	
	StringOffset = RenderLineRightJustified (TheString);
	
}
	
	
// --------------------------------------------------------------------------- 
// Handle a left button press. This should only be called when we are
// in edit mode. This button moves to the left (with wrap-around) so
// we can change the previous digit with the up/down keys.
void LCDUnsignedItem::OnLeft (void)
{
	// In error mode, any button clears the error
	if (IsInErrorMode)
	{
		IsInErrorMode = false;
		IsInEditMode = true;
		OnEditModeEntry();
	}
	else
	{
		EditorIndex --;

		// If we are on the first character, wrap to the end
		if (TheString[EditorIndex] == '[')
		{
			EditorIndex = strlen(TheString) - 2; // Remember the ']' !
		}
		
		// TheLCD->setCursor (EditorIndex+StringOffset, 1);
		CursorOn();
	}
}
	
// --------------------------------------------------------------------------- 
// Handle a right button press. This should only be called when we are
// in edit mode. This button moves to the right (with wrap-around) so
// we can change the next digit with the up/down keys.
void LCDUnsignedItem::OnRight (void)
{
	// In error mode, any button clears the error
	if (IsInErrorMode)
	{
		IsInErrorMode = false;
		IsInEditMode = true;
		OnEditModeEntry();
	}
	else
	{
		EditorIndex ++;

		// If we are on the first character, wrap to the end
		if (TheString[EditorIndex] == ']') // Remember the ']' !
		{
			EditorIndex = 1; // Remember the '[' !
		}
		
		CursorOn();
	}
}
	
// --------------------------------------------------------------------------- 
// Handle an up button press. This should only be called when we are
// in edit mode. It increments the digit currently being edited by 1.
void LCDUnsignedItem::OnUp (void)
{
	// In error mode, any button clears the error
	if (IsInErrorMode)
	{
		IsInErrorMode = false;
		IsInEditMode = true;
		OnEditModeEntry();
	}
	else
	{
		// Increment the current character with wrap around
		if (TheString[EditorIndex] == '9')
			TheString[EditorIndex] = '0';
		else
			TheString[EditorIndex]++;

		// Update the display
		//TheLCD->write (TheString[EditorIndex]);
		SetCharacter (TheString[EditorIndex]);
		
		// Turns out you have to reset cursor after updating LCD
		//TheLCD->setCursor (EditorIndex+StringOffset, 1);
	}
}
	
// --------------------------------------------------------------------------- 
// Handle a down button press. This should only be called when we are
// in edit mode. It decrements the digit currently being edited by 1.
void LCDUnsignedItem::OnDown (void)
{
	// In error mode, any button clears the error
	if (IsInErrorMode)
	{
		IsInErrorMode = false;
		IsInEditMode = true;
		OnEditModeEntry();
	}
	else
	{
		// Decrement the current character with wrap around
		if (TheString[EditorIndex] == '0')
			TheString[EditorIndex] = '9';
		else
			TheString[EditorIndex]--;

		// Update the display
		SetCharacter (TheString[EditorIndex]);

		// Turns out you have to reset cursor after updating LCD
		//TheLCD->setCursor (EditorIndex+StringOffset, 1);
	}
}
		
// --------------------------------------------------------------------------- 
// Handle an Enter button press. This should only be called when we are
// in edit mode. Base class functionality is mostly fine here. We just
// have to turn off the cursor as well.
void LCDUnsignedItem::OnEnter (void)
{
	// Update the value from the editor string, skipping '['
	TheValue = strtoul (&TheString[1], NULL, 10);
	
	CursorOff();
	LCDItemBase::OnEnter();
}

