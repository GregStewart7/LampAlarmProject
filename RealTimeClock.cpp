#include "RealTimeClock.h"

RealTimeClock::RealTimeClock(int buttonPin, LCDManager &lcdManager)
  : _buttonPin(buttonPin), _lcdManager(lcdManager) {
  // Initialize time to 00:00:00.
  _hour = 0;
  _minute = 0;
  _second = 0;
  _lastDebounceTime = 0;
  _lastButtonState = HIGH;  // With INPUT_PULLUP, button is HIGH when not pressed.
  _buttonState = HIGH;
  _buttonPressStartTime = 0;
  _lastDisplayUpdate = 0;
}

void RealTimeClock::begin() {
  pinMode(_buttonPin, INPUT_PULLUP);
  rtc.begin();

  // Set the DS3231 time to 00:00:00 on an arbitrary date (e.g., January 1, 2023).
  rtc.adjust(DateTime(2023, 1, 1, _hour, _minute, _second));

  // Read back the current time.
  DateTime now = rtc.now();
  _hour = now.hour();
  _minute = now.minute();
  _second = now.second();

  // Update the LCD display with the initial time (only HH:MM).
  _lcdManager.displayTime(_hour, _minute);
}

void RealTimeClock::update() {
  // --- Button press handling with duration measurement ---
  int reading = digitalRead(_buttonPin);

  // Reset debounce timer if the reading has changed.
  if (reading != _lastButtonState) {
    _lastDebounceTime = millis();
  }

  // If the reading has been stable for longer than the debounce delay...
  if ((millis() - _lastDebounceTime) > _debounceDelay) {
    if (reading != _buttonState) {
      _buttonState = reading;
      if (_buttonState == LOW) {
        // Button pressed: record the start time.
        _buttonPressStartTime = millis();
      } else {  // Button released.
        if (_buttonPressStartTime != 0) {  // Ensure a press was recorded.
          unsigned long pressDuration = millis() - _buttonPressStartTime;
          _buttonPressStartTime = 0; // Reset the press start time.
          DateTime now = rtc.now();
          if (pressDuration < 1000) {
            // Short press (<3 sec): increment minute.
            int newMinute = (now.minute() + 1) % 60;
            rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), newMinute, now.second()));
          } else {
            // Long press (>=3 sec): increment hour.
            int newHour = (now.hour() + 1) % 24;
            rtc.adjust(DateTime(now.year(), now.month(), now.day(), newHour, now.minute(), now.second()));
          }
        }
      }
    }
  }
  _lastButtonState = reading;

  // --- Update the display every second (showing only HH:MM) ---
  if (millis() - _lastDisplayUpdate >= 1000) {
    DateTime now = rtc.now();
    _hour = now.hour();
    _minute = now.minute();
    _second = now.second();
    _lcdManager.displayTime(_hour, _minute);
    _lastDisplayUpdate = millis();
  }
}
