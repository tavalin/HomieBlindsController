#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H
#include "Configuration.h"
#include <Encoder.h>

class MotorDriver {

  public:
    MotorDriver();
    long getMotorPosition();
    void turnCW(uint8_t mySpeed);
    void turnCCW(uint8_t mySpeed);
    void loop();
    void stopMotor();

  private:
    void turnMotor(uint8_t myPin, uint8_t mySpeed);
    uint8_t motorPin1;
    uint8_t motorPin2;
    Encoder encoder = Encoder(ENC_PIN1, ENC_PIN2); // (ENC_PIN1, ENC_PIN2);
};


#endif
