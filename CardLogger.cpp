#include "CardLogger.h"

CardLogger::CardLogger()
{   
    if (!SD.begin(SPI_CS_SDCARD))
    {
        Serial.println(F("Unable to initialize SDCARD - Logging disabled"));
        return;
    }

    _enabled = true;

    _cardType = SD.cardType();
    if (_cardType == CARD_NONE)
    {
      Serial.println(F("No SD card attached"));
      return;
    }

    Serial.print(F("SD Card Type: "));
    switch (_cardType)
    {
        case CARD_MMC: Serial.println(F("MMC")); break;
        case CARD_SD: Serial.println(F("SDSC")); break;
        case CARD_SDHC: Serial.println(F("SDHC")); break;
        default: Serial.println(F("Unknown")); break;
    }

    _cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", _cardSize);
}

void CardLogger::Record(ClockTime *clockTime, float temperature, float humidity, float pressure)
{
    char *iso = new char[64];

    clockTime->GetIso8601(iso);
    if (iso == NULL)
    {
        Serial.println(F("Failed to get ISO8601 time - No logging available"));
        return;
    }

    char *year = new char[5];
    memcpy(year, iso, 4);
    year[4] = '\0';

    char *filename = new char[32];
    sprintf(filename, "/%s.csv", year);
    delete[] year;

    File dataFile = SD.open(filename, FILE_APPEND);

    if (dataFile)
    {
        char *buffer = new char[64];

        sprintf(buffer, "%s,%f,%f,%f\0", iso, temperature, humidity, pressure);

        dataFile.println(buffer);
        dataFile.close();

        delete[] buffer;
    }
    else
    {
        Serial.println(F("Failed to open logging file"));
    }
    

    delete[] filename;
    delete[] iso;
}

void CardLogger::WriteSensorData(uint8_t type, SensorData **data, int nItems)
{
    char *filename = "/hour.bin";

    if (type == 1)
        filename = "/day.bin";

    Serial.print("Writing stats to ");
    Serial.println(filename);

    File dataFile = SD.open(filename, FILE_WRITE);
    if (!dataFile)
    {
        Serial.println(F("Failed to open file to write stats"));
        return;
    }

    for (int i=0; i<nItems; i++)
    {
        if (data[i] == NULL)
            dataFile.write(0x00);
        else
        {
            dataFile.write(0xff);
            dataFile.write((uint8_t*)data[i], sizeof(SensorData));
        }
    }

    dataFile.close();
}

void CardLogger::ReadSensorData(uint8_t type, SensorData **data, int nItems)
{
    char *filename = "/hour.bin";

    if (type == 1)
        filename = "/day.bin";

    Serial.print("Reading stats from ");
    Serial.println(filename);

    File dataFile = SD.open(filename, FILE_READ);
    if (!dataFile)
    {
        Serial.println(F("Failed to open file to read stats"));
        return;
    }

    for (int i=0; i<nItems; i++)
    {
        if (dataFile.read() == 0x00)
            data[i] = NULL;
        else
        {
            SensorData *itm = new SensorData(0,0,0);
            dataFile.read((uint8_t*)itm, sizeof(SensorData));
            data[i] = itm;
        }
    }

    dataFile.close();
}