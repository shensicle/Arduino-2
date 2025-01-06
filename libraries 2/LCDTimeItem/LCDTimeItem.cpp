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

#include <LCDTimeItem.h>
#include <Arduino.h>	

// Some formatting defines used to display the time on the LCD in the correct
// resolution/precision. These could be constants, but given the limited 
// resources of the Arduino board, better to let the compiler do the work.
#define WHOLE_SECONDS_DIGITS 5
#define FRACTIONAL_SECONDS_DIGITS 2


// --------------------------------------------------------------------------- 
// Method to take in a time value (in milliseconds) and return it in a
// string in the format ssss.mm
void LCDTimeItem::TimeToString (char* theString, unsigned long theTime)
{
	unsigned long wholeSeconds = theTime / 1000;
	unsigned long fractionalSeconds = (theTime % 1000);
	
	unsigned long tempWhole = wholeSeconds / 10000;  // get first digit

        snprintf (theString, 
        	      2, // +1 for the null terminator 
        	      "%01u",
        	      tempWhole);
        tempWhole = wholeSeconds - (tempWhole*10000);
        
	// If you change the #define for WHOLE_SECONDS_DIGITS, make sure to change
	// the format specifier (originally "%05d") below. Unfortunately the
	// Arduino library does not support the "%0*d" format specifier for 
	// snprintf(), so the define cannot be included directly.
        snprintf (&theString[1], 
        	      WHOLE_SECONDS_DIGITS,  
        	      "%04u",
        	      tempWhole);
    
	// If you change the #define for FRACTIONAL_SECONDS_DIGITS, make sure to change
	// the format specifier (originally "%02d") below. Unfortunately the
	// Arduino library does not support the "%0*d" format specifier for 
	// snprintf(), so the define cannot be included directly.
        snprintf (&theString[WHOLE_SECONDS_DIGITS], 
    	          FRACTIONAL_SECONDS_DIGITS+2,  // +1 for the null terminator and +1 for the "."
    	          ".%02u", 
    	          fractionalSeconds);
}
	

// --------------------------------------------------------------------------- 
// Method to take in a time string in the form ssss.mm and return it
// as an unsigned long value in milliseconds.
void LCDTimeItem::StringToTime (char* theString, unsigned long* theTime)
{
	unsigned long wholeSeconds;
	unsigned long fractionalSeconds;
	
	int index = 1;  // Skip '['
	
	// This will stop at the first non-integer character - in this case,
	// the '.'
	wholeSeconds = strtoul (&theString[index], NULL, 10);
	
	// Move to the character following the '.'
	while (theString[index] != '.')
		index++;
	index++;
	
	fractionalSeconds = strtoul (&theString[index], NULL, 10);
	
	*theTime = (wholeSeconds * 1000) + (fractionalSeconds*10);
}


// --------------------------------------------------------------------------- 
// Display the current value of this LCD item on the second line of
// the display.
void LCDTimeItem::DisplayCurrentValue (void)
{
	// String index
	int index = 0;
	
	// If we are in edit mode, display the value with square brackets around
	// it; otherwise display normally.
	if (IsInEditMode)
		TheString[index++] = '[';
	
	// Next, convert our value to a string for display.
	TimeToString (&TheString[index], TheValue);
	
	// Just in case ...
	TheString[NumColumns] = 0x00;
	
	// Make sure we haven't gone too long
	index = strlen (TheString);
	if (index >= (NumColumns - 1))
		index = NumColumns - 1;
	
	if (IsInEditMode == true)
	{
		TheString[index++] = ']';
	}
	else // Not in edit mode
	{
		if (index >= (NumColumns))
			index = NumColumns;
	}
	
	TheString[index] = 0x00;
	
	StringOffset = RenderLineRightJustified (TheString);
}
	
	
// --------------------------------------------------------------------------- 
// Handle a left button press. This should only be called when we are
// in edit mode. This button moves to the left (with wrap-around) so
// we can change the previous digit with the up/down keys.
void LCDTimeItem::OnLeft (void)
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
		// Skip over the "."
		else if (TheString[EditorIndex] == '.')
		{
			EditorIndex --;
		}
		
		CursorOn();
	}
}
	
// --------------------------------------------------------------------------- 
// Handle a right button press. This should only be called when we are
// in edit mode. This button moves to the right (with wrap-around) so
// we can change the next digit with the up/down keys.
void LCDTimeItem::OnRight (void)
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
		
		// Skip over the "."
		else if (TheString[EditorIndex] == '.')
		{
			EditorIndex ++;
		}
		
		CursorOn();
	}
}

// --------------------------------------------------------------------------- 
// Handle an Enter button press. This should only be called when we are
// in edit mode.
void LCDTimeItem::OnEnter (void)
{
	// Update the value from the editor string.
	StringToTime (TheString, &TheValue);

	LCDItemBase::OnEnter();
}

