#ifndef RTCMANAGER_H
#define RTCMANAGER_H

#include <Arduino.h>
#include <RTClib.h>

class RealTimeClock {
  public:
    // Constructor.
    RealTimeClock();

    // Initialize the DS3231 RTC.
    void Initialize();

    // Set the desired date and time (stored locally until applied).
    void SetDate(int year, int month, int day);
    void SetTime(int hour, int minute);
    // Write the stored date and time to the RTC.
    void SetClock();

    // Alarm functions:
    // Program a daily alarm for the specified hour and minute.
    void SetAlarm(int hour, int minute);
    // Clear the alarm (disable alarm checking).
    void ClearAlarm();
    // Poll the RTC time; if the alarm is set and the current time matches, set the alarmTriggered flag.
    void CheckAlarm();

    // Get the current DateTime from the RTC.
    DateTime GetDateTime();

    // This volatile flag is set when the alarm condition is met.
    volatile bool alarmTriggered;

  private:
    // Stored date/time values.
    int _year;
    int _month;
    int _day;
    int _hour;
    int _minute;
    int _second;

    // Alarm time variables.
    int _alarmHour;
    int _alarmMinute;
    bool _alarmSet;

    // DS3231 RTC object.
    RTC_DS3231 _rtc;
};

#endif
