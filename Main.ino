#include <Arduino.h>
#include "LCDManager.h"
#include "RealTimeClock.h"
#include "Wire.h"

// Create an instance of LCDManager with your LCD pins.
// Example: rs=12, en=11, d4=5, d5=4, d6=3, d7=2.
LCDManager lcdManager(23, 22, 4, 5, 6, 7);

// Create an instance of RealTimeClock with the button pin and a reference to lcdManager.
// Example: button pin 7.
RealTimeClock rtcClock(24, lcdManager);

void setup() {
  Serial.begin(115200);
  lcdManager.begin();
  Serial.println("lcdManager Running");
  rtcClock.begin();
  Serial.println("rtcClock Running");
}

void loop() {
  rtcClock.update();
}
