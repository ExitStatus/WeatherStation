#ifndef ClockTime_h
#define ClockTime_h

#include <time.h>
#include <string.h>

#include "Arduino.h"
#include "Interval.h"
#include "Tiny_SH1106.h"
#include <Wire.h>
#include <RtcDS3231.h>
#include "LCDFont.h"

class ClockTime
{
  private:
    Interval *_timer;
    Tiny_SH1106 *_lcd;
    RtcDS3231<TwoWire> *_rtc;
    uint32_t _now = 1581162788;
    bool _clockDots = false;
    int _mode = 0;

  public: 
    ClockTime(Tiny_SH1106 *lcd);
    ~ClockTime();

    void Render();
    void SetMode(int mode);
};

#endif
