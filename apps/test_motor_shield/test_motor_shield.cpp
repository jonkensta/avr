#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *myMotor = AFMS.getStepper(2, 1);

void step_forward() {
    myMotor->step(1, FORWARD, SINGLE);
}
void step_backward() {
    myMotor->step(1, BACKWARD, SINGLE);
}

AccelStepper stepper(step_forward, step_backward); // use functions to step

void setup()
{
    AFMS.begin();
    stepper.setSpeed(1000);
}

void loop()
{
   stepper.runSpeed();
}
