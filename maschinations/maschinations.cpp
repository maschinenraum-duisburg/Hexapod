#include "maschinations.h"



uint32_t Wheel(byte WheelPos, int n) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return LEDS_LEG[n].Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return LEDS_LEG[n].Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return LEDS_LEG[n].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void animateBootup(int params[]) {
      for (int i = 0; i < LEDS_COUNT; i++) {
        for (int n = 0; n < MAX_LEGS; n++) {   
          LEDS_LEG[n].setPixelColor(i, LEDS_LEG[n].Color(params[1], params[2], params[3]));
          LEDS_LEG[n].show();
        }
        delay(params[4]);
      }

      for (int i = 0; i < LEDS_COUNT; i++) {
        for (int n = 0; n < MAX_LEGS; n++) {   
          LEDS_LEG[n].setPixelColor(i, LEDS_LEG[n].Color(0, 0, 0));
          LEDS_LEG[n].show();
        }
        delay(params[4]);
      }
      lastAction = millis();
      booted = true;
}

void animateCarousel(int params[]) {
  for (int n = 0; n < MAX_LEGS; n++) {   
      for (int i = 0; i < LEDS_COUNT; i++) {
        LEDS_LEG[n].setPixelColor(i, LEDS_LEG[n].Color(params[1], params[2], params[3]));
      }
      LEDS_LEG[n].show();
      delay(params[4]);
    }
    for (int n = 0; n < MAX_LEGS; n++) {   
      for (int i = 0; i < LEDS_COUNT; i++) {
        LEDS_LEG[n].setPixelColor(i, LEDS_LEG[n].Color(0, 0, 0));
      }
      LEDS_LEG[n].show();
      delay(params[4]);
    }
}

void animateRainbow(int params[]){
  while(button() != 1)
      {
        if(millis() - previousAnimTime > 5){
          previousAnimTime = millis();
          for (int n = 0; n < MAX_LEGS; n++) {   
            for (int i = 0; i < LEDS_COUNT; i++) {
              int colorIndex = ((LEDS_COUNT - 1 - i) * 256 / LEDS_COUNT + animationStep) & 255;
              LEDS_LEG[n].setPixelColor(i, Wheel(colorIndex, n));
            }
            LEDS_LEG[n].show();
          }
          animationStep = (animationStep + 1) % 256; // Schritt für den Farbwechsel
        }
      }
}
void animateOff{
    for (int n = 0; n < MAX_LEGS; n++) {    
          LEDS_LEG[n].clear();
          LEDS_LEG[n].show();
      }
}


void animate(int params[]){

  switch (params[0]){
    case 0: //bootup Animation  [R,G,B,delay]

    break;

    case 1: //Carousel Animation  [R,G,B,delay]
      
    break;

    case 98: //Idleanimation
      
    break;

    case 99: //Turn off all LEDS
    
    break;
  }
}
