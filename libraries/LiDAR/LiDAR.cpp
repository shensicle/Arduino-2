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

#include "LiDAR.h"
#include "arduino.h"

// ----------------------------------------------------------------------------
// Write to a Lidar module register over I2C
void Lidar::WriteRegister (unsigned char theRegister, unsigned short theValue)
{
  Wire.beginTransmission(TheAddress);
  Wire.write((unsigned char)(theRegister));
  Wire.write((unsigned char)(0));
  Wire.write((unsigned char)(theValue & 0xFF));
  Wire.write((unsigned char)(theValue >> 8));
  Wire.endTransmission();
}

// ----------------------------------------------------------------------------	
// Configure the Lidar and start it running
void Lidar::StartLidar (void)
{
    WriteRegister (0, 0);                    //电流寄存器设置 Set the current register to 0
    WriteRegister (61, 0x000);               //设置为主动读取结果 Set the mode to 0x0000 - means we have to actively read Lidar
    WriteRegister (66, CycleTimeMillisec);   //周期寄存器 Set the measurement cycle register

    WriteRegister (3, 0x0001);               // Start measuring
}
	
// ----------------------------------------------------------------------------	
// Constructor.
Lidar::Lidar (unsigned char  i2CAddress, unsigned short  cycleTimeMillisec)
{
	// Have to do this here so that it's possible to creae a static instantiation of this class (before
	// setup() is called.  
	Wire.begin();

	TheAddress = i2CAddress;
	CycleTimeMillisec = cycleTimeMillisec;
	IsRunning = false;
	
	ErrorCount = 2;
}
	       
// ----------------------------------------------------------------------------	
// Read the distance from Lidar and save in CurrentDistance. This function must be called
// periodically, ideally at the cycle time passed into the constructor.
void Lidar::UpdateDistance (void)
{  
  
  if (ErrorCount >= 2)
  {
  		StartLidar();
  		ErrorCount = 0;
  		Serial.println ("Lidar Error");
  }	
	
  //传输寄存器地址 Tell Lidar we want to read distance data
  Wire.beginTransmission(TheAddress);
  Wire.write((unsigned char)(23));
  Wire.write((unsigned char)(0));
  Wire.endTransmission();
  
  Wire.requestFrom(TheAddress, 2); // Request 2 bytes

  if (Wire.available() >= 2) 
  {
    unsigned char lsb = Wire.read();   // Read lower byte
    unsigned char msb = Wire.read();   // Read upper byte
    CurrentDistance = (msb << 8) | lsb;
    
//    Serial.print ("Raw distance: "); Serial.println(CurrentDistance);
  }
  else // No data available
  {
    ErrorCount ++;
    CurrentDistance = 0;
  }
}

/*	
// ----------------------------------------------------------------------------	
// Read the distance from Lidar and save in CurrentDistance. This function must be called
// periodically, ideally at the cycle time passed into the constructor.
void Lidar::UpdateDistance (void)
{  
  //传输寄存器地址 Tell Lidar we want to read distance data
  Wire.beginTransmission(TheAddress);
  Wire.write((unsigned char)(23));
  Wire.write((unsigned char)(0));
  Wire.endTransmission();
  
  Wire.requestFrom(TheAddress, 2); // Request 2 bytes

  if (Wire.available() >= 2) 
  {
    unsigned char lsb = Wire.read();   // Read lower byte
    unsigned char msb = Wire.read();   // Read upper byte
    CurrentDistance = (msb << 8) | lsb;
    
    Serial.print ("Raw distance: "); Serial.println(CurrentDistance);

    if(IsRunning == false)
    {
      IsRunning = true;
      ErrorCount = 0;
      StartLidar();
      Serial.println("connected");
    }
    else
    {
      //doKlmFilter(distance);
      if(CurrentDistance < 64000)
      { 
       Serial.print(CurrentDistance);
       Serial.println(" mm");
      }
      else
      {
        Serial.print("ErrorCode:");
        Serial.println(CurrentDistance);
      }
    }
  }
  else // No data available
  {
    if(IsRunning)
    {
        ErrorCount ++;
        if(ErrorCount > 2) // If 2 errors in a row, re-initiailize Lidar
        {
          IsRunning = false;            // @@@ can just restart Lidar here
          Serial.println("Disconnect");
        }
    }
  }
}
*/	
// ----------------------------------------------------------------------------	
// Return the current distance measurement. Parameter is set to the distance in cm
// or 0 if the measurement is invalid.
void Lidar::GetDistance (unsigned short& theDistance)
{
    // Distances above 64000 are error codes
    if (CurrentDistance >= 64000)
    		theDistance = 0;
    else
    		theDistance = CurrentDistance;	
}
	