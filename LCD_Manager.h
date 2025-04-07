#ifndef LCDMANAGER_H
#define LCDMANAGER_H

#include <Arduino.h>
#include <LiquidCrystal.h>

class LcdScreen 
{
  public: 
  // Constructor
  LcdScreen(int rs, int en, int d4, int d5, int d6, int d7);

  // Functions
  void Initialize();
  void LampModeDisplay(int year, int month, int day, int hour, int minute);
  void SelectModeDisplay(String setAlarmOpt, String setDateOpt, String setTimeOpt, String selected);
  void SetDateDisplay(int year, int month, int day, String indicator);
  void SetTimeDisplay(int hour, int minute, String indicator);
  void SetAlarmDisplay(int hour, int minute, String setOrCancelAlarm);

  private:
  // LCD object from the LiquidCrystal library
  LiquidCrystal _lcd;
};

#endif