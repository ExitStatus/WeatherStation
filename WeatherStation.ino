                                                                                                                                                                                                                                                                                                                                                      #include <Wire.h>
//#//include "Tiny_SH1106.h"

#include <Wire.h>

#include "Stats.h"
#include "ClockTime.h"
#include "Button.h"
#include "Hardware.h"

#define OLED_RESET 4
Tiny_SH1106 lcd(OLED_RESET);

Stats *stats;
ClockTime *clockTime;
Button *leftButton; 
Button *middleButton; 
Button *rightButton; 
int mode = 0;

void setup() 
{
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  // SDA, SCL
  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(I2C_SPEED);

  stats = new Stats(&lcd);

  lcd.begin(SH1106_SWITCHCAPVCC, I2C_OLED_ADDRESS);
  lcd.clearDisplay();
  lcd.display();

  clockTime = new ClockTime(&lcd);
  leftButton = new Button(PIN_LEFT);
  middleButton = new Button(PIN_MIDDLE);
  rightButton = new Button(PIN_RIGHT);
}

void loop() 
{
  if (rightButton->Clicked())
  {
    Serial.println("Right button click");
    NextMode();
  }

  if (leftButton->Clicked())
  {
    Serial.println("Left button click");
    PrevMode();
  }

  if (middleButton->Clicked())
  {
    Serial.println("Middle button click");
  }

  RtcDateTime now = clockTime->Render();
  stats->Render(now);
}

void NextMode()
{
  if (++mode > 4)
    mode = 0;

  clockTime->SetMode(mode);
  stats->SetMode(mode);
}

void PrevMode()
{
  if (mode == 0)
    mode = 4;
  else
    mode--;
  
  clockTime->SetMode(mode);
  stats->SetMode(mode);
}