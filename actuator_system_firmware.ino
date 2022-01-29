#include <AccelStepper.h> // https://www.pjrc.com/teensy/td_libs_AccelStepper.html
#include <Encoder.h> // https://www.pjrc.com/teensy/td_libs_Encoder.html

AccelStepper xA_axis(1, 3, 2); // pin 3 = step, pin 2 = direction
//AccelStepper xB_axis(1, 3, 2); // pin 3 = step, pin 2 = direction
//AccelStepper y_axis(1, 3, 2); // pin 3 = step, pin 2 = direction

Encoder xA_encoder(10, 11); // pin 10, pin 11
//Encoder xB_encoder(10, 11); // pin 10, pin 11
//Encoder y_encoder(10, 11); // pin 10, pin 11

// Definitions
int xSpd = 40000;
int xAccel = 200;

int ySpeed = 40000;
int yAccel = 200;


void setup() {
  xA_axis.setMaxSpeed(xSpd);
  xA_axis.setAcceleration(xAccel);

  Serial.begin(9600);  // Start Serial
}

void loop() {
  xAxisForward(4000);
  xAxisBackward(-4000);
}



// Functions

int xAxisForward (int dist) {
  xA_axis.setMaxSpeed(xSpd);
  xA_axis.setAcceleration(xAccel);
  xA_axis.move(dist);
  while (xA_axis.distanceToGo() > 0) {
    xA_axis.run();
  }
}

int xAxisBackward (int dist) {
  xA_axis.setMaxSpeed(xSpd);
  xA_axis.setAcceleration(xAccel);
  xA_axis.move(dist);
  while (xA_axis.distanceToGo() < 0) {
    xA_axis.run();
  }
}
