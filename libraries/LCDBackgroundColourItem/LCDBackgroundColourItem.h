#ifndef _LCDBACKGROUNDCOLOURITEM_H
#define _LCDBACKGROUNDCOLOURITEM_H

#include <LCDListItem.h>
#include <AppBase.h>
#include <LCDApp.h>

// Labels for the various colours available along with their LCD colour codes. Defined in the .ino file
extern const char* LCDColourLabels[];
extern const short  LCDColourSettings[];


class LCDBackgroundColourItem : public LCDListItem
{
protected:
	// Pointer to the application
//	LCDApplication* TheApp;  // In base clase @@@
		
	// Called when edit mode has been terminated and the results of the edit
	// have been determined to be valid. In this case, either start execution
	// of the current program (run) or discontinue it (stop).
	virtual void ApplyEdit (void);
	
	// Fetch data that this object will display on the second line and load
	// object attributes accordingly. This method returns
	// a flag which, when set, indicates that the data value has changed since
	// the last time FetchData() was called. We do this because LCD updates 
	// are fairly time-consuming and we only want to do them when necessary.
	virtual int FetchData (void);
	
	// Returns a flag which, when set, indicates that the results of an edit
	// are valid. In this case, edit is not valid if new background colour is the
	// same as the current foreground colour.
	virtual bool IsValid (void);


	
public:
	// Constructor
	LCDBackgroundColourItem (shen_LCD*   	theLCD, 
		          		     char* 	theLabel, // Offset of label in LCDStringTable 
		          	LCDApplication* theApp)
	: LCDListItem (theLCD, theLabel, theApp, (char**)LCDColourLabels, 8)
		
	  { }		

};

#endif

