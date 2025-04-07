#ifndef POT_Manager_H
#define POT_Manager_H

#include <Arduino.h>

class Potentiometer 
{
  public:
  // Constructor for read-only POT:
  Potentiometer(int potPin);

  // Constructor for read and output POT:
  Potentiometer(int potPin, int pwmPin);

  // Functions:
  void Initialize();
  double ReadPOT();
  double MakePWM();

  private:
  // Variables
  int _potPin;
  int _pwmPin;
  double _potValue;
  double _pwmValue;
};

#endif