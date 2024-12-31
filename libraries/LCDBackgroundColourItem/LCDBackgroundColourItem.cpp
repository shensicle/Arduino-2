
#include <LCDBackgroundColourItem.h>

// --------------------------------------------------------------
// Called when edit mode has been terminated and the results of the edit
// have been determined to be valid. In this case, either start execution
// of the current program (run) or discontinue it (stop).
void LCDBackgroundColourItem::ApplyEdit (void)
{
	LCDListItem::ApplyEdit();
	
	//TheLCD->setfillscreen (LCDColourSettings[CurrentItem]);   // reboot required to make this take effect @@@
	TheApp->SetBackgroundColour(CurrentItem);

}

	
// --------------------------------------------------------------
// Fetch data that this object will display on the second line and load
// object attributes accordingly. This is intended to be called from the
// OnEntry() and Update() methods of derived classes. This method returns
// a flag which, when set, indicates that the data value has changed since
// the last time FetchData() was called. We do this because LCD updates 
// are fairly time-consuming and we only want to do them when necessary.

int LCDBackgroundColourItem::FetchData (void)
{
    int returnValue = 0;

    unsigned short oldItem = CurrentItem;
    CurrentItem = TheApp->GetBackgroundColour();
	
   
    if (oldItem != CurrentItem)
        returnValue = 1;
    
    return (returnValue);
}

// --------------------------------------------------------------
// Returns a flag which, when set, indicates that the results of an edit
// are valid. In this case, edit is not valid if new background colour is the
// same as the current foreground colour.
 bool LCDBackgroundColourItem::IsValid (void) 
 { 
 	bool returnValue = true;
 	
 	short foregroundColour = TheApp->GetForegroundColour();
 	
 	if (CurrentItem == foregroundColour)
 			returnValue = false;
 	
 	return (returnValue); 
 }
