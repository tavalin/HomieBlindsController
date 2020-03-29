#include "MotorDriver.h"
#include  <FastLED.h>

MotorDriver::MotorDriver() {
  Serial.println("[MotorDriver()] start");

  motorPin1 = MOTOR_IN1;
  motorPin2 = MOTOR_IN2;

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  // seems to be nnecessary to stop flicking on the LED
  analogWriteFreq(50);

  stopMotor();
  Serial.println("[MotorDriver()] end");
}

long MotorDriver::getMotorPosition() {

}

void MotorDriver::turnCW(uint8_t speed) {
  Serial.println("[MotorDriver::turnCW() start]");
  
  // If spinning up
  stopMotor();

  turnMotor(motorPin1, speed);
  Serial.println("[MotorDriver::turnCW() end]");
}
void MotorDriver::turnCCW(uint8_t speed) {
  Serial.println("[MotorDriver::turnCCW()] start");

  // If spinning up
  stopMotor();

  turnMotor(motorPin2, speed);
    Serial.println("[MotorDriver::turnCCW()] end");
}

void MotorDriver::loop() {

  EVERY_N_MILLISECONDS(500) {
    Serial.println("[MotorDriver::loop()]");

    long pos = encoder.read();

    Serial.print("[MotorDriver::loop()] pos: ") ;
    Serial.println(pos);
  }

}

void MotorDriver::stopMotor() {
  Serial.println("[MotorDriver::stopMotor()] start");

  // need to disable PWM but setting value of analogWrite to 0 
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);

  // then we can disable the motor
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);

  Serial.println("[MotorDriver::stopMotor()] end");
}

void MotorDriver::turnMotor(uint8_t myPin, uint8_t mySpeed) {
  Serial.println("[MotorDriver::turnMotor()] start");
  // If spinning up
  stopMotor();

  int pwm = map(mySpeed, 0, 255, 511, 1023);

  analogWrite(myPin, pwm);
  Serial.println("[MotorDriver::turnMotor()] end");

}
