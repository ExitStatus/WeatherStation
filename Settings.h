#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "Interval.h"
#include "Tiny_SH1106.h"
#include "Button.h"

class Settings
{
    private:
        Tiny_SH1106 *_lcd;
        Button *_leftButton; 
        Button *_middleButton; 
        Button *_rightButton; 

    public:
        Settings(Tiny_SH1106 *lcd, Button *leftButton, Button *middleButton, Button *rightButton);
        void UpdateSettingsUI();
};

#endif