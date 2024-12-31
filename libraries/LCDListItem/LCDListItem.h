#ifndef _LCDLISTITEM_H
#define _LCDLISTITEM_H

#include <LCDItemBase.h>

class LCDListItem : public LCDItemBase
{
protected:

	// Pointer to an array of strings containing all of our possible
	// list items.
	char** TheList;
	
	// Current position within the list
	unsigned short CurrentItem;
	
	// The number of items on the list
	unsigned short NumItems;
	
	// Returns a flag which, when set, indicates that the results of an edit
	// are valid. Since we are processing a known list, our edits are always
	// valid.
	virtual bool IsValid (void) { return (true); }
	
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
	LCDListItem (shen_LCD*  theLCD, 
		     	char*  theLabel, // Offset of label in LCDStringTable 
		    	LCDApplication* theApp,
		     	char**         theList, 
		    	 unsigned short numItems);
	
	// Handle a left button press. This should only be called when we are
	// in edit mode. We don't use this key.
	virtual void OnLeft (void) {};
	
	// Handle a right button press. This should only be called when we are
	// in edit mode. We don't use this key.
	virtual void OnRight (void) {};
	
	// Handle an up button press. This should only be called when we are
	// in edit mode and it moves to the next value in our list.
	virtual void OnUp (void);
	
	// Handle a down button press. This should only be called when we are
	// in edit mode and it moves to the previous value in our list
	virtual void OnDown (void);
		
};

#endif

