#ifndef _LCDLISTITEM_H
#define _LCDLISTITEM_H

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
	LCDListItem (char*            theLabel,
		    	 LCDApplication*  theApp,
		     	 char**           theList, 
		    	 unsigned short   numItems);
	
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

