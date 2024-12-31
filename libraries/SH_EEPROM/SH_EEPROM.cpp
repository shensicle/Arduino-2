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

