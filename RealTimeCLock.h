#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

#include <Arduino.h>
#include <RTClib.h>         // Adafruit's RTClib for DS3231
#include "LCDManager.h"     // Our LCD management module

// Define the different operating modes.
enum ClockMode { CLOCK_MODE, MODE_SELECT, SET_CLOCK_MODE };

class RealTimeClock {
  public:
    // Constructor: takes the button pin for updating time and a reference to an LCDManager.
    RealTimeClock(int buttonPin, LCDManager &lcdManager);
    
    // Initialize the DS3231, LCD, and set the initial time.
    void begin();
    
    // Call in loop() to process mode changes and update the display.
    void update();
    
  private:
    int _buttonPin;          // Digital pin for the button.
    RTC_DS3231 rtc;          // DS3231 RTC object from RTClib.
    LCDManager &_lcdManager; // Reference to the LCDManager.
    
    // Debounce and button press duration variables.
    int _lastButtonState;
    int _buttonState;
    unsigned long _lastDebounceTime;
    const unsigned long _debounceDelay = 50;  // milliseconds.
    unsigned long _buttonPressStartTime;
    
    // Variable to update the display once per second.
    unsigned long _lastDisplayUpdate;
    
    // Variable to record when MODE_SELECT is entered.
    unsigned long _modeSelectEnteredTime;
    
    // Current time values.
    int _hour;
    int _minute;
    int _second;
    
    // Current operating mode.
    ClockMode _mode;
};

#endif
