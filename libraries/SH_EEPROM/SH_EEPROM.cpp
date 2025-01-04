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

#include <SH_EEPROM.h>

// ---------------------------------------------------
// Erase EEPROM - set all locations to 0xff
void SH_EEPROM::erase (void)
{
	byte checksum = 0;
	
	for (int i = 0; i < SH_EEPROM_SIZE; i++)
	{
		EEPROM.write (i, 0xff);
		checksum += 0xff;
	}

	// First, calculate the proper checksum	
	checksum = 0xff - checksum;

	// And then write an invalid checksum so that we know EEPROM is not valid
	checksum --;
	
	saveChecksum (checksum);	
}

// ---------------------------------------------------
// Read num bytes from EEPROM starting at offset.
// If this would read off the end of EEPROM, return an error. Otherwise
// return the data in the supplied buffer.
bool SH_EEPROM::read (unsigned short num, unsigned short offset, char* buf)
{
	bool returnValue = false;
	if ((offset + num) < SH_EEPROM_SIZE)
	{
		for (int i = offset; i < (num+offset); i++)
		{
			buf[i] = EEPROM.read (i);
		}
		returnValue = true;
	}
	return (returnValue);
}
	
// ---------------------------------------------------
// Write num bytes to EEROM starting at offset. If this would write
// off the end of the EEPROM, return an error. Otherwise, write the
// supplied data and update the EEPROM checksum.
bool SH_EEPROM::write (unsigned short num, unsigned short offset, char* buf)
{
	bool returnValue = false;
	if ((offset + num) < SH_EEPROM_SIZE)
	{
		for (int i = offset; i < (num+offset); i++)
		{
			EEPROM.write (i, buf[i]);
		}
		writeChecksum();   // May need commits here and after erase @@@ - see https://blog.hirnschall.net/esp8266-eeprom/ - can also use get() and put() instead of read()/write()
		returnValue = true;
	}
	return (returnValue);
}

// ---------------------------------------------------
// Validate the EEPROM checksum, returning TRUE if it's okay and
// FALSE otherwise.
bool validateChecksum (void)
{
	bool returnValue = false;
	
	byte checksum = 0;
	
	for (int i = 0; i <= SH_EEPROM_SIZE; i++)
	{
		checksum += EEPROM.read (i);
	}
	
	// If the checksum is 0xff, it is valid
	if (checksum == 0xff)
	{
		returnValue = true;
	}	
	
	return (returnValue);
	
}
	
// ---------------------------------------------------
// Calculate the checksum of EEPROM and store it in the last location.
// Checksum is designed so adding up all values in EEPROM should result
// in a value of 0xff.
void SH_EEPROM::writeChecksum (void)
{
	byte checksum = 0;
	
	for (int i = 0; i < SH_EEPROM_SIZE; i++)
	{
		checksum += EEPROM.read (i);
	}

	// Then, calculate the proper checksum	
	checksum = 0xff - checksum;
	
	// And save it
	saveChecksum (checksum);
}
	
// ---------------------------------------------------
// Write the supplied checksum to the checksum location
void SH_EEPROM::saveChecksum (byte checksum)
{
	EEPROM.write (SH_EEPROM_SIZE, checksum);
}

