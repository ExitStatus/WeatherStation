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

void CardLogger::Record(RtcDateTime dateStamp, float temperature, float humidity, float pressure)
{
    char *filename = new char[32];
    sprintf(filename, "%d.csv", dateStamp.Year());
    File dataFile = SD.open(filename, FILE_WRITE);

    if (dataFile)
    {
        dataFile.printf("%d-%d-%dT%d:%d:%d,%f,%f,%f\r\n", 
            dateStamp.Year(), dateStamp.Month(), dateStamp.Day(), 
            dateStamp.Hour(), dateStamp.Minute(), dateStamp.Second(),
            temperature, humidity, pressure);

        dataFile.close();
    }

    delete filename;
}