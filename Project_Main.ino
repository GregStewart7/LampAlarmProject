/* 
 * Arduino LAMP_ALARM Project
 * Version 1.0
 * Author: Gregory Stewart
 * 
*/

#include <Arduino.h>
#include "POT_Manager.h"
#include "RTC_Manager.h"
#include "LCD_Manager.h"
#include "MODE_Manager.h"
#include "MP3_Manager.h"

// Create POT objects (pot1 may be used for other functions; pot2 is used for selection and date editing)
Potentiometer pot1(A1, 13);
Potentiometer pot2(A2);

// Create RTC and LCD objects (adjust pin numbers as needed)
RealTimeClock rtc;
LcdScreen lcd(25, 24, 4, 5, 6, 7);

// Create DFPlayer object to play MP3 file (RX, TX)
MP3_Manager mp3(9, 10);

// Create the ModeManager, using pot2 for selection and date editing.
ModeManager modeManager(&rtc, &lcd, &pot2);

void setup() 
{  
  Serial.begin(115200);
  pot1.Initialize();
  pot2.Initialize();
  rtc.Initialize();
  lcd.Initialize();
   // Initialize the MP3 manager.
  if (!mp3.Initialize()) {
    Serial.println("MP3 Initialization Error");
  }
  mp3.SetVolume(15);  // Set volume (0–30)
}

void loop() 
{
  modeManager.Update();
  pot1.MakePWM();
  rtc.CheckAlarm();
if (rtc.alarmTriggered) {
    // Execute your alarm action (e.g., call your MusicPlayer::playMusic() method).
    rtc.alarmTriggered = false;  // Reset the flag.
    Serial.println("ALARM IS GOING OFF");
    mp3.Play(1);
  }
  delay(100);
}
