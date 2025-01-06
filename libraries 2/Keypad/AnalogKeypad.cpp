#include <Arduino.h>
#include <AnalogKeypad.h>

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
  	
// --------------------------------------------------------
// Return the current keypress, if any. Otherwise return KEYPAD_NOKEY
char AnalogKeypad::GetKey (void)
 {
   char newRead = KEYPAD_NOKEY;

   // On an ESP826, A0 is the only analog input
   unsigned short analogValue = analogRead(A0);

   if (analogValue < KEYPAD_RIGHT_MAX)
   { 	
      // Serial.printf ("Right %d\n", analogValue);
      newRead = KEYPAD_RIGHT;
   }
   else if (analogValue < KEYPAD_DOWN_MAX)
   {
      // Serial.printf ("Down %d\n", analogValue);
      newRead = KEYPAD_DOWN;
   }
   else if (analogValue < KEYPAD_LEFT_MAX)
   {
     // Serial.printf ("Left %d\n", analogValue);
     newRead = KEYPAD_LEFT;
   }
   else if (analogValue < KEYPAD_UP_MAX)
   {
      // Serial.printf ("Up %d\n", analogValue);
      newRead = KEYPAD_UP;
   }
   else if (analogValue < KEYPAD_SELECT_MAX)
   {
      // Serial.printf ("Select %d\n", analogValue);
      newRead = KEYPAD_SELECT;
   }
   else if (analogValue < KEYPAD_OTHER_MAX)
   {
      // Serial.printf ("Other %d\n", analogValue);
      newRead = KEYPAD_OTHER;
   }
   else
   {
     // if (analogValue != 1024) Serial.printf("Null %d\n", analogValue);
   }
   
   // Make sure holding down a key results in only one event. If same key(s) as
   // last time are still active, return 0, which is KEYPAD_NOKEY.
   char returnValue = newRead & ~LastRead;
   LastRead = newRead;

   return (returnValue);
 }
	
