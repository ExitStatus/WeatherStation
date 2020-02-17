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
    /*
    int nOffset = offset;

    char buffer[100];

    sprintf(buffer, "EEPROM Write temp %d to offset %d", Temperature, nOffset);
    Serial.println(buffer);
    EEPROM.put(nOffset, Temperature);
    nOffset += sizeof(Temperature);

    sprintf(buffer, "EEPROM Write humid %d to offset %d", Humidity, nOffset);
    Serial.println(buffer);
    EEPROM.put(nOffset, Humidity);
    nOffset += sizeof(Humidity);

    sprintf(buffer, "EEPROM Write pressure %d to offset %d", Pressure, nOffset);
    Serial.println(buffer);
    EEPROM.put(nOffset, Pressure);
    nOffset += sizeof(Pressure);

    return nOffset - offset;
    */

   EEPROM.put(offset, *this);
   return sizeof(*this);
}

int SensorData::EEPROM_Read(int offset)
{
    /*
    int nOffset = offset;

    Temperature = EEPROM.read(nOffset);
    nOffset += sizeof(Temperature);

    Humidity = EEPROM.read(nOffset);
    nOffset += sizeof(Humidity);

    Pressure = EEPROM.read(nOffset);
    nOffset += sizeof(Pressure);

    return nOffset - offset;
    */

   EEPROM.get(offset, *this);
   return sizeof(*this);
}