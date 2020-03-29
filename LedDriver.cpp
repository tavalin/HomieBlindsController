#include <FastLED.h>
#include "LedDriver.h"
#if defined(DEBUG)
#include "fps.h"
#endif
#include "fps.h"

LedDriver::LedDriver() {
  debugln("[LedDriver::LedDriver()] start");

  minBrightness = BREATH_MIN_BRIGHTNESS;
  maxBrightness = BREATH_MAX_BRIGHTNESS;

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOUR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOUR_ORDER>(leds, NUM_LEDS).setCorrection(UncorrectedColor);

  currentColour = CRGB::Black;
  targetColour = CRGB::Black;

  blendStep = 0;

  debugln("[LedDriver::LedDriver()] end");
}

void LedDriver::loop() {

  debugln("[LedDriver::loop()] start");
  showfps();
#if defined(DEBUG)
  showfps();
#endif

  if (myState == TRANSITIONING) {
    handleTransition();
  }

  handleBreath();
  FastLED.show();

  debugln("[LedDriver::loop()] end");

}
void LedDriver::setColour(CRGB colour) {
  debugln("[LedDriver::setColour()] start");
  myState = TRANSITIONING;
  targetColour = colour;
  debugln("[LedDriver::setColour()] end");
}

void LedDriver::handleTransition() {
  debugln("[LedDriver::handleTransition()] start");
  if (currentColour != targetColour) {
    CRGB interim = blend(currentColour, targetColour, blendStep++);
    debugln("[LedDriver::handleTransition()] " + "r: " + String(interim.red) + ", g: " + String(interim.green) + ", b: " + String(interim.blue) + ", step: " + String(blendStep));
    fill_solid(leds, NUM_LEDS, interim);
    FastLED.delay(1);
    if (interim == targetColour) {
      debugln("[LedDriver::handleTransition()] reached target colour");
      currentColour = targetColour;
      blendStep = 0; // we have finished blend - rest to 0
      myState = BREATHING;
    }
  }
  debugln("[LedDriver::handleTransition()] end");
}

void LedDriver::handleBreath() {
  debugln("[LedDriver::handleBreath()] start");
  long mil = millis();
  float breath = (exp(sin(mil / 2000.0 * PI)) - 0.36787944) * 108.0;
  breath = map (breath, 0, 255, minBrightness, maxBrightness);
  FastLED.setBrightness(breath);
  debugln("[LedDriver::handleBreath()] end");
}
