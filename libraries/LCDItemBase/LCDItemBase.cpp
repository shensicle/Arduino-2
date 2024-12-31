#include <LCDItemBase.h>
	
#include <Arduino.h>

// Erase the specified text line by drawing a rectangle in the current background colour
void LCDItemBase::EraseTextLine (unsigned lineOffset)
{
	TheLCD->fillRect(LCD_TEXT_COLUMN, lineOffset, LCD_DISPLAY_WIDTH_PIXELS, LCD_CHAR_HEIGHT_PIXELS, TheApp->GetBackgroundColourHW()); //x orig, y orig, x size, y size
}

// --------------------------------------------------------------
// Called when edit mode has been terminated and the results of the edit
// have been determined to be valid. In most cases, this will be
// overriden in derived classes to take some action based on the results
// of the edit (like saving the new configuration). On its own, this
// method restores the non-edit display and sets IsInEditMode to false.
void LCDItemBase::ApplyEdit (void)
{

	IsInEditMode = false;
	DisplayCurrentValue();
}

// --------------------------------------------------------------
// Render the second line of the display right-justified. Leading 
// blanks are added as necessary. Parameter is a null-terminated 
// string.
// Returns the display column (starting at 0) of the first character in the string.
int LCDItemBase::RenderLineRightJustified (char* theString, unsigned short lineNum)
{
	// Storage for the output string. The "+1" gives us a place to store
	// the null terminator.
	char outputString[MAX_LCD_COLUMNS+1];
	
	// Counter
	int i; 
	
	// Avoids calculating the length of our input string more than once
	int theLength;
	
	// If the string is too long, truncate it to avoid a memory fault
	theLength = strlen (theString);
	if (theLength > MAX_LCD_COLUMNS)
	{
		theString[MAX_LCD_COLUMNS] = 0x00;
		theLength = MAX_LCD_COLUMNS;
	}
	
	// Start output string with the number of leading blanks we need
	for (i = 0; i < (MAX_LCD_COLUMNS - theLength); i++)
	{
		outputString[i] = ' ';
	}
	
	// And add the string passed into us
	strcpy (&outputString[i], theString);

    EraseTextLine (LCDRowOffsets[lineNum+2]);


	TheLCD->setCursor (LCD_TEXT_COLUMN, LCDRowOffsets[lineNum+2]); // Line 0 is top line of old LCD; line 3 is top line of new LCD. So lineNum of 1 maps onto 3 and linenum of 2 maps onto 4. But the array starts at 0
	TheLCD->setTextColor(TheApp->GetForegroundColourHW());
	TheLCD->print (outputString);
	
	return(i);
}


// --------------------------------------------------------------
// Render the specified line of the display center-justified. Leading 
// and trailing blanks are added as necessary. Parameter is a 
// null-terminated string.Line number defaults to 0 (ie. the first line).
// Returns the display column (starting at 0) of the first character in the string.
int LCDItemBase::RenderLineCenterJustified (char* theString, 
	   	                             unsigned short lineNum)
{
	// Storage for the output string. The "+1" gives us a place to store
	// the null terminator.
	char outputString[MAX_LCD_COLUMNS+1];
	
	// Counters
	int i, j, k;
	
	int returnValue;
	
	j = strlen(theString);

	// Figure out how many leading spaces we need
	i = (MAX_LCD_COLUMNS - j)/2;
	
	returnValue = i;
	
	for (k = 0; k < i; k++)
	{
		outputString[k] = ' ';
	}

	memcpy (&outputString[i], theString, j);	

	i += j;
	
	for (k = i; k < (MAX_LCD_COLUMNS); k++)
	{
		outputString[k] = ' ';
	}
	outputString[MAX_LCD_COLUMNS] = 0x00;

    EraseTextLine (LCDRowOffsets[lineNum+2]);

	TheLCD->setCursor (LCD_TEXT_COLUMN, LCDRowOffsets[lineNum+2]); // Line 0 is top line of old LCD; line 3 is top line of new LCD. So lineNum of 1 maps onto 3 and linenum of 2 maps onto 4. But the array starts at 0

	TheLCD->setTextColor(TheApp->GetForegroundColourHW());
	TheLCD->print (outputString);

	return (returnValue);
}

// --------------------------------------------------------------
// Constructor
LCDItemBase::LCDItemBase (shen_LCD*      theLCD,
		   				  char*  		 theLabel,
		     			  LCDApplication* theApp)
{
	
	IsInEditMode = IsInErrorMode = false;

	TheLCD = theLCD;
	TheApp = theApp;

	strncpy (TheLabel, theLabel, MAX_LCD_COLUMNS);
	TheLabel [MAX_LCD_COLUMNS] = 0x00;     // Just in case

}


// --------------------------------------------------------------
// Handle an Enter button press. This should only be called when we are
// in edit mode.
void LCDItemBase::OnEnter (void)
{
	// If we are in error mode, clear it and go back to edit mode
	
	if (IsInErrorMode == true)
	{
		IsInErrorMode = false;
		IsInEditMode = true;
		OnEditModeEntry();
	}
		
	// If the result of the edit is valid ...
	else 
	{
		if (IsValid())
		{
			// Go back to the non-editing state
			ApplyEdit();
		}
		else // bad edit - go to error mode
		{
			IsInEditMode = false;
			IsInErrorMode = true;
			DisplayError();
		}
	}
}


// --------------------------------------------------------------
// Handle a cancel button press. This should only be called when we are
// in edit mode.
void LCDItemBase::OnCancel (void)
{
	// If we are in error mode, clear it and go back to edit mode
	if (IsInErrorMode == true)
	{
		IsInErrorMode = false;
		IsInEditMode = true;
		OnEditModeEntry();
	}
	else
	{
		// Abort the current edit
		IsInEditMode = false;
		
		// And update the display accordingly
		DisplayCurrentValue();
	}
}

// --------------------------------------------------------------
// Called when this item becomes active on the display. This method
// displays the item title passed into the constructor on line 1 and
// Calls DisplayCurrentValue() to display the item's value on line 2.
void LCDItemBase::OnEntry (void)
{
	// Display the item title on line 1
	RenderLineCenterJustified (TheLabel);

	FetchData ();

	// Call derived classes' DisplayCurrentValue() to update line 2
	DisplayCurrentValue(); 

}

// --------------------------------------------------------------
// Called from the LCD controller when we have to go into edit mode. Change the
// display so that there is a visual indication that value is being edited.
void LCDItemBase::OnEditModeEntry (void)
{
	IsInEditMode = true;
	DisplayCurrentValue();
}


// --------------------------------------------------------------
// Called periodically by application to update the display (in case values
// have been changed by the terminal interface or other system components.
// In this default version, DisplayCurrentValue() is called as long as
// we are not in error mode or edit mode.
void LCDItemBase::Update (void)
{
	if (!IsInEditMode && !IsInErrorMode)
	{
		int isNewValue = FetchData();
        
		if (isNewValue)
			DisplayCurrentValue();
	}
}
