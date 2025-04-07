#include "MP3_Manager.h"

MP3_Manager::MP3_Manager(uint8_t rxPin, uint8_t txPin)
  : _serial(rxPin, txPin) // Initialize SoftwareSerial with given pins.
{
  // Nothing else needed in the constructor.
}

bool MP3_Manager::Initialize() {
  _serial.begin(9600);
  // Initialize the DFPlayerMini.
  // If initialization fails, return false.
  if (!_dfPlayer.begin(_serial, false)) {
    return false;
  }
  return true;
}

void MP3_Manager::SetVolume(uint8_t volume) {
  _dfPlayer.volume(volume);
}

void MP3_Manager::Play(uint16_t track) {
  _dfPlayer.play(track);
}
