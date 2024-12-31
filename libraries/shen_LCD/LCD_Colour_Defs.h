#ifndef LCD_COLOUR_DEFS
#define LCD_COLOUR_DEFS

// These are the colours that the LCD foreground and background can be set to
const char* LCDColourLabels[] = {"Black", "White", "Red", "Green", "Blue", "Cyan", "Magenta", "Yellow", "Orange" };
const short  LCDColourSettings[] = {ST77XX_BLACK , ST77XX_WHITE, ST77XX_RED, ST77XX_GREEN, ST77XX_BLUE, ST77XX_CYAN, ST77XX_MAGENTA, ST77XX_YELLOW, ST77XX_ORANGE };

// These are the indices of the colours that the LCD foreground and background can be set to. Must match LCDColourLabels and LCDColourSettings define in ino file @@@ Clean up, this is crap
#define LCD_COLOUR_INDEX_BLACK   0
#define LCD_COLOUR_INDEX_WHITE   1
#define LCD_COLOUR_INDEX_RED     2
#define LCD_COLOUR_INDEX_GREEN   3
#define LCD_COLOUR_INDEX_BLUE    4
#define LCD_COLOUR_INDEX_CYAN    5
#define LCD_COLOUR_INDEX_MAGENTA 6
#define LCD_COLOUR_INDEX_YELLOW  7
#define LCD_COLOUR_INDEX_ORANGE  8

#endif
