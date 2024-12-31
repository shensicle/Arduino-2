#include <LCDListItem.h>

#include <Arduino.h>

	
// --------------------------------------------------------------------------- 
// Called when edit mode has been terminated and the results of the edit
// have been determined to be valid. In most cases, this will be
// overriden in derived classes to take some action based on the results
// of the edit (like saving the new configuration). On its own, this
// method restores the non-edit display and sets IsInEditMode to false.
void LCDListItem::ApplyEdit (void)
{
	// Call base class. Derived classes should to more
	LCDItemBase::ApplyEdit ();
}
	
// --------------------------------------------------------------------------- 
// Display the current value of this LCD item on the second line of
// the display.
void LCDListItem::DisplayCurrentValue (void)
{
	// Temporary storage for our string. The "+1" provides space for the
	// null terminator.
	char outputString[MAX_LCD_COLUMNS+1];
	
	// String index
	int index = 0;

	// If we are in edit mode, display the value with square brackets around
	// it; otherwise display normally.
	if (IsInEditMode)
		outputString[index++] = '[';
	
	// Next, copy in the string corresponding to the current item
	strncpy (&outputString[index], TheList[CurrentItem], MAX_LCD_COLUMNS-index);

	// Just in case ...
	outputString[MAX_LCD_COLUMNS] = 0x00;
	
	// Make sure we haven't gone too long
	index = strlen (outputString);

	if ((IsInEditMode == true) && (index >= (MAX_LCD_COLUMNS - 1)))
	{
		index = MAX_LCD_COLUMNS - 1;
		outputString[index++] = ']';

	}
	else if (IsInEditMode == true)
	{
		outputString[index++] = ']';

	}
	else if ((IsInEditMode == false) &&(index >= (MAX_LCD_COLUMNS)))
		index = MAX_LCD_COLUMNS;

	outputString[index] = 0x00;

	RenderLineRightJustified (outputString);

}

// --------------------------------------------------------------------------- 
// Constructor
LCDListItem::LCDListItem (shen_LCD* theLCD, char* theLabel, LCDApplication* theApp, char** theList, unsigned short numItems) :
   LCDItemBase (theLCD, theLabel, theApp)
{
	TheList = theList;
	CurrentItem = 0;
	NumItems = numItems;
}
	
// --------------------------------------------------------------------------- 
// Handle an up button press. This should only be called when we are
// in edit mode and it moves to the next value in our list.
void LCDListItem::OnUp (void)
{
	if (++CurrentItem >= NumItems)
		CurrentItem = 0;
	DisplayCurrentValue();
}
	
// --------------------------------------------------------------------------- 
// Handle a down button press. This should only be called when we are
// in edit mode and it moves to the previous value in our list
void LCDListItem::OnDown (void)
{
	if (CurrentItem == 0)
		CurrentItem = NumItems - 1;
	else
		CurrentItem --;

	DisplayCurrentValue();	
}
		

