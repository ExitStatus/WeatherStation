#include "Settings.h"

Settings::Settings(Tiny_SH1106 *lcd, Button *leftButton, Button *middleButton, Button *rightButton)
{
    _lcd = lcd;
    _leftButton = leftButton;
    _middleButton = middleButton;
    _rightButton = rightButton;
}

void Settings::UpdateSettingsUI()
{
    bool wasExitSelected = false;

    _lcd->clearDisplay();
    _lcd->setTextSize(1);
    _lcd->setTextColor(WHITE);
    _lcd->setCursor(10,0);
    _lcd->print(F("Configuration Menu"));
    _lcd->drawFastHLine(0, 8, 128, WHITE);

    _lcd->display();

    while (!wasExitSelected)
    {
         uint8_t middle = _middleButton->Clicked();
         if (middle == BUTTON_HELD)
            wasExitSelected = true;
    }

    _lcd->clearDisplay();
}