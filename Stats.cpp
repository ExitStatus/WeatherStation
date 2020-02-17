#include "Stats.h"

Stats::Stats(Tiny_SH1106 *lcd)
{
  _lcd = lcd;

  _AHT10 = new MyAHT10();
  if (!_AHT10->begin(eAHT10Address_default))
  {
    Serial.println(F("An error occurred initializing AHT10"));
    return;
  }

  _bmpSensor = new My_BMP280(&Wire);

  if (!_bmpSensor->begin(0x76)) {
    Serial.println(F("An error occurred initializing BMP280"));
    while (1);
  }

  /* Default settings from datasheet. */
  _bmpSensor->setSampling(My_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  My_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  My_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  My_BMP280::FILTER_X16,      /* Filtering. */
                  My_BMP280::STANDBY_MS_500); /* Standby time. */

  _timer = new Interval();
  _minuteTimer = new Interval();
  _hourTimer = new Interval();
  _dayTimer = new Interval();

  _timer->Start(5000, true);

  _minuteTimer->Start(60000, false);
  _hourTimer->Start(3600000, false);
  _dayTimer->Start(86400000, false);

/*
  _minuteTimer->Start(15000, false);
  _hourTimer->Start(30000, false);
  _dayTimer->Start(60000, false);
  */
}

Stats::~Stats()
{
  delete _timer;
  delete _minuteTimer;
  delete _hourTimer;
  delete _dayTimer;
  delete _AHT10;
  delete _bmpSensor;
}

void Stats::NextMode()
{
  if (++_mode > 1)
    _mode = 0;

  _timer->Now();
}

void Stats::PrevMode()
{
  if (_mode == 0)
    _mode = 1;
  else
    _mode--;
  
  _timer->Now();
}

void Stats::Render()
{
  if (!_timer->Ready())
      return;

  _lcd->setFont(NULL);
  _lcd->setTextSize(1);
  _lcd->setTextColor(WHITE);
  
  _lcd->fillRect(0, 38, 128, 26, BLACK);   

  float temp = _AHT10->GetTemperature();
  float dew = _AHT10->GetDewPoint();
  float humid = _AHT10->GetHumidity();

  if (isnan(temp) || isnan(dew) || isnan(humid)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float pressure = _bmpSensor->readPressure() / 100;
  if (isnan(pressure)) {
    Serial.println(F("Failed to read from BMP280 sensor!"));
    return;
  }

  switch (_mode)
  {
    case 0: RenderStyle0(temp, humid, dew, pressure); break;
    case 1: RenderStyle1(temp, humid, dew, pressure); break;
  }

  _lcd->display();

  _data.Second((int)temp, (int)humid, (int)pressure);

  if (_minuteTimer->Ready())
    _data.Minute();

  if (_hourTimer->Ready())
    _data.Hour();

  if (_dayTimer->Ready())
    _data.Day();
}

void Stats::printTemp(int x, int y, char *prefix, int temp)
{
  _lcd->setCursor(x,y);

  char *buffer = new char[6];
  sprintf(buffer, "%s%d\0", prefix, temp);

  _lcd->print(buffer);
  _lcd->print(F(" C"));

  _lcd->drawBitmap(x + (strlen(buffer) * 6), y,  degreeIcon, 8, 8, WHITE);
}

void Stats::RenderStyle0(float temp, float humid, float dew, float pressure)
{
  _lcd->drawBitmap(20, 38, thermometerIcon, 16, 16, 1);
  _lcd->drawBitmap(56, 38,  dripIcon, 16, 16, 1);
  _lcd->drawBitmap(92, 38,  pressureIcon, 16, 16, 1);

  char *buffer = new char[6];
  sprintf(buffer, "%d C\0", (int)temp);
    
  if ((int)temp < 10)
  {
    int x = 27 - ((3 * 6)/2);
    _lcd->setCursor(x,57);
    _lcd->print(buffer);
    _lcd->drawBitmap(x+7, 57,  degreeIcon, 8, 8, WHITE);
  }
  else
  {
    int x = 27 - ((4 * 5)/2);
    _lcd->setCursor(x,57);
    _lcd->print(buffer);
    _lcd->drawBitmap(x+12,57,  degreeIcon, 8, 8, WHITE);
  }

  sprintf(buffer, "%d%%\0", (int)humid);
  _lcd->setCursor(64 - ((strlen(buffer) * 6) / 2), 57);
  _lcd->print(buffer); 

  sprintf(buffer, "%d\0", (int)pressure);
  _lcd->setCursor(100 - ((strlen(buffer) * 6) / 2), 57);
  _lcd->print(buffer); 

  free(buffer);
}

void Stats::RenderStyle1(float temp, float humid, float dew, float pressure)
{
  _lcd->drawFastHLine(0, 38, 128, WHITE);

  printTemp(0, 44, " Temp:", (int)temp);
  printTemp(66, 44, "DewP:", (int)dew);
  
  _lcd->setCursor(0,56);
  _lcd->print(F("Humid:"));
  _lcd->print((int)humid);
  _lcd->print(F("%"));

  _lcd->setCursor(66,56);
  _lcd->print(F("Mbar:"));
  _lcd->print((int)pressure);
}