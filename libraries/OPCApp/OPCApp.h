#ifndef _OPCAPP_H
#define _OPCAPP_H

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

#include <SD.h>
#include <BH1750FVI.h>
#include <Stepper.h>

#include "OPCAppDefs.h"
#include "OPCConfig.h"

// One-pixel camera application

class OPCApp
{
public:
	
	// Constructor - set everything up
	OPCApp (BH1750FVI* theLightMeter, camera_config_t* defaultConfiguration);
	
	// Set the camera resolution. Return value indicates success when set.
	bool SetCameraResLow(void);
	bool SetCameraResHi(void);
	
	// Set the camera configuration. If parameters are invalid, a false is returned
	// and current camera configuration is not modified.
	bool SetCameraConfig (camera_config_t* theConfig);
	
	// Get the current camera configuration
	void GetCameraConfig (camera_config_t* theConfig);
	
	
	// Kick off the picture-taking process. Once the picture starts, a task running
	// on the other core will manage the capture. If something goes wrong, false
	// is returned.
	bool StartNewCapture (void);
	
	// Abort a capture in progress
	void AbortCapture (void);
	
	// Returns a flag which, when set, indicates that there is currently a capture
	// in progress
	bool CaptureInProgress (void)
	{ return (CaptureUnderway); }		
	
	// For testing
	void DumpConfig (void);
	
	// Temporarily public for testing
	// Figure out the name (number) of the next file we can write to
	void LoadNextFileNumber (void);
	
	// Called repeatedly from sketch loop() to perform the capture
	void CaptureTask(void);
	
protected:
    
    // Pointer to the light meter object
    BH1750FVI*  TheLightMeter;
    
    // Object to manage the configuration
    OPCConfigClass ConfigurationManager;
    
    // Configuration object
    camera_config_t TheConfiguration;
    
	// A flag which, when set, indicates that a capture is currently underway
	bool CaptureUnderway;
	
	// The number of the next file we can write. This number is turned into a
	// string and used to name the file.
	unsigned long NextFileNumber;
	
	// Save the number of the next file we can write so that we can use it on
	// power-up. This number is turned into a string and used to name the file.
	void SaveNextFileNumber (void);

	// Write the header at the start of an image file. Returns a flag which,
	// when set, indicates that the write was successful.
	bool WriteImageFileHeader(void);
	
	// Move the sensor to the first pixel position of the image
	void MoveSensorHome (void);
	
	// Handles for the files we use - could use one handle, but this makes
	// code a little more clear
	File ImageFile, NameFile;
	
	// Return a pointer to the name of the next image file to save
	char* GetImageFileName (void);
	
	// Coordinates of the next pixel to capture
	unsigned short NextRow, NextColumn;
	
};

#endif
