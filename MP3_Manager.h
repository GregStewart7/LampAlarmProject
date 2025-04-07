#ifndef MP3_MANAGER_H
#define MP3_MANAGER_H

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

class MP3_Manager {
  public:
    // Constructor: rxPin and txPin are used for the SoftwareSerial connection.
    MP3_Manager(uint8_t rxPin, uint8_t txPin);
    
    // Initialize the DFPlayer. Returns true if initialization is successful.
    bool Initialize();
    
    // Set volume (valid values: 0–30).
    void SetVolume(uint8_t volume);
    
    // Play the specified track number.
    void Play(uint16_t track);
    
  private:
    SoftwareSerial _serial;       // SoftwareSerial instance for DFPlayer communication.
    DFRobotDFPlayerMini _dfPlayer;  // DFPlayerMini instance.
};

#endif
