#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

#include <Arduino.h>
#include <RTClib.h>         // Adafruit's RTClib for DS3231
#include "LCDManager.h"     // Our LCD management module

class RealTimeClock {
  public:
    // Constructor: takes the button pin for updating time and a reference to an LCDManager.
    RealTimeClock(int buttonPin, LCDManager &lcdManager);

    // Initialize the DS3231 and LCD, and set the initial time to 00:00:00.
    void begin();

    // Call in loop() to check for button presses and update the time.
    void update();

  private:
    int _buttonPin;          // Digital pin for the button.
    RTC_DS3231 rtc;          // DS3231 RTC object from RTClib.
    LCDManager &_lcdManager; // Reference to the LCDManager for updating the display.

    // Debounce variables.
    int _lastButtonState;
    int _buttonState;
    unsigned long _lastDebounceTime;
    const unsigned long _debounceDelay = 50;  // Debounce delay in milliseconds.

    // Variable for measuring the duration of a button press.
    unsigned long _buttonPressStartTime;

    // Variable to update the display only once per second.
    unsigned long _lastDisplayUpdate;

    // Current time values.
    int _hour;
    int _minute;
    int _second;
};

#endif
