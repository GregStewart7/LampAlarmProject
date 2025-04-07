#include <Arduino.h>
#include "POT_Manager.h"

// Creates read-only object
Potentiometer::Potentiometer(int potPin)
  : _potPin(potPin), _pwmPin(-1), _potValue(0), _pwmValue(0) { }

Potentiometer::Potentiometer(int potPin, int pwmPin)
  : _potPin(potPin), _pwmPin(pwmPin), _potValue(0), _pwmValue(0) { }

void Potentiometer::Initialize() 
{
  // Initialize POT pin as an INPUT
  pinMode(_potPin, INPUT);

  // Initialize PWM pin as an OUTPUT
  if (_pwmPin >= 0) 
  {
    pinMode(_pwmPin, OUTPUT);
  }
}

double Potentiometer::ReadPOT() 
{
  // Read the value from the POT
  _potValue = analogRead(_potPin);
  
  return _potValue;
}

double Potentiometer::MakePWM()
{
  // Read the value from the POT
  ReadPOT();

  // Map the potValue (0 to 1023) to a PWM value (0 to 255)
  _pwmValue = map(_potValue, 0, 1023, 0, 255);

  // Output the mapped PWM value 
  analogWrite(_pwmPin, _pwmValue);
 
  return _pwmValue;
}