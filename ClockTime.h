#ifndef ClockTime_h
#define ClockTime_h

#include <time.h>
#include <string.h>

#include "Arduino.h"
#include "Interval.h"
#include "Tiny_SH1106.h"
#include <Wire.h>
#include <RtcDS3231.h>
#include <Fonts/FreeMonoBold12pt7b.h>

class ClockTime
{
  private:
    Interval *_timer;
    Tiny_SH1106 *_lcd;
    RtcDS3231<TwoWire> *_rtc;
    uint32_t _now = 1581162788;
    bool _clockDots = false;

  public: 
    ClockTime(Tiny_SH1106 *lcd);
    ~ClockTime();

    void Render();
};

#endif
