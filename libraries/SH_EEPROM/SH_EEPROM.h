#ifndef _SH_EEPROM_H
#define _SH_EEPROM_H

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

#include <arduino.h>
#include <EEPROM.h>

// The size of EEPROM available for user data
#define SH_EEPROM_SIZE 511

// =======================================================================
class SH_EEPROM
{
public:
	// Erase EEPROM - set all locations to 0xff
	void erase (void);
	
	// Read num bytes from EEPROM starting at offset.
	// If this would read off the end of EEPROM, return an error. Otherwise
	// return the data in the supplied buffer.
	bool read (unsigned short num, unsigned short offset, char* buf);
	
	// Write num bytes to EEROM starting at offset. If this would write
	// off the end of the EEPROM, return an error. Otherwise, write the
	// supplied data and update the EEPROM checksum.
	bool write (unsigned short num, unsigned short offset, char* buf);
	
	// Validate the EEPROM checksum, returning TRUE if it's okay and
	// FALSE otherwise.
	bool validateChecksum (void);
	
	// Constructor - need this to initialize the EEPROM object to correct size in ESP8266 as it uses flash emulation
	SH_EEPROM (void) { EEPROM.begin (SH_EEPROM_SIZE+1); }  // plus 1 is for the checksum
	
protected:
	// Calculate the checksum of EEPROM and store it in the last location.
	// Checksum is designed so adding up all values in EEPROM should result
	// in a value of 0xff.
	void writeChecksum (void);
	
	// Write the supplied checksum to the checksum location
	void saveChecksum (byte checksum);

};

#endif
