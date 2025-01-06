#ifndef _LCDTIMEITEM_H
#define _LCDTIMEITEM_H

/*
Copyright � 2024 Scott Henwood/shensicle photographic. All Rights Reserved.

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

#include <LCDUnsignedItem.h>

// ============================================================================
class LCDTimeItem : public LCDUnsignedItem
{
protected:
	
	// Display the current value of this LCD item on the second line of
	// the display.
	virtual void DisplayCurrentValue (void);
	
	// Method to take in a time value (in milliseconds) and return it in a
	// string in the format ssss.mm
	void TimeToString (char* theString, unsigned long theTime);
	
	// Method to take in a time string in the form ssss.mm and return it
	// as an unsigned long value in milliseconds.
	void StringToTime (char* theString, unsigned long* theTime);
	
public:
	// Constructor
	LCDTimeItem (shen_LCD*            theLCD, 
		         char *               theLabel, 
		         LCDApplication*      theApp,
		         unsigned long        minValue, 
		         unsigned long        maxValue,
		         unsigned long        charsSupported)
	: LCDUnsignedItem (theLCD, theLabel, theApp, minValue, maxValue, charsSupported) {};
	
	// Handle a left button press. This should only be called when we are
	// in edit mode. This button moves to the left (with wrap-around) so
	// we can change the previous digit with the up/down keys.
	virtual void OnLeft (void);
	
	// Handle a right button press. This should only be called when we are
	// in edit mode. This button moves to the right (with wrap-around) so
	// we can change the next digit with the up/down keys.
	virtual void OnRight (void);
	
	// Handle an Enter button press. This should only be called when we are
	// in edit mode.
	virtual void OnEnter (void);
};

#endif

