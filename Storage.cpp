#include "Storage.h"

#define HAS_MAXMIN_OFFSET       80
#define MAX_TEMPERATURE_OFFSET  81
#define MIN_TEMPERATURE_OFFSET  82

#define MAX_HUMIDITY_OFFSET     83
#define MIN_HUMIDITY_OFFSET     84

#define MAX_PRESSURE_OFFSET     85
#define MIN_PRESSURE_OFFSET     87

Storage::Storage()
{
    _seconds = Allocate(12);
    _minutes = Allocate(60);
    _hours = Allocate(HOURS_STORED);
    _days = Allocate(DAYS_STORED);

    _logger = new CardLogger();

    EEPROM_Init();

    _logger->ReadSensorData(0, _hours, HOURS_STORED);
    _logger->ReadSensorData(1, _days, DAYS_STORED);
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

void Storage::Minute(ClockTime *clockTime, float temperature, float humidity, float pressure)
{
    SensorData *sum = Cascade(_seconds, 12, _minutes, 60, 12);

    if (sum == NULL)
    {
        Serial.println(F("Unabled to find any seconds data"));
        return;
    }

    _logger->Record(clockTime, temperature, humidity, pressure);
}

void Storage::Hour()
{
    SensorData *sum = Cascade(_minutes, 60, _hours, HOURS_STORED, 60);

    if (sum == NULL)
    {
        Serial.println(F("Unabled to find any minute data"));
        return;
    }

    _logger->WriteSensorData(0, _hours, HOURS_STORED);
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

    _logger->WriteSensorData(1, _days, DAYS_STORED);
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
}

void Storage::SetMaxMin(float temperature, float humidity, float pressure)
{
    if (!_hasMaxMin)
    {
        if (EEPROM.read(HAS_MAXMIN_OFFSET) == 0)
        {
            _maxTemperature = _minTemperature = temperature;
            _maxHumidity = _minHumidity = humidity;
            _maxPressure = _minPressure = pressure;

            EEPROM.write(HAS_MAXMIN_OFFSET, 0xff);
            EEPROM.put(HAS_MAXMIN_OFFSET + 1, _maxTemperature);
            EEPROM.put(HAS_MAXMIN_OFFSET + 1 + sizeof(float), _minTemperature);
            EEPROM.put(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 2), _maxHumidity);
            EEPROM.put(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 3), _minHumidity);
            EEPROM.put(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 4), _maxPressure);
            EEPROM.put(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 5), _minPressure);
        }
        else
        {
            EEPROM.get(HAS_MAXMIN_OFFSET + 1, _maxTemperature);
            EEPROM.get(HAS_MAXMIN_OFFSET + 1 + sizeof(float), _minTemperature);
            EEPROM.get(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 2), _maxHumidity);
            EEPROM.get(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 3), _minHumidity);
            EEPROM.get(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 4), _maxPressure);
            EEPROM.get(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 5), _minPressure);

            Serial.print(F("EEPROM: Temperature: "));
            Serial.print(_maxTemperature);
            Serial.print(" / ");
            Serial.println(_minTemperature);

            Serial.print(F("EEPROM: Humidity: "));
            Serial.print(_maxHumidity);
            Serial.print(" / ");
            Serial.println(_minHumidity);

            Serial.print(F("EEPROM: Pressure: "));
            Serial.print(_maxPressure);
            Serial.print(" / ");
            Serial.println(_minPressure);
        }

        EEPROM.commit();
        _hasMaxMin = true;

        return;
    }

    if (temperature > _maxTemperature)
    {
        _maxTemperature = temperature;
        EEPROM.put(HAS_MAXMIN_OFFSET + 1, _maxTemperature);
        EEPROM.commit();
    }

    if (temperature < _minTemperature)
    {
        _minTemperature = temperature;
        EEPROM.put(HAS_MAXMIN_OFFSET + 1 + sizeof(float), _minTemperature);
        EEPROM.commit();
    }

    if (humidity > _maxHumidity)
    {
        _maxHumidity = humidity;
        EEPROM.put(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 2), _maxHumidity);
        EEPROM.commit();
    }

    if (humidity < _minHumidity)
    {
        _minHumidity = humidity;
        EEPROM.put(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 3), _minHumidity);
        EEPROM.commit();
    }

    if (pressure > _maxPressure)
    {
        _maxPressure = pressure;
        EEPROM.put(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 4), _maxPressure);
        EEPROM.commit();
    }

    if (pressure < _minPressure)
    {
        _minPressure = pressure;
        EEPROM.put(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 5), _minPressure);
        EEPROM.commit();
    }
}

float Storage::GetMaxTemperature()
{
    return _maxTemperature;
}

float Storage::GetMaxHumidity()
{
    return _maxHumidity;
}

float Storage::GetMaxPressure()
{
    return _maxPressure;
}

float Storage::GetMinTemperature()
{
    return _minTemperature;
}

float Storage::GetMinHumidity()
{
    return _minHumidity;
}

float Storage::GetMinPressure()
{
    return _minPressure;
}

void Storage::ResetMaxMin(float temperature, float humidity, float pressure)
{
    _maxTemperature = _minTemperature = temperature;
    _maxHumidity = _minHumidity = humidity;
    _maxPressure = _minPressure = pressure;

    EEPROM.write(HAS_MAXMIN_OFFSET, 0xff);
    EEPROM.put(HAS_MAXMIN_OFFSET + 1, _maxTemperature);
    EEPROM.put(HAS_MAXMIN_OFFSET + 1 + sizeof(float), _minTemperature);
    EEPROM.put(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 2), _maxHumidity);
    EEPROM.put(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 3), _minHumidity);
    EEPROM.put(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 4), _maxPressure);
    EEPROM.put(HAS_MAXMIN_OFFSET + 1 + (sizeof(float) * 5), _minPressure);    

    EEPROM.commit();
    _hasMaxMin = true;
}