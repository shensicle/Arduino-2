#ifndef _LIDAR_H
#define _LIDAR_H

/*
Copyright  2025 Scott Henwood/shensicle photographic. All Rights Reserved.

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


// For i2c
#include "Wire.h"


// the manual is wrong - it's not 0x20
#define LIDAR_DEFAULT_ADDRESS 0x10  

class Lidar
{
private:
	 // The i2c address of the Lidar module
	 unsigned char TheAddress;
	 
	 // How often the UpdateDistance() method is called to read distance from the Lidar
	 unsigned short CycleTimeMillisec;
	
	// A flag which, when set, indicates that the Lidar module is responding to commands
	bool IsRunning;
	
	// The last distance read from the Lidar
	unsigned short CurrentDistance;
	
	// Number of times in a row communications with the Lidar module has failed. Used to signify
	// the need for a Lidar reset
	unsigned char ErrorCount;

protected:
	
	// Write to a Lidar module register over I2C
	void WriteRegister (unsigned char theRegister, unsigned short theValue);
	
	// Configure the Lidar and start it running
	void StartLidar (void);
	
		
public:
    // Constructor
	Lidar (unsigned char  i2CAddress = LIDAR_DEFAULT_ADDRESS,
	       unsigned short  cycleTimeMillisec = 200);
	       
	// Read the distance from Lidar and save in CurrentDistance. This function must be called
	// periodically, ideally at the cycle time passed into the constructor.
     void UpdateDistance (void);
	
	// Return the current distance measurement. Parameter is set to the distance in cm
	// or 0 if the measurement is invalid.
	void GetDistance (unsigned short& theDistance);
	
};
#endif

