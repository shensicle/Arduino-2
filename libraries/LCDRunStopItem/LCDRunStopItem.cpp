
#include <LCDRunStopItem.h>

const char* LCDRunStopItem::Labels[] = {"Run", "Stop"};

// --------------------------------------------------------------------------- 
// Called when edit mode has been terminated and the results of the edit
// have been determined to be valid. In this case, either start execution
// of the current program (run) or discontinue it (stop).
void LCDRunStopItem::ApplyEdit (void)
{
	LCDListItem::ApplyEdit();

	if (CurrentItem == RunIndex)
	{
		TheApp->Run();
	}
	else  // must be stop
	{
		TheApp->Stop();
	}
}

	
// --------------------------------------------------------------------------- 
// Fetch data that this object will display on the second line and load
// object attributes accordingly. This is intended to be called from the
// OnEntry() and Update() methods of derived classes. This method returns
// a flag which, when set, indicates that the data value has changed since
// the last time FetchData() was called. We do this because LCD updates 
// are fairly time-consuming and we only want to do them when necessary.
int LCDRunStopItem::FetchData (void)
{
    int returnValue = 0;
    
    unsigned short oldItem = CurrentItem;
    
	if (TheApp->ProfileIsRunning())
		CurrentItem = RunIndex;
	else
		CurrentItem = StopIndex;

    if (oldItem != CurrentItem)
        returnValue = 1;
    
    return (returnValue);
}


