#include <LCDItemCollection.h>

#include <Arduino.h>

#define NO_ACTIVE_ITEM -1
	
// --------------------------------------------------------------
// Called when edit mode has been terminated and the results of the edit
// have been determined to be valid.static unsigned char* ColumnsSupportedPtr;
void LCDItemCollection::LCDItemCollection::ApplyEdit (void)
{
}
	
// --------------------------------------------------------------
// Display the current value of this LCD item on the second line of
// the display.
void LCDItemCollection::DisplayCurrentValue (void)
{
	RenderLineRightJustified ("Enter for more");
}
	
// --------------------------------------------------------------
// Constructor
LCDItemCollection::LCDItemCollection (
	           LCDApplication* theApp,
		       LCDItemBase**  theCollection,
	           unsigned short itemsInCollection,
		       char*          theLabel, 
		       bool		      displayOurself)
: LCDItemBase (theLabel, theApp)
{
	TheCollection = theCollection;
	ItemsInCollection = itemsInCollection;
	DisplayOurself = displayOurself;

	if (displayOurself)
		ActiveItem = NO_ACTIVE_ITEM;
	else
		ActiveItem = 0;
}
	
// --------------------------------------------------------------
// Handle a left button press. This should only be called when we are
// in edit mode.
void LCDItemCollection::OnLeft (void)
{
	// If we have an item that is being edited, send the key there
	if (ActiveItem != NO_ACTIVE_ITEM)
	{
		if (TheCollection[ActiveItem]->IsBeingEdited())
		{
			TheCollection[ActiveItem]->OnLeft();
		}
		else
		{
			// If we're at the first item this would be the
			// time to display ourselves if that is enabled.
			if ((DisplayOurself == true) && (ActiveItem == 0))
			{
				ActiveItem = NO_ACTIVE_ITEM;
				IsInEditMode = false;
				DisplayCurrentValue();
			}
			else
			{
				// We have an active item that is not being
				// edited. In this case, the left key moves us
				// to the previous item and makes it active.
				ActiveItem --;
				if (ActiveItem < 0)
					ActiveItem = ItemsInCollection - 1;
			
				TheCollection[ActiveItem]->OnEntry();
			}
		}
	}
	
	// The else condition should never happen!
}
	
// --------------------------------------------------------------
// Handle a right button press. This should only be called when we are
// in edit mode.
void LCDItemCollection::OnRight (void)
{
	
	// If we have an item that is being edited, send the key there
	if (ActiveItem != NO_ACTIVE_ITEM)
	{
		if (TheCollection[ActiveItem]->IsBeingEdited())
		{
			TheCollection[ActiveItem]->OnRight();
		}
		else
		{
			// If we're at the last item this would be the
			// time to display ourselves if that is enabled.
			if ((DisplayOurself == true) && (ActiveItem ==  (ItemsInCollection-1)))
			{
				ActiveItem = NO_ACTIVE_ITEM;
				IsInEditMode = false;
				DisplayCurrentValue();
			}
			else
			{
				// We have an active item that is not being
				// edited. In this case, the right key moves us
				// to the next item and makes it active.
				ActiveItem ++;
				if (ActiveItem >= ItemsInCollection)
					ActiveItem = 0;
			
				TheCollection[ActiveItem]->OnEntry(); 
			}
		}
	}
	
	// The else condition should never happen!
	
}

// --------------------------------------------------------------
// Handle an up button press. This should only be called when we are
// in edit mode.
void LCDItemCollection::OnUp (void)
{
	// If we have an item that is being edited, send the key there
	if (ActiveItem != NO_ACTIVE_ITEM)
	{
		if (TheCollection[ActiveItem]->IsBeingEdited())
		{
			TheCollection[ActiveItem]->OnUp();
		}
	}

	// Otherwise, ignore the key
}
	
// --------------------------------------------------------------
// Handle a down button press. This should only be called when we are
// in edit mode.
void LCDItemCollection::OnDown (void)
{
	// If we have an item that is being edited, send the key there
	if (ActiveItem != NO_ACTIVE_ITEM)
	{
		if (TheCollection[ActiveItem]->IsBeingEdited())
		{
			TheCollection[ActiveItem]->OnDown();
		}
	}
	
	// Otherwise, ignore the key
}
	
// --------------------------------------------------------------
// Handle an Enter button press. This should only be called when we are
// in edit mode.
void LCDItemCollection::OnEnter (void)
{
	// It we have an active item, pass this key to it.	
	if (ActiveItem != NO_ACTIVE_ITEM)
	{
		if (TheCollection[ActiveItem]->IsBeingEdited())
		{
			TheCollection[ActiveItem]->OnEnter();
			
			// If our active item is no longer being edited, neither
			// are we.
			if (TheCollection[ActiveItem]->IsBeingEdited() == false)
				IsInEditMode = false;
		}
		else // Not currently being edited
		{
			TheCollection[ActiveItem]->OnEditModeEntry();
		}
	}
	
	// The else case should never happen.
}
	
// --------------------------------------------------------------
// Handle a cancel button press. This should only be called when we are
// in edit mode.
void LCDItemCollection::OnCancel (void)
{
	// It we have an active item that is being edited, pass this key to it.	
	if (ActiveItem != NO_ACTIVE_ITEM)
	{
		if (TheCollection[ActiveItem]->IsBeingEdited())
		{
			TheCollection[ActiveItem]->OnEnter();
		}
		else
		{
			// In this case, we have an active item that is not
			// being edited. If we're set to the mode where we
			// have to display ourself as part of the rotation, do so
			if (DisplayOurself)
			{
				ActiveItem = NO_ACTIVE_ITEM;
				IsInEditMode = false;
				DisplayCurrentValue();
			}	
		}			
	}

	// Otherwise, ignore the key
}
		

// --------------------------------------------------------------
// Must be defined in derived classes. Should change the display so that
// there is a visual indication that value is being edited.
void LCDItemCollection::OnEditModeEntry (void)
{
	
	// If there are any items in our collection, display the first one
	if (ItemsInCollection)
	{
		if (ActiveItem == NO_ACTIVE_ITEM)
		{
			ActiveItem = 0;
			TheCollection[ActiveItem]->OnEntry();
		}
		else
		{
			if (TheCollection[ActiveItem]->IsBeingEdited())
				TheCollection[ActiveItem]->OnEnter();
			else
				TheCollection[ActiveItem]->OnEditModeEntry();
		}
		IsInEditMode = true;
	}
}

// --------------------------------------------------------------
// Called when this item becomes active on the display. This method
// displays the item title passed into the constructor on line 1 and
// Calls DisplayCurrentValue() to display the item's value on line 2
void LCDItemCollection::OnEntry (void)
{
	if (ItemsInCollection)
	{
		ActiveItem = 0;
		TheCollection[ActiveItem]->OnEntry();
	}
}
	
// --------------------------------------------------------------
// Called periodically by application to update the display (in case values
// have been changed by the terminal interface or other system components.
// In this case, we call the update function of the active item, if there
// is one.
void LCDItemCollection::Update (void)
{
	if (ActiveItem != NO_ACTIVE_ITEM)
	{
		TheCollection[ActiveItem]->Update();
	}
}


