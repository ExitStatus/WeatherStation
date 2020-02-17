#ifndef Button_h
#define Button_h

#include "Arduino.h"

class Button
{
  private:
    uint8_t _pin;
    uint8_t _currentState;
    
  public: 
    Button(uint8_t pin);
    bool StateChanged();
    bool Pressed();
    bool Clicked();
};

#endif