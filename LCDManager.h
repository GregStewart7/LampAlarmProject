#ifndef LCDMANAGER_H
#define LCDMANAGER_H

#include <Arduino.h>
#include <LiquidCrystal.h>

class LCDManager {
  public:
    // Constructor: initialize with LCD pins (rs, en, d4, d5, d6, d7)
    LCDManager(int rs, int en, int d4, int d5, int d6, int d7);
    
    // Initialize the LCD (set display size, etc.)
    void begin();
    
    // Display time in HH:MM format on the LCD.
    void displayTime(int hour, int minute);
  
  private:
    LiquidCrystal _lcd;
};

#endif
