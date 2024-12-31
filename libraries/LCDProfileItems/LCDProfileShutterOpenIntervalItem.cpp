#include <LCDProfileShutterOpenIntervalItem.h>

// ---------------------------------------------------------------------------
// Called when edit mode has been terminated and the results of the edit
// have been determined to be valid. Adjust the profile we're representing
// accordingly.
void LCDProfileShutterOpenIntervalItem::ApplyEdit (void)
{
	LCDUnsignedItem::ApplyEdit();
	TheProfile->SetShutterOpenInterval (TheValue);
}

// ---------------------------------------------------------------------------
// Fetch data that this object will display on the second line and load
// object attributes accordingly. This method returns
// a flag which, when set, indicates that the data value has changed since
// the last time FetchData() was called. We do this because LCD updates 
// are fairly time-consuming and we only want to do them when necessary.
int LCDProfileShutterOpenIntervalItem::FetchData (void)
{
    int returnValue = 0;
    
    unsigned long oldValue = TheValue;
	TheValue = TheProfile->GetShutterOpenInterval();

    if (oldValue != TheValue)
        returnValue = 1;
    
    return (returnValue);
}

