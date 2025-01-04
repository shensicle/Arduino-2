#ifndef _ANALOGKEYPAD_H
#define _ANALOGKEYPAD_H

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


#include <KeypadBase.h>

class AnalogKeypad : public KeypadBase
{
  protected: 
  
    // Max value returned on the analog input for each key. Must be determined
    // experimentally.
    const unsigned short KEYPAD_RIGHT_MAX   =  50;
	const unsigned short KEYPAD_DOWN_MAX    = 570;
	const unsigned short KEYPAD_LEFT_MAX    = 750;
	const unsigned short KEYPAD_UP_MAX      = 840;
	const unsigned short KEYPAD_SELECT_MAX  = 900;
	const unsigned short KEYPAD_OTHER_MAX   = 950;
	
	// Storage for the last value read from the analog input, providing basic debouncing
	char LastRead;

  public:
    AnalogKeypad (void) { LastRead = KEYPAD_NOKEY; }
  	
	// Return the current keypress, if any. Otherwise return KEYPAD_NOKEY
	virtual char GetKey (void);
	
};

#endif

