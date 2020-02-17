#ifndef SensorData_h
#define SensorData_h

#include <Arduino.h>
#include <EEPROM.h>

class SensorData
{
  public:
    int8_t Temperature;
    uint8_t Humidity;
    uint16_t Pressure;

    SensorData(int8_t temperature, uint8_t humidity, uint16_t pressure);
    void SerialWrite(char *title);
    int EEPROM_Write(int offset);
    int EEPROM_Read(int offset);
};

#endif
