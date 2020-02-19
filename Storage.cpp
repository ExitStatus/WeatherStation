#include "Storage.h"


Storage::Storage()
{
    _seconds = Allocate(12);
    _minutes = Allocate(60);
    _hours = Allocate(HOURS_STORED);
    _days = Allocate(DAYS_STORED);

    _logger = new CardLogger();

    EEPROM_Init();
    EEPROM_ReadStats();
}

Storage::~Storage()
{
    Deallocate(_seconds, 12);
    Deallocate(_minutes, 60);
    Deallocate(_hours, HOURS_STORED);
    Deallocate(_days, DAYS_STORED);

    delete _logger;
}

SensorData** Storage::Allocate(int size)
{
    SensorData **allocData = new SensorData*[size];
    for (int i=0; i<size; i++)
        allocData[i] = NULL;   
    
    return allocData;
}

void Storage::Deallocate(SensorData** allocData, int size)
{
    for (int i=0; i<size; i++)
        if (allocData[i] != NULL)
            delete allocData[i];

    delete[] allocData;
}

SensorData* Storage::Cascade(SensorData **from, int fromSize, SensorData **to, int toSize, int last)
{
    int count = 0;

    uint32_t tempTemp = 0;
    uint32_t tempHumid = 0;
    uint32_t tempPress = 0;

    for (int i=0; i < last; i++)
    {
        int offset = (fromSize - last) + i;

        if (from[offset] != NULL)
        {
            tempTemp += from[offset]->Temperature;
            tempHumid += from[offset]->Humidity;
            tempPress += from[offset]->Pressure;

            count++;
        }
    }

    if (count == 0)
        return NULL;

    SensorData *tail = to[0];
    if (tail != NULL)
        delete tail;

    for (int i=0; i < toSize - 1; i++)
        to[i] = to[i+1];

    to[toSize-1] = new SensorData(tempTemp / count, tempHumid / count, tempPress / count);

    return to[toSize-1];
}

void Storage::Second(int temperature, int humidity, int pressure)
{
    SensorData *tail = _seconds[0];
    if (tail != NULL)
        delete tail;

    for (int i=0; i < 11; i++)
        _seconds[i] = _seconds[i+1];

    _seconds[11] = new SensorData(temperature, humidity, pressure);
}

void Storage::Minute()
{
    SensorData *sum = Cascade(_seconds, 12, _minutes, 60, 12);

    if (sum == NULL)
    {
        Serial.println(F("Unabled to find any seconds data"));
        return;
    }

    //EEPROM_WriteStats();
    sum->SerialWrite("MINUTE");
}

void Storage::Hour()
{
    SensorData *sum = Cascade(_minutes, 60, _hours, HOURS_STORED, 60);

    if (sum == NULL)
    {
        Serial.println(F("Unabled to find any minute data"));
        return;
    }

    EEPROM_WriteStats();
    sum->SerialWrite("HOUR");
}

void Storage::Day()
{
    SensorData *sum = Cascade(_hours, HOURS_STORED, _days, DAYS_STORED, 24);

    if (sum == NULL)
    {
        Serial.println(F("Unabled to find any hours data"));
        return;
    }

    EEPROM_WriteStats();
    sum->SerialWrite("DAY");
}

void Storage::EEPROM_Init()
{
  EEPROM.begin(EEPROM_SIZE);

  Serial.print(F("EEPROM_Init ("));
  Serial.print(EEPROM_SIZE);
  Serial.println(F(" bytes storage size)"));
  
  bool reset = false;

  if (reset || EEPROM.read(0) != 0xbe && EEPROM.read(1) != 0xef)
  {
    EEPROM_Clear();
    
    EEPROM.write(0, 0xbe);
    EEPROM.write(1, 0xef);
    EEPROM.commit();

    Serial.println(F("EEPROM BOM Written"));
  }
  else
    Serial.println(F("EEPROM BOM Found"));
}

void Storage::EEPROM_Clear()
{
    for (int i = 0 ; i < 100 ; i++) 
        EEPROM.write(i, 0);

    for (int i=0; i<HOURS_STORED + DAYS_STORED; i++)
        EEPROM.write(i+100, 0xff);
}

int Storage::EEPROM_WriteSection(SensorData **data, int nItems, int offset, char *prefix)
{
    for (int i=0; i<nItems; i++)
    {
        if (data[i] == NULL)
        {
            EEPROM.write(offset, 0xff);
            offset++;
        }
        else
            offset += data[i]->EEPROM_Write(offset);
    }

    return offset;
}

int Storage::EEPROM_ReadSection(SensorData **data, int nItems, int offset, char *prefix)
{
    int z = 0;

    for (int i=0; i<nItems; i++)
    {
        if (EEPROM.read(offset) == 0xff)
        {
            data[i] = NULL;
            offset++;
        }
        else
        {
            SensorData *itm = new SensorData(0,0,0);
            offset += itm->EEPROM_Read(offset);
            data[i] = itm;
            z++;
        }       
    }

    Serial.print(prefix);
    Serial.print(" loaded ");
    Serial.println(z);

    return offset;
}

void Storage::EEPROM_WriteStats()
{
    uint32_t offset = 100;

    //offset = EEPROM_WriteSection(_minutes, 60, offset, "EEPROM_WriteStats MINUTE");
    offset = EEPROM_WriteSection(_hours, HOURS_STORED, offset, "EEPROM_WriteStats HOUR");
    offset = EEPROM_WriteSection(_days, DAYS_STORED, offset, "EEPROM_WriteStats DAY");

    EEPROM.commit();
}

void Storage::EEPROM_ReadStats()
{
    uint32_t offset = 100;

    //offset = EEPROM_ReadSection(_minutes, 60, offset, "EEPROM_ReadStats MINUTE");
    offset = EEPROM_ReadSection(_hours, HOURS_STORED, offset, "EEPROM_ReadStats HOUR");
    offset = EEPROM_ReadSection(_days, DAYS_STORED, offset, "EEPROM_ReadStats DAY");
}