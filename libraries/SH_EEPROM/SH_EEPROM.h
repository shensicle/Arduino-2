#ifndef _SH_EEPROM_H
#define _SH_EEPROM_H

#include <arduino.h>
#include <EEPROM.h>

// The size of EEPROM available for user data
#define SH_EEPROM_SIZE 511

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
