#include "LCDManager.h"

LCDManager::LCDManager(int rs, int en, int d4, int d5, int d6, int d7)
  : _lcd(rs, en, d4, d5, d6, d7) {
}

void LCDManager::begin() {
  _lcd.begin(16, 2);
}

void LCDManager::displayTime(int hour, int minute) {
  _lcd.clear();
  _lcd.setCursor(0, 0);
  
  // Format and display the time as HH:MM.
  if (hour < 10) _lcd.print("0");
  _lcd.print(hour);
  _lcd.print(":");
  if (minute < 10) _lcd.print("0");
  _lcd.print(minute);
}
