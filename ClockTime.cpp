#include "ClockTime.h"

ClockTime::ClockTime(Tiny_SH1106 *lcd)
{
  _lcd = lcd;

  _rtc = new RtcDS3231<TwoWire>(Wire);
  _rtc->Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  if (!_rtc->IsDateTimeValid())
  {
    if (_rtc->LastError() != 0)
    {
      Serial.print(F("RTC communications error = "));
      Serial.println(_rtc->LastError());
    }
    else
      _rtc->SetDateTime(compiled);
  }

  if (!_rtc->GetIsRunning())
      _rtc->SetIsRunning(true);
   

  RtcDateTime now = _rtc->GetDateTime();
  if (now < compiled) 
      _rtc->SetDateTime(compiled);

  _rtc->Enable32kHzPin(false);
  _rtc->SetSquareWavePin(DS3231SquareWavePin_ModeNone); 

  _timer = new Interval();
  _timer->Start(1000, true);
}

ClockTime::~ClockTime()
{
  free(_timer);
  free(_rtc);
}

void ClockTime::SetMode(int mode)
{
  _mode = mode;
  _timer->Now();

  Render();
}

void ClockTime::Render()
{
  
  if (_mode > 1 || !_timer->Ready() || !_rtc->IsDateTimeValid())
      return;

  RtcDateTime now = _rtc->GetDateTime();
  _lcd->fillRect(0,0, 127,38, BLACK);
  
  _lcd->setCursor(28,14);
  _lcd->setTextSize(1);
  _lcd->setTextColor(WHITE);

  _lcd->setFont(&DSEG7_Classic_Regular_20);

  char *buffer = new char[6];
  
  if (_clockDots)
    sprintf(buffer, "%02d:%02d", now.Hour(), now.Minute());
  else
    sprintf(buffer, "%02d %02d", now.Hour(), now.Minute());

  _lcd->print(buffer);

  delete buffer;
  
  _clockDots = !_clockDots;  
  
  _lcd->setFont();

  // date

  char *dow;
  switch (now.DayOfWeek())
  {
    case 0 : dow = "Sun"; break;
    case 1 : dow = "Mon"; break;
    case 2 : dow = "Tue"; break;
    case 3 : dow = "Wed"; break;
    case 4 : dow = "Thu"; break;
    case 5 : dow = "Fri"; break;
    case 6 : dow = "Sat"; break;
    default: dow = "???";
  }

  char *mon;
  switch (now.Month())
  {
    case 1 : mon = "Jan"; break;
    case 2 : mon = "Feb"; break;
    case 3 : mon = "Mar"; break;
    case 4 : mon = "Apr"; break;
    case 5 : mon = "May"; break;
    case 6 : mon = "Jun"; break;
    case 7 : mon = "Jul"; break;
    case 8 : mon = "Aug"; break;
    case 9 : mon = "Sep"; break;
    case 10 : mon = "Oct"; break;
    case 11 : mon = "Nov"; break;
    case 12 : mon = "Dec"; break;
    default: mon = "???";
  }

  char *buffer2 = new char[28];
  sprintf(buffer2, "%s, %02d %s %04d\0", dow, now.Day(), mon, now.Year());

  _lcd->setCursor(64 - ((strlen(buffer2) * 6) /2),25);
  _lcd->print(buffer2);
  delete buffer2;

  _lcd->display();
}
