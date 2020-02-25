#ifndef Stats_h
#define Stats_h

#include <Arduino.h>
#include "My_BMP280.h"
#include "My_AHT10.h"
#include "Interval.h"
#include "Tiny_SH1106.h"
#include "Storage.h"
#include "LCDFont.h"

#include "Hardware.h"
#include "ClockTime.h"

static const unsigned char PROGMEM degreeIcon[] = {
  B00110000,
  B01001000,
  B01001000,
  B00110000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};

static const unsigned char PROGMEM thermometerIcon[] =
{ B00000011, B10000000,
  B00000100, B01011110,
  B00000100, B01000000,
  B00000100, B01011110,
  B00000100, B01000000,
  B00000101, B01011100,
  B00000101, B01000000,
  B00000101, B01011000,
  B00000101, B01000000,
  B00000101, B01000000,
  B00001001, B00100000,
  B00010011, B10010000,
  B00010011, B10010000,
  B00001001, B00100000,
  B00000100, B01000000,
  B00000011, B10000000 
};

static const unsigned char PROGMEM dripIcon[] =
{ B00000001, B00000000,
  B00000001, B00000000,
  B00000010, B10000000,
  B00000100, B01000000,
  B00000100, B01000000,
  B00001000, B00100000,
  B00010000, B00010000,
  B00010000, B00010000,
  B00100000, B00001000,
  B00100000, B00101000,
  B00100000, B00101000,
  B00100000, B00101000,
  B00100000, B01001000,
  B00010000, B10010000,
  B00001000, B00100000,
  B00000111, B11000000 
};

static const unsigned char PROGMEM pressureIcon[] =
{ B00000011, B11000000,
  B00001110, B01110000,
  B00011000, B00011000,
  B00110010, B00001100,
  B00100000, B01100100,
  B01101000, B11100110,
  B01000001, B11000010,
  B01000001, B10000010,
  B01101000, B00010110,
  B00100000, B00000100,
  B00110010, B01001100,
  B00011000, B00011000,
  B00001100, B00110000,
  B00000111, B11100000,
  B00000001, B10000000,
  B00011111, B11111000 
};


class Stats
{
  private:
    Interval *_timer;
    Interval *_minuteTimer;
    Interval *_hourTimer;
    Interval *_dayTimer;
    Tiny_SH1106 *_lcd;
    MyAHT10 *_AHT10;
    My_BMP280 *_bmpSensor;
    uint8_t _mode = 0;
    Storage _data;
    float _currentTemperature;
    float _currentHumidity;
    float _currentPressure;
    float _currentDewPoint;

    void RenderStyle0(float temp, float humid, float dew, float pressure);
    void RenderStyle1(float temp, float humid, float dew, float pressure);
    void RenderStyle2(float temp, float humid, float dew, float pressure);
    void RenderStyle3(float temp, float humid, float dew, float pressure);
    void RenderStyle4(float temp, float humid, float dew, float pressure);

    void printTemp(int x, int y, char *prefix, int temp);
    void printTemp(int x, int y, char *prefix, float temp);
    void printHumidity(int x, int y, char *prefix, float temp);
    void printPressure(int x, int y, char *prefix, float temp);
  public: 
    Stats(Tiny_SH1106 *lcd);
    ~Stats();
    void SetMode(int mode);
    void Render(ClockTime *clockTime);
    void RecordStats(ClockTime *clockTime);
    void ResetMaxMin();
    void WaitForInit();
};

#endif
