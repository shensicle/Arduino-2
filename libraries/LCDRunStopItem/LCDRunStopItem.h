#ifndef _LCDRUNSTOPITEM_H
#define _LCDRUNSTOPITEM_H

#include <LCDListItem.h>
#include <AppBase.h>


class LCDRunStopItem : public LCDListItem
{
protected:
	// Pointer to the application
//	ApplicationBase* TheApp;  // now in base class @@@
	
	static const char* Labels[];
	const int RunIndex;
	const int StopIndex;
	
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

	
public:
	// Constructor
	LCDRunStopItem (shen_LCD*     theLCD, 
		        	char* theLabel,
		            LCDApplication* theApp)
	: LCDListItem (theLCD, theLabel, theApp, (char**)Labels, 2), RunIndex(0), StopIndex(1) 
	  { }		
};

#endif

