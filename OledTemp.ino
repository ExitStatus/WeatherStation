                                                                                                                                                                                                                                                                                                                                                      #include <Wire.h>
//#//include "Tiny_SH1106.h"

#include <Wire.h>

#include "Stats.h"
#include "ClockTime.h"
#include "Button.h"

#define OLED_RESET 4
Tiny_SH1106 lcd(OLED_RESET);

Stats *stats;
ClockTime *clockTime;
Button *leftButton; 
Button *middleButton; 
Button *rightButton; 

void setup() 
{
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  // SDA, SCL
  Wire.begin(25,26);
  Wire.setClock(1000000);

  stats = new Stats(&lcd);
  
  lcd.begin(SH1106_SWITCHCAPVCC, 0x3C);
  lcd.clearDisplay();
  lcd.display();

  clockTime = new ClockTime(&lcd);
  leftButton = new Button(12);
  middleButton = new Button(14);
  rightButton = new Button(27);
}

void loop() 
{
  if (rightButton->Clicked())
  {
    Serial.println("Right button click");
    stats->NextMode();
  }

  if (leftButton->Clicked())
  {
    Serial.println("Left button click");
    stats->PrevMode();
  }

  if (middleButton->Clicked())
  {
    Serial.println("Middle button click");
  }

  clockTime->Render();
  stats->Render();

  delay(100);
}
