#ifndef Storage_h
#define Storage_h

#include <EEPROM.h>
#include "SensorData.h"

#define MINUTES_STORED  60
#define DAYS_STORED  96
#define HOURS_STORED  96

#define EEPROM_SIZE  1500

class Storage
{
  private:
    SensorData **_seconds;
    SensorData **_minutes;
    SensorData **_hours;
    SensorData **_days;

    SensorData** Allocate(int size);
    void Deallocate(SensorData** allocData, int size);
    SensorData* Cascade(SensorData **from, int fromSize, SensorData **to, int toSize, int last);

    void EEPROM_Init();
    void EEPROM_Clear();
    void EEPROM_WriteStats();
    void EEPROM_ReadStats();
    int EEPROM_WriteSection(SensorData **data, int nItems, int offset, char *prefix);
    int EEPROM_ReadSection(SensorData **data, int nItems, int offset, char *prefix);

  public: 
    Storage();
    ~Storage();

    void Second(int temperature, int humidity, int pressure);
    void Minute();
    void Hour();
    void Day();
};

#endif
