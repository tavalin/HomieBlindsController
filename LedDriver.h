#ifndef LED_DRIVER_H
#define LED_DRIVER_H
#include <FastLED.h>
#include "Configuration.h"

enum State {
  TRANSITIONING,
  BREATHING
};

class LedDriver
{
  public:
    LedDriver();
    void loop();
    void setColour(CRGB colour);

  private:
    uint8_t maxBrightness;
    uint8_t minBrightness;
    uint8_t blendStep;
    CRGB currentColour;
    CRGB targetColour;
    enum State myState;
    struct CRGB leds[NUM_LEDS];

    void handleBreath();
    void handleTransition();
};



#endif
