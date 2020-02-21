#include "Button.h"

Button::Button(uint8_t pin)
{
    _pin = pin;
    pinMode(_pin, INPUT_PULLUP);
    _currentState = HIGH;
}


uint8_t Button::Clicked()
{
    int newState = digitalRead(_pin);
    
    if (newState == HIGH && _currentState == HIGH) 
        return BUTTON_NONE;

    if (newState == LOW && _currentState == HIGH)
    {
        _downtime = millis();
        _currentState = LOW;

        return BUTTON_NONE;
    }

    if (newState == HIGH && _currentState == LOW)
    {
        _currentState = HIGH;

        if (_downtime == 0)
            return BUTTON_NONE;
        
        if (millis() - _downtime < 10)
            return BUTTON_BOUNCED;

        if (millis() - _downtime < 1000)
            return BUTTON_CLICKED;
    }

    if (_downtime > 0 && (millis() - _downtime > 4000))
    {
        _downtime = 0;
        return BUTTON_HELD;
    }

    return BUTTON_NONE;
}