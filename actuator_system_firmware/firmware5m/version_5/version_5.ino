/* Actuator Firward v0.4
   Author: Matthew Mueller, Phillip Milner
   Revision Date: February 10, 22
*/
#include <AccelStepper.h> // https://www.pjrc.com/teensy/td_libs_AccelStepper.html
#include <Encoder.h> // https://www.pjrc.com/teensy/td_libs_Encoder.html

AccelStepper xA_axis(1, 3, 2); // pin 3 = step, pin 2 = direction
AccelStepper xB_axis(1, 4, 5); // pin 3 = step, pin 2 = direction
AccelStepper y_axis(1, 6, 7); // pin 3 = step, pin 2 = direction

Encoder xA_encoder(10, 11); // pin 10, pin 11
Encoder xB_encoder(12, 13); // pin 10, pin 11
Encoder y_encoder(14, 15); // pin 10, pin 11
//Initialze Future Vars

String dataIn = "";
int xPos = 0;
int yPos = 0;
double xA_encoder_pos = 0;
double xB_encoder_pos = 0;
double y_encoder_pos = 0;


double xA_pos_correction = 0;
double xB_pos_correction = 0;

// Definitions
int defaultSpd = 40000;
int defaultAccel = 200;

//

long xA_pos  = 1;
long new_xA_pos;

int distance = 60;

int steps = (distance * 3200) / (2 * 3.1459265 * 6.3661977);

//Setup on boot

void setup() {
  xA_axis.setMaxSpeed(defaultSpd);
  xA_axis.setAcceleration(defaultAccel);
  xB_axis.setMaxSpeed(defaultSpd);
  xB_axis.setAcceleration(defaultAccel);
  y_axis.setMaxSpeed(defaultSpd);
  y_axis.setAcceleration(defaultAccel);

  Serial.begin(9600 );  // Start Serial
  Serial.println("Actuator System Firmware v0.4:");
  Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");

}
int i = 0;
//Contro Scheme
void loop() {
  //Read Serial
  if (Serial.available()) {       // Check if there's data
    dataIn = Serial.readString();           // Read said data into the variable "in" <X345.3Y34532.3>
    xPos = parseInputPos(dataIn, 'x');
    yPos = parseInputPos(dataIn, 'y');
  }

  // Read Encoder Digital
  xA_encoder_pos = xA_encoder.read() * 2 * 3.1459265 * 6.3661977; // total_steps; //in mm
  xB_encoder_pos = xB_encoder.read() * 2 * 3.1459265 * 6.3661977; // total_steps; //in mm
  y_encoder_pos = y_encoder.read() * 2 * 3.1459265 * 6.3661977; // total_steps; //in mm
  if (i>=70){
  
    Serial.println("(XA" + String(xA_encoder_pos)+"XB"+String(xB_encoder_pos)+"Y"+String(y_encoder_pos)+")");
i=0;
  }
i++;
  // Closed-Loop: Correct the stepper position
  if (xA_encoder_pos != xPos || xA_encoder_pos != xPos && xA_axis.isRunning() == false && xB_axis.isRunning() == false) {
    //if the accelstepper postion and the encoder postion dont match, also the motor is not running
    //reset the position in accel stepper to match the encoder position

    xA_pos_correction = xPos - xA_encoder_pos;
    xB_pos_correction = xPos - xB_encoder_pos;

    xA_axis.move(xA_pos_correction);
    xA_axis.move(xA_pos_correction);

    while (xA_encoder_pos != xPos || xA_encoder_pos != xPos) {
      xA_axis.run();
      xB_axis.run();
    }

    xA_axis.setCurrentPosition(xA_encoder_pos);
    Serial.println("calibrate_xA+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n+++++++++++++++++++++++++");
    delay(10000);
  }



  
  xA_axis.moveTo(xPos);

  xA_axis.run();
  xB_axis.run();
  y_axis.run();

}

//Functions -----------------------------------------------------------
float parseInputPos(String in, char coord) {
  String sub  = "";
  float val  = 0;
  if (coord == 'x' &&  in.substring(0, 1) == '<') {
    sub  = in.substring(in.indexOf('X') + 1, in.indexOf('Y'));
    val = sub.toFloat();
  }
  if (coord == 'y' && in.substring(0, 1) == '<') {
    sub  = in.substring(in.indexOf('Y') + 1, in.indexOf('>'));
    val = sub.toFloat();
  }
  return val ;
}
