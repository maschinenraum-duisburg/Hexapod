#ifndef machinations_h
  #define maschinations_h

  #include <Adafruit_NeoPixel.h>

  class Maschination {
    public:
      Maschinations(Adafruit_NeoPixel* leds, int ledCount, int maxLegs);
      void bootup(int params[]);
      void carousel(int params[]);
      void idleAnimation();
      void turnOffAll();
      bool isAnimationRunning();

    private:
      Adafruit_NeoPixel* _leds;
      int _ledCount;
      int _maxLegs;
      bool _isRunning;
      uint32_t Wheel(byte WheelPos, int n);
  };

#endif



