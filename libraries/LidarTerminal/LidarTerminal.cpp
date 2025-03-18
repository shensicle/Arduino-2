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

#include <LidarTerminal.h>
#include <LidarConfig.h>


// Size of buffer for incoming serial characters
#define BUF_SIZE 80

	
// --------------------------------------------------------------------------- 
// Constructor
LidarTerminal::LidarTerminal (LidarConfigClass* theConfig, char* firmwareVersion)
	: Parser(&InputString) 
{ 
	InputString = ""; 
	TheConfig = theConfig; 
	CommandComplete = false; 
	
	// Reserve space for incoming commands
    InputString.reserve(BUF_SIZE);
    
    // Save the firmware version, just so we can report it
    FirmwareVersion = firmwareVersion;

}
	
// --------------------------------------------------------------------------- 
// Add a character to the command currently being built up
void LidarTerminal::Add (char inChar)
{
	InputString += inChar;
	
	if (inChar == '\n')
		CommandComplete = true;
}
	
// --------------------------------------------------------------------------- 
// If we have a complete command, parse and act on it
void LidarTerminal::Update (void)
{
	
	bool status = true;
	unsigned short newValue;
	
	if (CommandComplete == true)
	{
		    Parser.Reset();
		
		    // Return the first character from the terminal string, which
		    // is a single character command
		    char command = Parser.GetChar();
		    command = toupper(command);
		    
		    switch (command)
		    {
		    	    // Set the object detection threshold in mm
		    		case 'T':
		    			newValue = (unsigned short) Parser.GetUnsignedLong();
		    			status = TheConfig->SetObjectDetectionThreshold ( newValue );
		    			break;
        
        			// Set the lidar polling interval in msec
 		    		case 'P':
		    			newValue = (unsigned short) Parser.GetUnsignedLong();
		    			status = TheConfig->SetPollingInterval ( newValue );
		    			break;
        
                    // Set the beep duration in msec
 		    		case 'D':
		    			newValue = (unsigned short) Parser.GetUnsignedLong();
		    			status = TheConfig->SetBeepDuration ( newValue );
		    			break;
        
                    // Set the backoff threshold in mm
		    		case 'B':
		    			newValue = (unsigned short) Parser.GetUnsignedLong();
		    			status = TheConfig->SetBackoffThreshold ( newValue );
		    			break;
        
      				// Get and display current configuration
      				case 'G':
      						// First, print the system configuration
	                        Serial.print ("\nBike Lidar:\nFirmware version: "); 
	                        Serial.printf ("%s\n\n", FirmwareVersion); 
          
                            Serial.print ("Object detection threshold: ");
                            Serial.printf ("%u mm\n", TheConfig->GetObjectDetectionThreshold());
                            Serial.print ("Polling Interval: ");
                            Serial.printf ("%u msec\n", TheConfig->GetPollingInterval());
                            Serial.print ("Beep Duration: ");
                            Serial.printf ("%u msec\n", TheConfig->GetBeepDuration());
                            Serial.print ("BackoffThreshold: ");
                            Serial.printf ("%u mm\n\n", TheConfig->GetBackoffThreshold());

      					break;
        
      				case 'H':
      					Serial.println ("Commands:");
      					Serial.printf ("\nB <backoff threshold>\n");
      					Serial.printf ("\tSet the backoff threshold in mm (%u to %u)\n", 
      					                MIN_OBJECT_DETECT_THRESHOLD,
      					                MAX_OBJECT_DETECT_THRESHOLD);
      					Serial.printf ("\nD <beep duration>\n");
      					Serial.printf ("\tSet the beep duration in msec, (%u to %u)\n",
      					                MIN_BEEP_DURATION_MSECS, MAX_BEEP_DURATION_MSECS);
      					Serial.printf ("\nP <polling interval>\n");
      					Serial.printf ("\tSet the polling interval in msec, (%u to %u)\n",
      					                MIN_POLLING_INTERVAL_MSECS, MAX_POLLING_INTERVAL_MSECS);
      					Serial.printf ("\nT <detection threshold>\n");
      					Serial.printf ("\tSet the detection threshold in mm (%u to %u))\n",
      									MIN_OBJECT_DETECT_THRESHOLD,
      					                MAX_OBJECT_DETECT_THRESHOLD);

      					Serial.printf ("\nG - display the current configuration\n");
      					Serial.printf ("\nH - display this message\n\n");
      					break;
        
      				default:
      					status = false;
      					break;

      		    }
    
      		    CommandComplete = false;
      		    InputString = ""; 
  
      		    // Turn status to a more human friendly value
      		    if (status == true)
      		    	    Serial.println (F("+OK"));
      		    else
      		    	    Serial.println (F("-ERR"));
      	}  
}

