#ifndef FPS_H
#define FPS_H
#include "Configuration.h"
long currentMillis = 0;
long lastMillis = 0;
int loops = 0;

void showfps() {
  currentMillis = millis();
  loops++;
  if (currentMillis - lastMillis > 1000) {
    Serial.print("[showfps] ");
    Serial.println(loops);
    lastMillis = currentMillis;
    loops = 0;
  }
}
#endif
