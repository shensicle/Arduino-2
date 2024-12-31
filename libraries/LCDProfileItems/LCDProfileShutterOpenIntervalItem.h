#ifndef _LCDPROFILESHUTTEROPENINTERVALITEM_H
#define _LCDPROFILESHUTTEROPENINTERVALITEM_H

#include <LCDTimeItem.h>
#include <TimedProfile.h>

class LCDProfileShutterOpenIntervalItem : public LCDTimeItem
{
protected:
	
	// Pointer to the profile we're managing startup delay for
	TimedProfile* TheProfile;

	// Called when edit mode has been terminated and the results of the edit
	// have been determined to be valid. Update our profile's shutter open interval.
	virtual void ApplyEdit (void);
	
	// Fetch data that this object will display on the second line and load
	// object attributes accordingly. This method returns
	// a flag which, when set, indicates that the data value has changed since
	// the last time FetchData() was called. We do this because LCD updates 
	// are fairly time-consuming and we only want to do them when necessary.
	virtual int FetchData (void);	

		
public:
	// Constructor
	LCDProfileShutterOpenIntervalItem  (shen_LCD*      theLCD,
		         		    char*  theLabel,
		         		    TimedProfile*  theProfile,
		         		    LCDApplication* theApp)
	: LCDTimeItem  (theLCD,
		        theLabel, 
		        theApp,
		        1,         // min value 
		        86400000,  // max value
		        11)        // characters
	{ TheProfile = theProfile; }	
};

#endif

