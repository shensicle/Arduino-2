#ifndef _LCDCURRENTPROFILEITEM_H
#define _LCDCURRENTPROFILEITEM_H

#include <LCDListItem.h>
#include <LCDApp.h>

static char* Labels[] = {"1", "2", "3"};

class LCDCurrentProfileItem : public LCDListItem
{
protected:
	
	// Pointer to the application
//	LCDApplication* TheApp;   moved to base class @@@
	
	// Called when edit mode has been terminated and the results of the edit
	// have been determined to be valid. In most cases, this will be
	// overriden in derived classes to take some action based on the results
	// of the edit (like saving the new configuration). On its own, this
	// method restores the non-edit display and sets IsInEditMode to false.
	virtual void ApplyEdit (void);
	
	// Fetch data that this object will display on the second line and load
	// object attributes accordingly. This method returns
	// a flag which, when set, indicates that the data value has changed since
	// the last time FetchData() was called. We do this because LCD updates 
	// are fairly time-consuming and we only want to do them when necessary.
	virtual int FetchData (void);

	
public:
	// Constructor
	LCDCurrentProfileItem (shen_LCD* theLCD, 
		               	char* theLabel, // Offset of label in LCDStringTable 
		               LCDApplication* theApp)
	: LCDListItem (theLCD, theLabel, theApp, Labels, 3) {}		

};

#endif

