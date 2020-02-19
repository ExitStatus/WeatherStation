#include "SensorData.h"

SensorData::SensorData(int8_t temperature, uint8_t humidity, uint16_t pressure)
{
    Temperature = temperature;
    Humidity = humidity;
    Pressure = pressure;
}

void SensorData::SerialWrite(char *title)
{
    Serial.print(title);
    Serial.print(": Temperature=");
    Serial.print(Temperature);
    Serial.print(" Humidity=");
    Serial.print(Humidity);
    Serial.print(" Pressure=");
    Serial.println(Pressure);
}

int SensorData::EEPROM_Write(int offset)
{
    EEPROM.put(offset, *this);
    return sizeof(*this);
}

int SensorData::EEPROM_Read(int offset)
{
   EEPROM.get(offset, *this);
   return sizeof(*this);
}