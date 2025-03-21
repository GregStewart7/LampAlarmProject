#include "RealTimeClock.h"

RealTimeClock::RealTimeClock(int buttonPin, LCDManager &lcdManager)
  : _buttonPin(buttonPin), _lcdManager(lcdManager) {
  _hour = 0;
  _minute = 0;
  _second = 0;
  _lastDebounceTime = 0;
  _lastButtonState = HIGH;  // With INPUT_PULLUP, button is HIGH when not pressed.
  _buttonState = HIGH;
  _buttonPressStartTime = 0;
  _lastDisplayUpdate = 0;
  _modeSelectEnteredTime = 0;
  _mode = CLOCK_MODE;       // Start in CLOCK mode.
}

void RealTimeClock::begin() {
  pinMode(_buttonPin, INPUT_PULLUP);
  rtc.begin();

  // Only adjust the RTC if it lost power.
  if (rtc.lostPower()) {
    // If power was lost, set the RTC to a default time (e.g., January 1, 2023, 00:00).
    rtc.adjust(DateTime(2023, 1, 1, 0, 0, 0));
  }
  
  DateTime now = rtc.now();
  _hour = now.hour();
  _minute = now.minute();
  _second = now.second();
  
  // In CLOCK_MODE, display the time and mode identifier "C".
  _lcdManager.displayTime(_hour, _minute);
  _lcdManager.displayMode("C");
}

void RealTimeClock::update() {
  int reading = digitalRead(_buttonPin);
  int potValue = analogRead(A0);  // POT1 reading.
  
  // --- Debounce the button reading ---
  if (reading != _lastButtonState) {
    _lastDebounceTime = millis();
  }
  if ((millis() - _lastDebounceTime) > _debounceDelay) {
    if (reading != _buttonState) {
      _buttonState = reading;
      if (_buttonState == LOW) {
         // Button pressed: record the start time (if not already set).
         if (_buttonPressStartTime == 0) {
             _buttonPressStartTime = millis();
         }
      } else {  // Button released.
         unsigned long pressDuration = millis() - _buttonPressStartTime;
         _buttonPressStartTime = 0;
         if (_mode == SET_CLOCK_MODE) {
             if (pressDuration < 3000) {
                // Short press (<3 sec): increment minute.
                DateTime now = rtc.now();
                int newMinute = (now.minute() + 1) % 60;
                rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), newMinute, now.second()));
             } else if (pressDuration >= 3000 && pressDuration < 10000) {
                // Long press (3–10 sec): increment hour.
                DateTime now = rtc.now();
                int newHour = (now.hour() + 1) % 24;
                rtc.adjust(DateTime(now.year(), now.month(), now.day(), newHour, now.minute(), now.second()));
             } else if (pressDuration >= 10000) {
                // Very long press (>=10 sec): confirm changes and return to CLOCK_MODE.
                _mode = CLOCK_MODE;
             }
         } else if (_mode == MODE_SELECT) {
             // In MODE_SELECT, only accept a confirmation if at least 1 sec has passed.
             if (millis() - _modeSelectEnteredTime > 1000) {
                String selection = (potValue < 512) ? "C" : "S";
                if (selection == "S") {
                   _mode = SET_CLOCK_MODE;
                   _lcdManager.displayMode("S");
                } else {
                   _mode = CLOCK_MODE;
                   _lcdManager.displayMode("C");
                }
             }
         }
      }
    }
  }
  _lastButtonState = reading;
  
  // --- Mode-dependent behavior ---
  switch (_mode) {
    case CLOCK_MODE: {
       // Update the time display every second.
       if (millis() - _lastDisplayUpdate >= 1000) {
          DateTime now = rtc.now();
          _hour = now.hour();
          _minute = now.minute();
          _second = now.second();
          _lcdManager.displayTime(_hour, _minute);
          _lcdManager.displayMode("C");
          _lastDisplayUpdate = millis();
       }
       // In CLOCK_MODE, if the button is held for 5 seconds, enter MODE_SELECT.
       if (_buttonState == LOW && _buttonPressStartTime != 0 &&
           (millis() - _buttonPressStartTime >= 1000)) {
          _mode = MODE_SELECT;
          _modeSelectEnteredTime = millis(); // Record when MODE_SELECT is entered.
          String selection = (potValue < 512) ? "C" : "S";
          _lcdManager.displayModeSelection("C", "S", selection);
          _buttonPressStartTime = 0;
       }
       break;
    }
    case MODE_SELECT: {
       // Update the selection prompt only if needed (at most once per second).
       String selection = (potValue < 512) ? "C" : "S";
       static String lastSelection = "";
       if (selection != lastSelection || (millis() - _lastDisplayUpdate >= 1000)) {
          _lcdManager.displayModeSelection("C", "S", selection);
          lastSelection = selection;
          _lastDisplayUpdate = millis();
       }
       break;
    }
    case SET_CLOCK_MODE: {
       // Update the display every second with the "S" indicator.
       if (millis() - _lastDisplayUpdate >= 1000) {
          DateTime now = rtc.now();
          _hour = now.hour();
          _minute = now.minute();
          _second = now.second();
          _lcdManager.displayTime(_hour, _minute);
          _lcdManager.displayMode("S");
          _lastDisplayUpdate = millis();
       }
       break;
    }
  }
}
