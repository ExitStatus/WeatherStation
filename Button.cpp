#include "Button.h"

Button::Button(uint8_t pin)
{
    _pin = pin;
    pinMode(_pin, INPUT_PULLUP);
    _currentState = HIGH;
}

bool Button::StateChanged()
{
    uint8_t buttonState = digitalRead(_pin);
    if (buttonState != _currentState)
    {
        _currentState = buttonState;
        return true;
    }

    return false;
}

bool Button::Pressed()
{
    if (_currentState == LOW)
        return true;
    else
        return false;
}

bool Button::Clicked()
{
    if (StateChanged() && !Pressed())
        return true;
    else
        return false;
}