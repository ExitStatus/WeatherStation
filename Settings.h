#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "Interval.h"
#include "Tiny_SH1106.h"
#include "Interval.h"
#include "Stats.h"
#include "ClockTime.h"
#include "Button.h"

class Settings
{
    private:
        Tiny_SH1106 *_lcd;
        Button *_leftButton; 
        Button *_middleButton; 
        Button *_rightButton;
        Stats *_stats;
        ClockTime *_clockTime;

        void DrawMenu(int firstItem, int selectedItem);
        void ChangeSetting(int selectedItem);
        void ResetMaxMin();

    public:
        Settings(Tiny_SH1106 *lcd, Button *leftButton, Button *middleButton, Button *rightButton, Stats *stats, ClockTime *clockTime);
        void UpdateSettingsUI();

};

#endif