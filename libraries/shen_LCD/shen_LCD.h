#ifndef _SHEN_LCD
#define _SHEN_LCD

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

class shen_LCD : public Adafruit_ST7735
{
  public: 
  
  shen_LCD (unsigned chipSelect,
            unsigned dc,
            unsigned reset) : Adafruit_ST7735 (chipSelect,dc,reset)
   {}
 

 
};

#endif
