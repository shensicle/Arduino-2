#ifndef _LIDAR_CONFIGDEFS_H
#define _LIDAR_CONFIGDEFS_H

/*
Copyright 20125 Scott Henwood All Rights Reserved.
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


// Structure to save the device configuration in non-volatile memory
typedef struct
{
      unsigned short  ObjectDetectionThreshold; // How close object is in metres to set off alarrm (mm)
      unsigned short  PollingIntervalMSecs;     // How often to poll the lidar (msec)
      unsigned short  BeepDurationMSecs;        // How long is one beep (msec)
      unsigned short  BackoffThreshold; // If distance is within the detection threshold but increases
                                        // between polls, assume a second object as long as the distance
                                        // exceeds this value - filters out sensor noise (mm)
      unsigned char   Spare[11];         
      unsigned char   checksum;
      
} lidar_config_t;


// Default values for configuration items
#define DEFAULT_OBJECT_DETECT_THRESHOLD (unsigned short)10000
#define DEFAULT_POLLING_INTERVAL_MSECS  (unsigned short)500
#define DEFAULT_BEEP_DURATION_MSECS     (unsigned short)100
#define DEFAULT_BACKOFF_THRESHOLD       (unsigned short)2000      


// And limits
 #define MIN_OBJECT_DETECT_THRESHOLD (unsigned short)200    // Experimentally derived
 #define MAX_OBJECT_DETECT_THRESHOLD (unsigned short)30000  // Theoretical device limit
 
 #define MIN_POLLING_INTERVAL_MSECS (unsigned short)100     // 100 msec
 #define MAX_POLLING_INTERVAL_MSECS (unsigned short)30000   // 30 seconds 
 
 #define MIN_BEEP_DURATION_MSECS (unsigned short)50    
 #define MAX_BEEP_DURATION_MSECS (unsigned short)5000       // 5 seconds   
 
#endif  
