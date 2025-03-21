#include "LCDManager.h"

LCDManager::LCDManager(int rs, int en, int d4, int d5, int d6, int d7)
  : _lcd(rs, en, d4, d5, d6, d7) {
}

void LCDManager::begin() {
  _lcd.begin(16, 2);
}

void LCDManager::displayTime(int hour, int minute) {
  // Clear entire display for simplicity.
  _lcd.clear();
  // Display time (HH:MM) on row 1 (bottom row).
  _lcd.setCursor(0, 1);
  if (hour < 10) _lcd.print("0");
  _lcd.print(hour);
  _lcd.print(":");
  if (minute < 10) _lcd.print("0");
  _lcd.print(minute);
}

void LCDManager::displayMode(String mode) {
  // Display the mode label in the top right corner.
  // For a 16-character display, starting around column 10 works for short labels.
  _lcd.setCursor(1, 0);
  _lcd.print(mode);
}

void LCDManager::displayModeSelection(String opt1, String opt2, String selected) {
  // Clear only the bottom row.
  for (int i = 0; i < 16; i++) {
    _lcd.setCursor(i, 1);
    _lcd.print(" ");
  }
  // Display two options (each roughly half the screen).
  _lcd.setCursor(1, 1);
  if (selected == opt1) {
    _lcd.print("_" + opt1 + "_");
  } else {
    _lcd.print(opt1);
  }
  _lcd.setCursor(5, 1);
  if (selected == opt2) {
    _lcd.print("_" + opt2 + "_");
  } else {
    _lcd.print(opt2);
  }
}

