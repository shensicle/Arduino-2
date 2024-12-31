
#include <LCDCurrentProfileItem.h>

// --------------------------------------------------------------
// Called when edit mode has been terminated and the results of the edit
// have been determined to be valid. In most cases, this will be
// overriden in derived classes to take some action based on the results
// of the edit (like saving the new configuration). On its own, this
// method restores the non-edit display and sets IsInEditMode to false.
void LCDCurrentProfileItem::ApplyEdit (void)
{
	LCDListItem::ApplyEdit();
	TheApp->SetCurrentProfile(CurrentItem);
}


// --------------------------------------------------------------
// Fetch data that this object will display on the second line and load
// object attributes accordingly. This is intended to be called from the
// OnEntry() and Update() methods of derived classes. This method returns
// a flag which, when set, indicates that the data value has changed since
// the last time FetchData() was called. We do this because LCD updates 
// are fairly time-consuming and we only want to do them when necessary.
int LCDCurrentProfileItem::FetchData (void)
{
    int returnValue = 0;
    
    unsigned short oldItem = CurrentItem;
	CurrentItem = TheApp->GetCurrentProfile();
    
    if (oldItem != CurrentItem)
        returnValue = 1;
    
    return (returnValue);
}

