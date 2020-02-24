#include "Settings.h"

#define MAX_MENU_ITEMS  5

#define MENU_RESET_MAXMIN   0
#define MENU_TEMP_UNITS     1
#define MENU_ALTITUDE       2
#define MENU_WIFI           3
#define MENU_EXIT           4

Settings::Settings(Tiny_SH1106 *lcd, Button *leftButton, Button *middleButton, Button *rightButton, Stats *stats, ClockTime *clockTime)
{
    _lcd = lcd;
    _leftButton = leftButton;
    _middleButton = middleButton;
    _rightButton = rightButton;
    _stats = stats;
    _clockTime = clockTime;
}

void Settings::UpdateSettingsUI()
{
    bool wasExitSelected = false;

    _lcd->clearDisplay();
    _lcd->setTextSize(1);
    _lcd->setTextColor(WHITE);
    _lcd->setCursor(10,0);
    _lcd->print(F("Configuration Menu"));
    _lcd->drawFastHLine(0, 10, 128, WHITE);

    _lcd->display();

    int firstItem = 0;
    int selectedItem = 0;

    DrawMenu(firstItem, selectedItem);

    while (!wasExitSelected)
    {
         uint8_t middle = _middleButton->State();

         if ((middle == BUTTON_CLICKED || middle == BUTTON_HELD) && selectedItem == MENU_EXIT)
         {
            wasExitSelected = true;
            continue;
         }

        if (middle == BUTTON_HELD)
        {
            Serial.println("SETTING MIDDLE");
            ChangeSetting(selectedItem);
            DrawMenu(firstItem, selectedItem);
        }

        if (_rightButton->State() == BUTTON_CLICKED && selectedItem < MAX_MENU_ITEMS - 1)
        {
            selectedItem++;
            DrawMenu(firstItem, selectedItem);
        }

        if (_leftButton->State() == BUTTON_CLICKED && selectedItem > 0)
        {
            selectedItem--;
            DrawMenu(firstItem, selectedItem);
        }

        _stats->RecordStats(_clockTime);
    }

    _lcd->clearDisplay();
}

void Settings::DrawMenu(int firstItem, int selectedItem)
{
    int currentItem = firstItem;
    int y = 14;

    while (currentItem < MAX_MENU_ITEMS)
    {
        uint16_t background = currentItem == selectedItem ? WHITE : BLACK;
        uint16_t foreground = currentItem == selectedItem ? BLACK : WHITE;

        _lcd->fillRect(0, y, 128, 10, background);
        _lcd->setTextColor(foreground);
        _lcd->setCursor(4,y+1);

        switch(currentItem)
        {
            case MENU_RESET_MAXMIN: 
                _lcd->print(F("Reset Max / Min"));
                break;

            case MENU_TEMP_UNITS:
                _lcd->print(F("Units: Centigrade"));
                break;

            case MENU_ALTITUDE:
                _lcd->print(F("Altitude"));
                break;

            case MENU_WIFI:
                _lcd->print(F("WIFI"));
                break;

            case MENU_EXIT:
                _lcd->print(F("Exit"));
                break;
        }

        y += 10;
        currentItem++;
    }

    _lcd->display();
}

void Settings::ChangeSetting(int selectedItem)
{
    _lcd->fillRect(0, 11, 128, 53, BLACK);

    switch(selectedItem)
    {
        case MENU_RESET_MAXMIN: ResetMaxMin(); break;
        /*
        case MENU_TEMP_UNITS
            _lcd->print(F("Units: Centigrade"));
            break;

        case MENU_ALTITUDE:
            _lcd->print(F("Altitude"));
            break;

        case MENU_WIFI:
            _lcd->print(F("WIFI"));
            break;

        case MENU_EXIT:
            _lcd->print(F("Exit"));
            break;
            */
    }    

    _lcd->fillRect(0, 11, 128, 53, BLACK);
}

void Settings::ResetMaxMin()
{
    _stats->ResetMaxMin();

    _lcd->setTextColor(WHITE);
    _lcd->setCursor(10,30);
    _lcd->print("Max amd min values");
    _lcd->setCursor(19,40);
    _lcd->print("have been reset");
    _lcd->display();

    Interval shortDelay(5000, false);
    while (!shortDelay.Ready() && !_middleButton->State() == BUTTON_CLICKED)
    {
        _stats->RecordStats(_clockTime);
    }
}