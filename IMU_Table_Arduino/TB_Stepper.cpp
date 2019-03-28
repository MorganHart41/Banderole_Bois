#include "TB_Stepper.h"
#include <Arduino.h>
#include <stdint.h>

TB_Stepper::TB_Stepper(int pul, int dir, int en) {
  this->pul = pul;
  this->dir = dir;
  this->en = en;
}

// Initialize the stepper motor
void TB_Stepper::init() {
  pinMode(pul, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(en, OUTPUT);
}

// Steps the stepper motor "steps" times at speed  "rpm"
void TB_Stepper::step(int steps, int rpm) {
  //calculate delay for given speed - Assuming 1.8 degree step and controller
  // set to 1/32 step
  float rpm_delay_unround = (rpm * 6.4);        // total steps in 1 min / 1000
                                                // (/1000 to handle overflow)

  rpm_delay_unround = 60 / rpm_delay_unround;   // total steps per second

  rpm_delay_unround = rpm_delay_unround / 2;    // since there are 2 delays per step

  int rpm_delay = rpm_delay_unround * 1000;     // round to whole integer and
                                                // compensate for the overflow
                                                // handling
  if(steps > 0) {
    for (int i=0; i <= steps; i++) {
      digitalWrite(dir,LOW);
      digitalWrite(en,HIGH);
      digitalWrite(pul,HIGH);
      delayMicroseconds(rpm_delay);
      digitalWrite(pul,LOW);
      delayMicroseconds(rpm_delay);
    }
  } else {
    for (int i=0; i <= abs(steps); i++) {
      digitalWrite(dir,HIGH);
      digitalWrite(en,HIGH);
      digitalWrite(pul,HIGH);
      delayMicroseconds(rpm_delay);
      digitalWrite(pul,LOW);
      delayMicroseconds(rpm_delay);
    }
  }
}
