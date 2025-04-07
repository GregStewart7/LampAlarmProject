#include <Arduino.h>
#include "RTC_Manager.h"

RealTimeClock::RealTimeClock() : alarmTriggered(false), _alarmSet(false) {
  // Set default date/time values (make sure month is 1–12).
  _year   = 2025;
  _month  = 1;
  _day    = 1;
  _hour   = 0;
  _minute = 0;
  _second = 0;

  // Default alarm values.
  _alarmHour   = 0;
  _alarmMinute = 0;
}

void RealTimeClock::Initialize() {
  _rtc.begin();
  _rtc.disable32K();

  // If power was lost, adjust the RTC to the compile time.
  if (_rtc.lostPower()) {
    _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void RealTimeClock::SetDate(int year, int month, int day) {
  _year  = year;
  _month = month;
  _day   = day;
}

void RealTimeClock::SetTime(int hour, int minute) {
  _hour   = hour;
  _minute = minute;
}

void RealTimeClock::SetClock() {
  // Update the RTC with the stored date and time.
  DateTime dt(_year, _month, _day, _hour, _minute, _second);
  _rtc.adjust(dt);
}

void RealTimeClock::SetAlarm(int hour, int minute) {
  _alarmHour   = hour;
  _alarmMinute = minute;
  _alarmSet    = true;
  alarmTriggered = false;  // Reset flag
}

void RealTimeClock::ClearAlarm() {
  _alarmSet = false;
  alarmTriggered = false;
}

void RealTimeClock::CheckAlarm() {
  if (_alarmSet) {
    DateTime now = _rtc.now();
    // Check if the current hour and minute match the stored alarm time.
    // To ensure a one‑time trigger, we check that seconds is near the start of the minute.
    if (now.hour() == _alarmHour && now.minute() == _alarmMinute && now.second() < 2) {
      alarmTriggered = true;
    }
  }
}

DateTime RealTimeClock::GetDateTime() {
  return _rtc.now();
}
