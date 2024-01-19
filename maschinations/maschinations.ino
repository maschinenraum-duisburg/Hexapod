#include <Adafruit_NeoPixel.h>

const int LED_PINS[] = {2,3,4,5,6,7};            //LED port definitions
const int LEDS_COUNT = 8;           //Number of leds per leg
Adafruit_NeoPixel LEDS_LEG[6];
const int MAX_LEGS = sizeof(LEDS_LEG) / sizeof(LEDS_LEG[0]);
const int FRAME_TIME_MS = 20;         //frame time (20msec = 50Hz)
const int IDLE_TIME_MS = 4000;         //Idletimer dauer

int animParams[10];     //[anzahl parameter, animationsmodus, param 1, ... , param n]
int animationStep = 0; // Aktueller Schritt der Animation
int animationLastStep = 0; //letzter Schritt der Animation
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
    if((millis() - lastAction) > IDLE_TIME_MS)      //ideanimation
    {     
      setArray(animParams,1,98); 
      animate(animParams); 
    } 
  } 

}


