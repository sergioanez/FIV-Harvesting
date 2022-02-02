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

long xA_pos  = 1;
long new_xA_pos;

int distance = 60;

int steps = (distance * 3200) / (2 * 3.1459265 * 6.3661977);



void setup() {
  xA_axis.setMaxSpeed(xSpd);
  xA_axis.setAcceleration(xAccel);

Serial.begin(9600);  // Start Serial
Serial.println("Encoder Test:");
}

void loop() {
  
    if (Serial.available() > 0 ){      
         distance = Serial.parseInt();
         steps = (distance * 3200) / (2 * 3.1459265 * 6.3661977);
  }
  xAxisForward(steps);
  xAxisBackward(-1*steps);
  
}



// Functions

int xAxisForward (int dist) {
  xA_axis.setMaxSpeed(xSpd);
  xA_axis.setAcceleration(xAccel);
  xA_axis.move(dist);
  while (xA_axis.distanceToGo() > 0) {
    xA_axis.run();
    new_xA_pos = xA_encoder.read() *0.09;
    if (new_xA_pos != xA_pos) {
     // Serial.print("count = ");
      //Serial.print(new_xA_pos);
      //Serial.println();
      xA_pos = new_xA_pos;
    }
  }
}

int xAxisBackward (int dist) {
  xA_axis.setMaxSpeed(xSpd);
  xA_axis.setAcceleration(xAccel);
  xA_axis.move(dist);
  while (xA_axis.distanceToGo() < 0) {
    xA_axis.run();
    new_xA_pos = xA_encoder.read() * 0.09;
    if (new_xA_pos != xA_pos) {
      //Serial.print("count = ");
      //Serial.print(new_xA_pos);
      //Serial.println();
      xA_pos = new_xA_pos;
    }
  }
}
