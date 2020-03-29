/*
   Automatic Blind Controller Sketch
   Copyright Daniel Walters
   19 May 2020 - v0.1 - Initial draft

   This sketch allows the automated control of cord-based blinds.

   This sketch has been designed from scratch however the concept is derived
   from the v2.02 sketch found on cabuu.com
   (http://www.mangetout.net/cabuu/Arduino_Scripts/Blind%20Controller/BCV2_02_Blank.ino)
*/

// TODO: Separate blinds to it's own class?
// TODO: Allow setting of position (0 all the way CCW, 50 open, 0 all the way CW)
// TODO: Think about calibration process - 1) user holds down button until ready vs 2) controller spins motor automatically and user presses button when at end vs 3) blinds automatically determine end
// TODO: Get rid of Blynk?

#include <AceButton.h>
using namespace ace_button;
#include <Homie.h>
#include "LedDriver.h"
#include "MotorDriver.h"

#define FW_VERSION "0.0.1"
#define FW_NAME "awesome-blind-controller"

// HomieEvent handler prototypes
void onHomieEvent(const HomieEvent& event);
bool notificationLedEnabledHandler(const HomieRange& range, const String& value);
bool notificationLedColourHandler(const HomieRange& range, const String& value);
bool shuttersLevelHandler(const HomieRange& range, const String& value);

// HomieNodes
HomieNode ledNode("notification-led", "Notification LED", "led");
HomieNode controllerNode("shutters", "Blinds Controller (Motor)", "shutters");

///////////////////////////////////////////////
//                LED Driver
///////////////////////////////////////////////
LedDriver ledDriver;

/////////////////////////////////////////////////
//                Motor Driver
/////////////////////////////////////////////////
MotorDriver motorDriver;

/////////////////////////////////////////////////
//                Button setup
/////////////////////////////////////////////////

void handleUpButtonEvent(AceButton*, uint8_t, uint8_t);
void handleDownButtonEvent(AceButton*, uint8_t, uint8_t);

const uint8_t UP_BUTTON_PIN = 15;  // GPIO15 - D8
const uint8_t DOWN_BUTTON_PIN = 13; // GPIO13 - D7

ButtonConfig upConfig;
AceButton upButton(&upConfig);

ButtonConfig downConfig;
AceButton downButton(&downConfig);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  Serial << "[setup]" << endl;

  // set up buttons
  setupButtons();

  // set up Homie
  setupHomie();

  // Anything else that relies on MQTT connection goes in the setupHandler() method

}

void loop() {
  long start = millis();
  EVERY_N_MILLISECONDS(1000) {
    Homie.getLogger() << "[loop]" << endl;
  }
  ledDriver.loop();
  Homie.loop();

  // These don't depend on having an active MQTT connection

  upButton.check();
  downButton.check();
  motorDriver.loop();

  debugln("[loop()] time taken: " + String((millis() - start)) + "ms");
  // Anything thing requires an MQTT connection goes in the loopHandler() method
}

void setupHomie() {
  debugln("[setupHomie()] start");

  long start = millis();

  // Set up Homie
  Homie_setFirmware(FW_NAME, FW_VERSION);

  // Setup MQTT topic structures
  ledNode.advertise("enabled").setDatatype("boolean").settable(notificationLedEnabledHandler);
  ledNode.advertise("colour").setDatatype("boolean").settable(notificationLedColourHandler);

  controllerNode.advertise("level").setDatatype("integer")
  .setFormat("0:100").settable(shuttersLevelHandler);

  //Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);
  Homie.onEvent(onHomieEvent);
  Homie.disableResetTrigger();
  Homie.setup();

  debugln("Took " + String((millis() - start)) + "ms to setup Homie");
  debugln("[setupHomie()] end");
}

void setupButtons() {

  Homie.getLogger() << "Setting up buttons..." << endl;

  // configure UP button processing
  upConfig.setEventHandler(handleUpButtonEvent);
  upButton.init(UP_BUTTON_PIN, LOW);

  // configure down button processing
  downConfig.setEventHandler(handleDownButtonEvent);
  downButton.init(DOWN_BUTTON_PIN, LOW);

  Homie.getLogger() << "Button set complete..." << endl;
}

void onHomieEvent(const HomieEvent& event) {

  switch (event.type) {

    case HomieEventType::NORMAL_MODE:
      // Do whatever you want when normal mode is started
      Homie.getLogger() << "Normal mode started. Transition LED to ORANGE" << endl;
      ledDriver.setColour(CONFIG_COLOUR);
      break;

    case HomieEventType::MQTT_READY:
      // Do whatever you want when MQTT is connected in normal mode
      Homie.getLogger() << "MQTT Connected. Transition LED to GREEN" << endl;
      ledDriver.setColour(IDLE_COLOUR);
      break;
    case HomieEventType::MQTT_DISCONNECTED:
      // Do whatever you want when MQTT is disconnected in normal mode
      Homie.getLogger() << "MQTT Disconnected. Transition LED to RED" << endl;
      ledDriver.setColour(DISCONNECTED_COLOUR);
      // You can use event.mqttReason
      break;

  }
}
bool notificationLedEnabledHandler(const HomieRange& range, const String& value) {
  return true;
}
bool notificationLedColourHandler(const HomieRange& range, const String& value) {
  return true;
}
bool shuttersLevelHandler(const HomieRange& range, const String& value) {
  return true;
}

void handleUpButtonEvent(AceButton *  button , uint8_t eventType,
                         uint8_t buttonState ) {

  // Print out a message for all events.
  Homie.getLogger() << "handleUpButtonEvent(): ";
  Homie.getLogger() << "virtualPin: ";
  Homie.getLogger() << button->getPin();
  Homie.getLogger() << "; eventType: ";
  Homie.getLogger() << eventType;
  Homie.getLogger() << "; buttonState: ";
  Homie.getLogger() << buttonState << endl;

  switch (eventType) {
    case AceButton::kEventPressed:
      ledDriver.setColour(MOVING_COLOUR);
      motorDriver.turnCCW(254);
      break;
    case AceButton::kEventReleased:
      motorDriver.stopMotor();
      ledDriver.setColour(IDLE_COLOUR);
      break;
  }
}

void handleDownButtonEvent(AceButton *  button , uint8_t eventType,
                           uint8_t buttonState ) {

  // Print out a message for all events.
  Homie.getLogger() << "handleDownButtonEvent(): ";
  Homie.getLogger() << "virtualPin: ";
  Homie.getLogger() << button->getPin();
  Homie.getLogger() << "; eventType: ";
  Homie.getLogger() << eventType;
  Homie.getLogger() << "; buttonState: ";
  Homie.getLogger() << buttonState << endl;

  switch (eventType) {
    case AceButton::kEventPressed:
      ledDriver.setColour(MOVING_COLOUR);
      motorDriver.turnCW(254);
      break;
    case AceButton::kEventReleased:
      motorDriver.stopMotor();
      ledDriver.setColour(IDLE_COLOUR);
      break;
  }
}
