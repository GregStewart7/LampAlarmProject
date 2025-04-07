#include "MODE_Manager.h"

ModeManager::ModeManager(RealTimeClock* rtc, LcdScreen* lcd, Potentiometer* pot)
  : _rtc(rtc),
    _lcd(lcd),
    _pot(pot),
    _mode(LAMP_MODE),
    _lastButtonPressTime(0),
    _buttonWasPressed(false),
    _yellowButtonWasPressed(false),
    _yellowButtonPressStart(0),
    _selectedYear(0),
    _selectedMonth(0),
    _selectedDay(0),
    _selectedHour(0),
    _selectedMinute(0),
    _selectedAlarmHour(0),
    _selectedAlarmMinute(0)
{
  pinMode(BLACK_BUTTON_PIN, INPUT_PULLUP);
  pinMode(YELLOW_BUTTON_PIN, INPUT_PULLUP);
}

void ModeManager::Update() 
{
  unsigned long currentMillis = millis();

  // Read black button state (active LOW)
  bool blackButtonPressed = (digitalRead(BLACK_BUTTON_PIN) == LOW);

  switch(_mode) 
  {
    case LAMP_MODE:
      {
        // Display current time.
        DateTime nowTime = _rtc->GetDateTime();
        _lcd->LampModeDisplay(nowTime.year(), nowTime.month(), nowTime.day(), nowTime.hour(), nowTime.minute());
        
        // If black button is pressed (and debounced), switch to SELECT_MODE.
        if (blackButtonPressed && !_buttonWasPressed && (currentMillis - _lastButtonPressTime > 200)) {
          _mode = SELECT_MODE;
          _lastButtonPressTime = currentMillis;
        }
      }
      break;
      
    case SELECT_MODE:
      {
        // Use the pot reading to choose among options.
        // Map the pot value into three options: "D" (Date), "T" (Time), and "A" (Alarm).
        double potValue = _pot->ReadPOT();
        String selected;
        if (potValue < 341) {
          selected = "D";
        } else if (potValue < 683) {
          selected = "T";
        } else {
          selected = "A";
        }
        
        _lcd->SelectModeDisplay("A", "D", "T", selected);
        
        // Now, use the black button (when pressed and debounced) to choose the mode.
        if (blackButtonPressed && !_buttonWasPressed && (currentMillis - _lastButtonPressTime > 200)) {
          if (selected == "D") {
            // Enter SELECT_DATE_MODE. Copy current date from the RTC.
            DateTime nowTime = _rtc->GetDateTime();
            _selectedYear  = nowTime.year();
            _selectedMonth = nowTime.month();
            _selectedDay   = nowTime.day();
            _lastButtonPressTime = currentMillis;
            _mode = SELECT_DATE_MODE;
          }
          else if (selected == "T") {
            // Enter SELECT_TIME_MODE. Copy current time from the RTC.
            DateTime nowTime = _rtc->GetDateTime();
            _selectedHour = nowTime.hour();
            _selectedMinute = nowTime.minute();
            _lastButtonPressTime = currentMillis;
            _mode = SELECT_TIME_MODE;
          }
          else {
            // Enter SELECT_ALARM_MODE. Copy current time from the RTC.
            DateTime nowTime = _rtc->GetDateTime();
            _selectedAlarmHour = nowTime.hour();
            _selectedAlarmMinute = nowTime.minute();
            _lastButtonPressTime = currentMillis;
            _mode = SELECT_ALARM_MODE;
          }
        }
      }
      break;
      
    case SELECT_DATE_MODE:
      {
        // In SELECT_DATE mode, use the pot to determine the direction.
        double potReading = _pot->ReadPOT();
        String direction = (potReading > 512) ? "U" : "D";  // U: increment, D: decrement
        
        // Display the editable date (month, day, year) with the indicator.
        _lcd->SetDateDisplay(_selectedYear, _selectedMonth, _selectedDay, direction);

        // Handle yellow button for adjusting the date.
        bool yellowButtonPressed = (digitalRead(YELLOW_BUTTON_PIN) == LOW);
        if (yellowButtonPressed && !_yellowButtonWasPressed) {
          _yellowButtonPressStart = currentMillis;
        }
        if (!yellowButtonPressed && _yellowButtonWasPressed) {
          unsigned long pressDuration = currentMillis - _yellowButtonPressStart;
          if (pressDuration < 2000) {
            // Short press: adjust day.
            if (potReading > 512)
              _selectedDay++;
            else
              _selectedDay--;
            if (_selectedDay < 1) _selectedDay = 31;
            if (_selectedDay > 31) _selectedDay = 1;
          }
          else if (pressDuration < 4000) {
            // Medium press: adjust month.
            if (potReading > 512)
              _selectedMonth++;
            else
              _selectedMonth--;
            if (_selectedMonth < 1) _selectedMonth = 12;
            if (_selectedMonth > 12) _selectedMonth = 1;
          }
          else {
            // Long press: adjust year.
            if (potReading > 512)
              _selectedYear++;
            else
              _selectedYear--;
          }
        }
        _yellowButtonWasPressed = yellowButtonPressed;

        // In SELECT_DATE_MODE, use the black button to save the new date.
        if (blackButtonPressed && !_buttonWasPressed && (currentMillis - _lastButtonPressTime > 200)) {
          _rtc->SetDate(_selectedYear, _selectedMonth, _selectedDay);
          _rtc->SetClock();  // Update RTC with new date.
          _lastButtonPressTime = currentMillis;
          _mode = LAMP_MODE;
        }
      }
      break;
    
    case SELECT_TIME_MODE:
      {
        // In SELECT_TIME mode, use the pot to determine the direction.
        double potReading = _pot->ReadPOT();
        String direction = (potReading > 512) ? "U" : "D";  // U: increment, D: decrement
        
        // Display the editable time with the indicator.
        _lcd->SetTimeDisplay(_selectedHour, _selectedMinute, direction);
        
        // Handle yellow button for adjusting the time.
        bool yellowButtonPressed = (digitalRead(YELLOW_BUTTON_PIN) == LOW);
        if (yellowButtonPressed && !_yellowButtonWasPressed) {
          _yellowButtonPressStart = currentMillis;
        }
        if (!yellowButtonPressed && _yellowButtonWasPressed) {
          unsigned long pressDuration = currentMillis - _yellowButtonPressStart;
          if (pressDuration < 2000) {
            // Short press: adjust minute.
            if (potReading > 512)
              _selectedMinute++;
            else
              _selectedMinute--;
            if (_selectedMinute < 0) _selectedMinute = 59;
            if (_selectedMinute > 59) _selectedMinute = 0;
          }
          else {
            // Press >= 2000 ms: adjust hour.
            if (potReading > 512)
              _selectedHour++;
            else
              _selectedHour--;
            if (_selectedHour < 0) _selectedHour = 23;
            if (_selectedHour > 23) _selectedHour = 0;
          }
        }
        _yellowButtonWasPressed = yellowButtonPressed;
        
        // In SELECT_TIME_MODE, use the black button to save the new time.
        if (blackButtonPressed && !_buttonWasPressed && (currentMillis - _lastButtonPressTime > 200)) {
          _rtc->SetTime(_selectedHour, _selectedMinute);
          _rtc->SetClock();  // Update RTC with new time.
          _lastButtonPressTime = currentMillis;
          _mode = LAMP_MODE;
        }
      }
      break;

        case SELECT_ALARM_MODE:
      {
        // Use the pot reading to decide whether the user wants to set or cancel the alarm.
        double potReading = _pot->ReadPOT();
        String setOrNot = (potReading > 512) ? "SA" : "CA";  // SA: set alarm, CA: cancel alarm

        // Display the editable alarm time along with the indicator.
        _lcd->SetAlarmDisplay(_selectedAlarmHour, _selectedAlarmMinute, setOrNot);

        // Handle yellow button for adjusting the alarm time.
        bool yellowButtonPressed = (digitalRead(YELLOW_BUTTON_PIN) == LOW);
        if (yellowButtonPressed && !_yellowButtonWasPressed) {
          _yellowButtonPressStart = currentMillis;
        }
        if (!yellowButtonPressed && _yellowButtonWasPressed) {
          unsigned long pressDuration = currentMillis - _yellowButtonPressStart;
          if (pressDuration < 2000) {
            // Short press: adjust minute.
            _selectedAlarmMinute++;
            if (_selectedAlarmMinute < 0) _selectedAlarmMinute = 59;
            if (_selectedAlarmMinute > 59) _selectedAlarmMinute = 0;
          }
          else {
            // Press >= 2000 ms: adjust hour.
            _selectedAlarmHour++;
            if (_selectedAlarmHour < 0) _selectedAlarmHour = 23;
            if (_selectedAlarmHour > 23) _selectedAlarmHour = 0;
          }
        }
        _yellowButtonWasPressed = yellowButtonPressed;
        
        // When the black button is pressed in SELECT_ALARM_MODE,
        // if the pot indicates "SA" then set the alarm; if "CA" then clear alarms.
        if (blackButtonPressed && !_buttonWasPressed && (currentMillis - _lastButtonPressTime > 200)) {
          if (potReading > 512) {
            // Set the alarm time on the RTC.
            _rtc->SetAlarm(_selectedAlarmHour, _selectedAlarmMinute);
          } else {
            // Clear any alarms on the RTC.
            _rtc->ClearAlarm();
          }
          _lastButtonPressTime = currentMillis;
          _mode = LAMP_MODE;
        }
      }
      break;
  }

  _buttonWasPressed = blackButtonPressed;
  delay(100);
}
