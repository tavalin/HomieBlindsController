#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <Arduino.h>

///////////////////////////////////////////////
// Homie
///////////////////////////////////////////////
#define FW_NAME "awesome-blind-controller"
#define FW_VERSION "v0.0.1"

///////////////////////////////////////////////
// FastLED
///////////////////////////////////////////////
#define DATA_PIN D3     // GPIO0 - D3                    
#define NUM_LEDS 24
#define LED_TYPE WS2812
#define BREATH_MIN_BRIGHTNESS  8
#define BREATH_MAX_BRIGHTNESS  48
#define COLOUR_ORDER GRB

///////////////////////////////////////////////
// Notifcation colours
///////////////////////////////////////////////
#define CONFIG_COLOUR CRGB::DarkOrange
#define IDLE_COLOUR CRGB::Green
#define MOVING_COLOUR CRGB::Yellow 
#define DISCONNECTED_COLOUR CRGB::Red

///////////////////////////////////////////////
//Motor
///////////////////////////////////////////////
#define MOTOR_IN1 5 // GPIO5 - D1
#define MOTOR_IN2 4 // GPIO4 - D2
#define ENC_PIN1 12 // GPIO12 - D6
#define ENC_PIN2 14 // GPIO14 - D5

///////////////////////////////////////////////
// Debugging
///////////////////////////////////////////////
//#define DEBUG
#if defined(DEBUG)
#define debug(...) Serial.print(__VA_ARGS__)
#define debugln(...) Serial.println(__VA_ARGS__)
#else
#define debug(...)
#define debugln(...)
#endif

#endif
