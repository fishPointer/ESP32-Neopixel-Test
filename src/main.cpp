#include<Arduino.h>
#include <Adafruit_NeoPixel.h>
#define PIN1        27
#define UPPIXELS 9
#define DELAYVAL  50
Adafruit_NeoPixel upline(UPPIXELS, PIN1, NEO_GRBW + NEO_KHZ800);
void setup()
{
  upline.begin();
  upline.show();
}

int snakeLength = 1;
int snakeSpeed = 100;

int headPos = snakeLength - 1;
int tailPos = 0;

uint32_t lastTime = millis();
uint32_t firstPixelHue = 0;

void loop()
{

if(snakeLength >= UPPIXELS) {snakeLength = UPPIXELS - 1;}


  for(int i=0 ; i<UPPIXELS ; i++)
  {

      int pixelHue = firstPixelHue + (i*65536L/UPPIXELS);
      upline.setPixelColor(i, upline.gamma32(upline.ColorHSV(pixelHue,255,255)));
  
  upline.show();
  firstPixelHue += 32;

  if((millis() - lastTime) > snakeSpeed)
  {
    if(++headPos >= UPPIXELS)
    {
      headPos = 0;
    }
    if(++tailPos >= UPPIXELS)
    {
      tailPos = 0;
    }
    lastTime = millis();
  }

  }

}