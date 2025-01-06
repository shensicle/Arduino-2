#ifndef _LCDITEMCOLLECTION_H
#define _LCDITEMCOLLECTION_H

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


// =======================================================================
class LCDItemCollection : public LCDItemBase
{
protected:
	
	// Pointer to an array containing the collection of related LCD items
	LCDItemBase** TheCollection;
	
	// Number of items in the collection
	unsigned short ItemsInCollection;
	
	// A flag which, when set, indicates that we should display ourself
	// in rotation as well as the members of our collection. This value
	// would be set if we are not the top level collection as it will allow
	// access to other members of the top level via the left and right
	// buttons.
	bool DisplayOurself;
	
	// Index of our currently active item. Set to -1 if no item is active
	int ActiveItem;

	// Returns a flag which, when set, indicates that the results of an edit
	// are valid.
	virtual bool IsValid (void) {return (true);}
	
	// Called when edit mode has been terminated and the results of the edit
	// have been determined to be valid. In most cases, this will be
	// overriden in derived classes to take some action based on the results
	// of the edit (like saving the new configuration). On its own, this
	// method restores the non-edit display and sets IsInEditMode to false.
	virtual void ApplyEdit (void);
	
	// Display the current value of this LCD item on the second line of
	// the display.
	virtual void DisplayCurrentValue (void);
	
	// Fetch data that this object will display on the second line and load
	// object attributes accordingly. For a collection, we don't need to do
	// anything.
	virtual int FetchData (void) { return (0); } 

	
public:
	// Constructor
	LCDItemCollection (shen_LCD*       theLCD,
	                   LCDApplication* theApp,
		               LCDItemBase**   theCollection,
		               unsigned short  itemsInCollection,
		               char*           theLabel, 
			   	       bool		       displayOurself = true);
	
	// Handle a left button press. This should only be called when we are
	// in edit mode.
	virtual void OnLeft (void);
	
	// Handle a right button press. This should only be called when we are
	// in edit mode.
	virtual void OnRight (void);
	
	// Handle an up button press. This should only be called when we are
	// in edit mode.
	virtual void OnUp (void);
	
	// Handle a down button press. This should only be called when we are
	// in edit mode.
	virtual void OnDown (void);
	
	// Handle an Enter button press. This should only be called when we are
	// in edit mode.
	virtual void OnEnter (void);
	
	// Handle a cancel button press. This should only be called when we are
	// in edit mode.
	virtual void OnCancel (void);
		
	// Must be defined in derived classes. Should change the display so that
	// there is a visual indication that value is being edited.
	virtual void OnEditModeEntry (void);
	
	// Called when this item becomes active on the display. This method
	// displays the item title passed into the constructor on line 1 and
	// Calls DisplayCurrentValue() to display the item's value on line 2.
	virtual void OnEntry (void);
	
	// Called periodically by application to update the display (in case values
	// have been changed by the terminal interface or other system components.
	// In this case, we call the update function of the active item, if there
	// is one.
	virtual void Update (void);
};

#endif

	