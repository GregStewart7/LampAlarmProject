#ifndef LCDMANAGER_H
#define LCDMANAGER_H

#include <Arduino.h>
#include <LiquidCrystal.h>

class LCDManager {
  public:
    // Constructor: initialize with LCD pins (rs, en, d4, d5, d6, d7)
    LCDManager(int rs, int en, int d4, int d5, int d6, int d7);
    
    // Initialize the LCD.
    void begin();
    
    // Display the time (HH:MM) on the LCD.
    void displayTime(int hour, int minute);
    
    // Display a mode label (e.g. "CLOCK" or "SET CLOCK") at the top right.
    void displayMode(String mode);
    
    // Display mode selection options on the bottom row with the currently selected option underlined.
    void displayModeSelection(String opt1, String opt2, String selected);
  
  private:
    LiquidCrystal _lcd;
};

#endif
