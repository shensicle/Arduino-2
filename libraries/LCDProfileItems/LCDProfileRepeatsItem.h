#ifndef _LCDPROFILEREPEATSITEM_H
#define _LCDPROFILEREPEATSITEM_H

#include <LCDUnsignedItem.h>
#include <TImedProfile.h>
#include <LCDApp.h>

class LCDProfileRepeatsItem : public LCDUnsignedItem
{
protected:
	
	// Pointer to the profile we're managing repeats for
	TimedProfile* TheProfile;

	// Called when edit mode has been terminated and the results of the edit
	// have been determined to be valid. Update our profile's repeat counter.
	virtual void ApplyEdit (void);
	
	// Fetch data that this object will display on the second line and load
	// object attributes accordingly. This method returns
	// a flag which, when set, indicates that the data value has changed since
	// the last time FetchData() was called. We do this because LCD updates 
	// are fairly time-consuming and we only want to do them when necessary.
	virtual int FetchData (void);
		
public:
	// Constructor
	LCDProfileRepeatsItem (shen_LCD*  theLCD,
		               char*          theLabel,
		               TimedProfile*  theProfile,
		               LCDApplication* theApp)
	: LCDUnsignedItem  (theLCD,
		            theLabel,
                    theApp,
		            1,    // min value 
		            1000, // max value
		            4)    // characters
	{ TheProfile = theProfile; }	
	
};

#endif

