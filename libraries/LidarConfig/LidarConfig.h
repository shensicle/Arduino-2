#ifndef _LIDAR_CONFIG_H
#define _LIDAR_CONFIG_H

/*
Copyright 2025 Scott Henwood All Rights Reserved.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products 
   derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY CANARIE Inc. "AS IS" AND 
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


#include <LidarConfigDefs.h>
#include <Preferences.h>

// Class to manage the lidar configuration
class LidarConfigClass
{
    private:
        // The buffer that stores our configuration, as layed out in non-volative storage
        lidar_config_t TheConfiguration;
        		
        // Return the one's complement checksum of the configuration structure
        unsigned char CalculateChecksum (void);
        
        // Object to store configuration
        Preferences TheStorage;
		        
    protected:
    
        // Write configuration information to non-volatile storage, adding a checksum
        void Write (void);
		
        // Read configuration information from non-volatile storage and validate the checksum
        // Returns true if configuration is valid and false otherwise
        bool Read(void);	
	
  	public:
 	    
  	    // Constructor
  	    LidarConfigClass ();
				  	    
  	    // Change the object detect threshold. Change fails if
  	    // new value is out of range. 
  	    // Return true if new value is valid and false otherwise.
  	    bool SetObjectDetectionThreshold(unsigned short newThreshold);
  	    		
        // Change the polling interval, specified in milliseconds. 
        // Change fails if new value is out of range.
        // Return true if new value is valid and false otherwise.
        bool SetPollingInterval(unsigned short newInterval);
        
        // Determine how long a beep lasts for. Change fails if
  	    // new value is out of range.
  	    // Return true if new value is valid and false otherwise.
        bool SetBeepDuration (unsigned short newDuration);
  	    		
  	    // Set the backoff threshold. If an object is in range, but it's distance increases by
  	    // at least this value, assume a second ojbect and alert. Change fails if
  	    // new value is out of range.
  	    // Return true if new value is valid and false otherwise.
  	    bool SetBackoffThreshold (unsigned short newBackoff);		
  	    		
  	    // Return the object detect threshold
  	    unsigned short GetObjectDetectThreshold (void)
  	    	{ return (TheConfiguration.ObjectDetectionThreshold); }
  	    
  	    // Return the current object detect threshold in mm.
  	    unsigned short GetObjectDetectionThreshold(void)
  	    	{ return TheConfiguration.ObjectDetectionThreshold; }
  	    		
  	    // Return the current polling interval in milliseconds. 
  	    unsigned short GetPollingInterval(void)
  	    	{ return TheConfiguration.PollingIntervalMSecs; }
  	    		
   	    // Return the current beep duration in milliseconds. 
  	    unsigned short GetBeepDuration(void)
  	    	{ return TheConfiguration.BeepDurationMSecs; }
  	    		
  	    // Return the current backoff in mm
  	    unsigned short GetBackoffThreshold (void)
  	    {  return TheConfiguration.BackoffThreshold; }
  	    		
  	    // Load the configuration from non-volatile storage. This must be called after the object is
  	    // created but before any of the other methods can be used. If the non-volatile storage checksum
  	    // test fails, it is re-initialized to defaults.
  	    void Load(void);        
};



#endif