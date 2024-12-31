#include <LCDProfileStartupDelayItem.h>

// ---------------------------------------------------------------------------
// Called when edit mode has been terminated and the results of the edit
// have been determined to be valid. Adjust the profile we're representing
// accordingly.
void LCDProfileStartupDelayItem::ApplyEdit (void)
{
	LCDTimeItem::ApplyEdit();
	TheProfile->SetStartupDelay (TheValue);
}

// ---------------------------------------------------------------------------
// Fetch data that this object will display on the second line and load
// object attributes accordingly.
int LCDProfileStartupDelayItem::FetchData (void)
{
    int returnValue = 0;
    
    int oldValue = TheValue;
	TheValue = TheProfile->GetStartupDelay();
    
    if (oldValue != TheValue)
        returnValue = 1;
    
    return (returnValue);
}



