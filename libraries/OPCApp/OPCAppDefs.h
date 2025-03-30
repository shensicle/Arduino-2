#ifndef _OPCAPPDEFS_H
#define _OPCAPPDEFS_H

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

	typedef enum bit_depth_t 
	{
		bit_depth_8        = 8,
		bit_depth_16       = 16
	};
	
	typedef enum colour_mode_t
	{
		colour_mode_grayscale = 0x00,
		colour_mode_RGB       = 0x01
	};
	
	typedef enum filter_method_t 
	{
		filter_method_default   = 0x00
	};
	
	typedef enum scan_t 
	{
		scan_progressive   = 0x00,
		scan_zigzag        = 0x01
	};
	
	
	// Structure to hold the camera config. Used to store configuration in non-volatile
	// storage and to pass it back and forth betweent this application and the
	// user interfaces. This structure is also stored in the raw image file so that
	// the PNG converter can understand the raw image.
	typedef struct camera_config_t
	{
		unsigned long   version;
		unsigned long   imageWidth;
		unsigned long   imageHeight;
		bit_depth_t     bitDepth;
		colour_mode_t   colourType;
		unsigned char   unused1;
		filter_method_t filterMethod;
		scan_t          scanType;
	};
	
#define OPC_CONFIGURATION_VERSION   1
#define DEFAULT_IMAGE_WIDTH       300
#define DEFAULT_IMAGE_HEIGHT      300
#define DEFAULT_BIT_DEPTH         bit_depth_t::bit_depth_16
#define DEFAULT_COLOUR_MODE       colour_mode_t::colour_mode_grayscale
#define DEFAULT_FILTER_METHOD     filter_method_t::filter_method_default
#define DEFAULT_SCAN_TYPE         scan_t::scan_zigzag

#endif


