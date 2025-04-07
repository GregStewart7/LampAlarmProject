#include <Arduino.h>
#include "LCD_Manager.h"

// Helper function to convert month number to abbreviated string.
const char* MonthToStr(int month) {
  const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                          "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  if (month < 1 || month > 12)
    return "";
  return months[month - 1];
}

LcdScreen::LcdScreen(int rs, int en, int d4, int d5, int d6, int d7)
  : _lcd(rs, en, d4, d5, d6, d7) { }

void LcdScreen::Initialize()
{
  // Initialize the LCD screen which has 16 columns and 2 rows
  _lcd.begin(16,2);
}

void LcdScreen::LampModeDisplay(int year, int month, int day, int hour, int minute)
{
  // Clear the screen
  _lcd.clear();

  // Set the curson location
  _lcd.setCursor(0, 0);

  // Print the date and time formatted as Jan 1, 2025 hh:mm
  _lcd.print(MonthToStr(month));
  _lcd.print(" ");
  _lcd.print(day);
  _lcd.print(", ");
  _lcd.print(year);
  _lcd.print("  ");
  _lcd.setCursor(0, 1);
  if (hour < 10) // adds 0 infront of hour if less than 10
  {
    _lcd.print("0");
  }
  _lcd.print(hour);
  _lcd.print(":");
  if (minute < 10) // adds 0 infront of minute if less than 10
  {
    _lcd.print("0");
  }
  _lcd.print(minute);
}

void LcdScreen::SelectModeDisplay(String setAlarmOpt, String setDateOpt, String setTimeOpt, String selected)
{
  // Clear the screen 
  _lcd.clear();

  // Set the cursor location
  _lcd.setCursor(0, 0);

  // Depending on selected option print _D_, _T_, or _A_  to highlight current choice
  if (selected == setAlarmOpt)
  {
    _lcd.setCursor(3, 0);
    _lcd.print(setDateOpt);
    _lcd.setCursor(7, 0);
    _lcd.print(setTimeOpt);
    _lcd.setCursor(10, 0);
    _lcd.print("_" + setAlarmOpt + "_");
  }
  else if (selected == setDateOpt)
  {
    _lcd.setCursor(2, 0);
    _lcd.print("_" + setDateOpt + "_");
    _lcd.setCursor(7, 0);
    _lcd.print(setTimeOpt);
    _lcd.setCursor(11, 0);
    _lcd.print(setAlarmOpt);
  }
  else if (selected == setTimeOpt)
  {
    _lcd.setCursor(3, 0);
    _lcd.print(setDateOpt);
    _lcd.setCursor(6, 0);
    _lcd.print("_" + setTimeOpt + "_");
    _lcd.setCursor(11, 0);
    _lcd.print(setAlarmOpt);    
  }
  else
  {
    _lcd.print("Select Error");
  }
}

void LcdScreen::SetDateDisplay(int year, int month, int day, String indicator)
{
  // Clear the screen
  _lcd.clear();

  // Set the cursor
  _lcd.setCursor(0, 0);

  // Display the date to be modified
  _lcd.print(MonthToStr(month));
  _lcd.print(" ");
  _lcd.print(day);
  _lcd.print(", ");
  _lcd.print(year);
  _lcd.setCursor(0, 1);
  _lcd.print("Change: ");
  _lcd.print(indicator);
}

void LcdScreen::SetTimeDisplay(int hour, int minute, String indicator)
{
  // Clear the screen
  _lcd.clear();

  // Set the cursor
  _lcd.setCursor(0, 0);

  // Display the time to be modified
  if (hour < 10) // adds 0 infront of hour if less than 10
  {
    _lcd.print("0");
  }
  _lcd.print(hour);
  _lcd.print(":");
  if (minute < 10) // adds 0 infront of minute if less than 10
  {
    _lcd.print("0");
  }
  _lcd.print(minute);
  _lcd.setCursor(0, 1);
  _lcd.print("Change: ");
  _lcd.print(indicator);
}

void LcdScreen::SetAlarmDisplay(int hour, int minute, String setOrCancelAlarm)
{
  // Clear the screen
  _lcd.clear();

  // Set the cursor
  _lcd.setCursor(6, 0);

  // Display the alarm time to be modified
  if (hour < 10) // adds 0 infront of hour if less than 10
  {
    _lcd.print("0");
  }
  _lcd.print(hour);
  _lcd.print(":");
  if (minute < 10) // adds 0 infront of minute if less than 10
  {
    _lcd.print("0");
  }
  _lcd.print(minute);

  _lcd.setCursor(0, 1);
  _lcd.print("Change: ");
  _lcd.print(setOrCancelAlarm);
}







