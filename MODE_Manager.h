#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

#include <Arduino.h>
#include "RTC_Manager.h"    // Provides RealTimeClock and GetDateTime()
#include "LCD_Manager.h"    // Provides LcdScreen and its display functions
#include "POT_Manager.h"    // Provides the Potentiometer class

// Mode state now includes SELECT_DATE_MODE and SELECT_TIME_MODE.
enum Mode { LAMP_MODE, SELECT_MODE, SELECT_DATE_MODE, SELECT_TIME_MODE , SELECT_ALARM_MODE};

class ModeManager 
{
  public:
    // Constructor: store pointers to RTC, LCD, and POT objects.
    ModeManager(RealTimeClock* rtc, LcdScreen* lcd, Potentiometer* pot);
    
    // Call Update() repeatedly (in your loop) to handle mode transitions and update the display.
    void Update();

  private:
    RealTimeClock* _rtc;
    LcdScreen* _lcd;
    Potentiometer* _pot;  // This pot is used for both menu selection (in SELECT_MODE) and for editing in the submodes.
    
    Mode _mode;
    
    // For the black button on pin 26 (used in LAMP_MODE and SELECT_MODE, and for saving changes in submodes)
    static const int BLACK_BUTTON_PIN = 26;
    unsigned long _lastButtonPressTime; // For debouncing the black button
    bool _buttonWasPressed;             // Tracks black button state

    // For the yellow button on pin 27 (used in SELECT_DATE_MODE and SELECT_TIME_MODE for adjustments)
    static const int YELLOW_BUTTON_PIN = 27;
    bool _yellowButtonWasPressed;
    unsigned long _yellowButtonPressStart;

    // Editable date values (set when entering SELECT_DATE_MODE)
    int _selectedYear;
    int _selectedMonth;
    int _selectedDay;

    // Editable time values (set when entering SELECT_TIME_MODE)
    int _selectedHour;
    int _selectedMinute;

    // Editable alarm time values
    int _selectedAlarmHour;
    int _selectedAlarmMinute;
};

#endif
