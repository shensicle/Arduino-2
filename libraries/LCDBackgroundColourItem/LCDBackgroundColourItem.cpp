/*
Copyright  2024 Scott Henwood/shensicle photographic. All Rights Reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

3. The name of the author may not be used to endorse or promote products 
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Scott Henwood/shensicle photographic "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <LCDBackgroundColourItem.h>

// --------------------------------------------------------------
// Called when edit mode has been terminated and the results of the edit
// have been determined to be valid. In this case, either start execution
// of the current program (run) or discontinue it (stop).
void LCDBackgroundColourItem::ApplyEdit (void)
{
	LCDListItem::ApplyEdit();

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
