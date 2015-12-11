#include <Arduino.h>
#include <Adafruit_MotorShield.h>

/////////////
// DEFINES //
/////////////

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *left_motor = AFMS.getMotor(1);
Adafruit_DCMotor *right_motor = AFMS.getMotor(2);

const uint8_t SPEED = 175;  // percentage [0-255]
const uint16_t DURATION = 500;  // ms

const uint8_t ECHO_PIN = 12;
const uint8_t TRIG_PIN = 13;

const unsigned long DISTANCE_THRESHOLD = 10;  // cm

///////////
// SETUP //
///////////

void setup_motors() {
    left_motor->run(RELEASE);
    right_motor->run(RELEASE);
}

void setup_distance_sensor() {
    pinMode(TRIG_PIN, OUTPUT);
    digitalWrite(TRIG_PIN, LOW);
    pinMode(ECHO_PIN, INPUT);
}

void setup() {
    AFMS.begin();
    setup_motors();
    setup_distance_sensor();
}

/////////////
// METHODS //
/////////////

unsigned long sense_distance() {
    /* send pulse */
    digitalWrite(TRIG_PIN, HIGH);
    const uint16_t magic_delay = 1000;  // us
    delayMicroseconds(magic_delay);
    digitalWrite(TRIG_PIN, LOW);

    /* calculate distance */
    unsigned long duration = pulseIn(ECHO_PIN, HIGH);
    const uint8_t conversion_factor = 6;
    return duration >> conversion_factor;
}

void go_forward() {
    left_motor->run(FORWARD);
    right_motor->run(FORWARD);
    left_motor->setSpeed(SPEED);
    right_motor->setSpeed(SPEED);
    delay(DURATION);
}

void go_backward() {
    left_motor->run(BACKWARD);
    right_motor->run(BACKWARD);
    left_motor->setSpeed(SPEED);
    right_motor->setSpeed(SPEED);
    delay(DURATION);
}

void turn_right() {
    left_motor->run(FORWARD);
    right_motor->run(BACKWARD);
    left_motor->setSpeed(SPEED);
    right_motor->setSpeed(SPEED);
    delay(DURATION);
}

void turn_left() {
    left_motor->run(BACKWARD);
    right_motor->run(FORWARD);
    left_motor->setSpeed(SPEED);
    right_motor->setSpeed(SPEED);
    delay(DURATION);
}

void stop() {
    left_motor->run(RELEASE);
    right_motor->run(RELEASE);
    left_motor->setSpeed(0);
    right_motor->setSpeed(0);
    delay(DURATION);
}

//////////
// MAIN //
//////////

void loop() {
    unsigned long distance = sense_distance();
    if (distance >= DISTANCE_THRESHOLD) {
        go_forward();
    } else {
        go_backward();
        turn_left();
    }
}
