#include <Adafruit_NeoPixel.h>

const int LED_PINS[] = {2,3,4,5,6,7};            //LED port definitions
const int LEDS_COUNT = 8;           //Number of leds per leg
Adafruit_NeoPixel LEDS_LEG[6];
const int MAX_LEGS = sizeof(LEDS_LEG) / sizeof(LEDS_LEG[0]);
const int FRAME_TIME_MS = 20;         //frame time (20msec = 50Hz)
const int IDLE_TIME_MS = 4000;         //Idletimer dauer

int animParams[10];     //[anzahl parameter, animationsmodus, param 1, ... , param n]
int animationStep = 0; // Aktueller Schritt der Animation
bool booted = false;

unsigned long previousTime;               //timerVariable Loop
unsigned long previousAnimTime;           //timerVariable animation
unsigned long lastAction;           //timerVariable Idlestate

int demo = 0;                         //only for animationtestdemo
int buttonPressed = 0;  
static unsigned long lastButtonPressTime = 0;
const unsigned long debounceDelay = 200; 
int lastBtnState = LOW;                       //Initialwert



void setup() {
  Serial.begin(115200);

  pinMode(8, INPUT_PULLUP); //demobutton
  
  //start Leds
  for(int i=0; i<MAX_LEGS; i++)
  {
    LEDS_LEG[i] = Adafruit_NeoPixel(LEDS_COUNT, LED_PINS[i], NEO_GRB + NEO_KHZ800);
    LEDS_LEG[i].setBrightness(32);
    LEDS_LEG[i].begin();
    LEDS_LEG[i].show();
  }

  lastAction = millis();
}

//LED Animationen
//params[0] = animationstyle
//params[1] = Farbe R
//params[2] = Farbe G
//params[3] = Farbe B
//params[4] = delay
//params[3] = Farbe B

//params[n] = Platzhalte für andere Werte, wie Dauer, Farbe, Helligkeit usw.
//Color = LED-COLOR in RGB LEDS_LEG[n].Color(R, G, B)
//LEDS_COUNT = 8; -> Max LEDS
//MAX_LEGS = 6;

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

void animate(int params[]){

  switch (params[0]){
    case 0: //bootup Animation  [R,G,B,delay]
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
    break;

    case 1: //Carousel Animation  [R,G,B,delay]
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
    break;

    case 98: //Idleanimation
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
    break;

    case 99: //Turn off all LEDS
      for (int n = 0; n < MAX_LEGS; n++) {    
          LEDS_LEG[n].clear();
          LEDS_LEG[n].show();
      }
    break;
  }
}

void setArray(int params[], int numArgs, ...) {
  va_list args;
  va_start(args, numArgs);

  for (int i = 0; i < numArgs; i++) {
    params[i] = va_arg(args, int);
  }

  va_end(args);
}

void playdemo(int i)
{
  setArray(animParams,1,99);
  animate(animParams); 
  switch(i){
    case 1:
      setArray(animParams,5,0,255,0,0,50);
      animate(animParams); 
    break;
    case 2:
      setArray(animParams,5,1,0,255,0,100);
      animate(animParams); 
    break;
    default:
      demo = 0;
    break;
  }
}

int button(){
  if (digitalRead(8) == HIGH && lastBtnState == LOW) {
    lastBtnState = HIGH;
    return 1;     
  }
  if (digitalRead(8) == LOW && lastBtnState == HIGH) {
    lastBtnState = LOW;  
    return 0;      
  }

}

void loop(){
  if(button() == 1) {
    lastAction = millis();   
    demo++;
    playdemo(demo); 
  } 
  if((millis() - previousTime) > FRAME_TIME_MS)
  {     
    previousTime = millis(); 
    if(booted != true) {
      setArray(animParams,5,0,255,0,0,50);
      animate(animParams);      
    }
    if(demo >= 3){
      demo= 0;
    }  
    if((millis() - lastAction) > IDLE_TIME_MS)
    {     
      setArray(animParams,1,98); //ideanimation
      animate(animParams); 
    } 
  } 

}


