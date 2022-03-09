#include <Encoder.h> // https://www.pjrc.com/teensy/td_libs_Encoder.html

// define step and direction pins
const int xA_step_Pin = 6;
const int xA_dir_Pin = 5;
const int xB_step_Pin = 4;
const int xB_dir_Pin = 3;
const int y_step_Pin = 2;
const int y_dir_Pin = 1;

// define inputs from neural network
int x_ref = 0;
int y_ref = 0;

int xA_current = 0;
int xB_current = 0;
int y_current = 0;

int spr = 400;
String dataIn = "";

// define encoder pins A and B
  Encoder xA_encoder(7, 8);
  Encoder xB_encoder(11, 12);
  Encoder y_encoder(9, 10);


int count = 0;
int spd = 200;
void setup() {
  // Set the stepper pins as Outputs
  pinMode(xA_step_Pin, OUTPUT);
  pinMode(xA_dir_Pin, OUTPUT);
  pinMode(xB_step_Pin, OUTPUT);
  pinMode(xB_dir_Pin, OUTPUT);
  pinMode(y_step_Pin, OUTPUT);
  pinMode(y_dir_Pin, OUTPUT);
  
  xA_encoder.write(0);
  xB_encoder.write(0);
  y_encoder.write(0);

    Serial.begin(9600);
}


void loop() {
  // read and convert encoder into count into steps
  xA_current = round(xA_encoder.read() * spr / 1000);
  xB_current = round(xB_encoder.read() * spr / 1000);
  y_current  = -round(y_encoder.read() * spr / 1000);

  //Read Serial reference signal
  if (Serial.available()) {       // Check if there's data
   // dataIn = String(Serial.readBytes(512, 50)); // Read data into the variable "in" <X345.3Y34532.3>
   dataIn = Serial.readStringUntil('\n');
   Serial.println(dataIn);
    x_ref = parseInputPos(dataIn, 'x') * spr / (3.1459265 * 6.3661977); //reads reference x position and converts mm position input into step domain
    y_ref = parseInputPos(dataIn, 'y') * spr / (3.1459265 * 6.3661977); // reads reference y position adn converts mm position input into step domain
  }

  

//Serial.println(y_current);
  // random signal
//    xA_ref = random(xA_current - 1, xB_current + 3);
//    xB_ref = xA_ref;
//    y_ref = random(y_current - 1, y_current + 3);

//    xA_ref = 4500*sin(0.001*count);
//    xB_ref = 4500*sin(0.001*count);
//     y_ref =  5000*sin(0.001*count);
//count++;

  // convert encoder into position in mm (encoder has 1000 cpr and radius of pulley is 6.3661977)
  //  xA_current = xA_encoder.read() * 2 * 3.1459265 * 6.3661977 / 1000;
  //  xB_current = xB_encoder.read() * 2 * 3.1459265 * 6.3661977 / 1000;
  //  y_current = y_encoder.read() * 2 * 3.1459265 * 6.3661977 / 1000;



  // calculate positional error
  //  xA_error = xA_current - xA_ref;
  //  xB_error = xB_current - xB_ref;
  //  y_error = y_current - y_ref;
//  digitalWrite(xA_dir_Pin, LOW);
//  digitalWrite(xB_dir_Pin, LOW);
//  digitalWrite(y_dir_Pin, LOW);

  if (xA_current > x_ref) {
    digitalWrite(xA_dir_Pin, LOW);
    digitalWrite(xA_step_Pin, HIGH);
    delayMicroseconds(spd);
    digitalWrite(xA_step_Pin, LOW);
  } else if (xA_current < x_ref) {
    digitalWrite(xA_dir_Pin, HIGH);
    digitalWrite(xA_step_Pin, HIGH);
    delayMicroseconds(spd);
    digitalWrite(xA_step_Pin, LOW);
  }
  else if (xA_current == x_ref) {
    digitalWrite(xA_step_Pin, LOW);
  }

  if (xB_current > x_ref) {
    digitalWrite(xB_dir_Pin, LOW);
    digitalWrite(xB_step_Pin, HIGH);
    delayMicroseconds(spd);
    digitalWrite(xB_step_Pin, LOW);
  }
  else if (xB_current < x_ref) {
    digitalWrite(xB_dir_Pin, HIGH);
    digitalWrite(xB_step_Pin, HIGH);
    delayMicroseconds(spd);
    digitalWrite(xB_step_Pin, LOW);
  }
  else if (xB_current == x_ref) {
    digitalWrite(xB_step_Pin, LOW);
  }

  if (y_current > y_ref) {
    digitalWrite(y_dir_Pin, LOW);
    digitalWrite(y_step_Pin, HIGH);
    delayMicroseconds(spd);
    digitalWrite(y_step_Pin, LOW);
  }
  else if (y_current < y_ref) {
    digitalWrite(y_dir_Pin, HIGH);
    digitalWrite(y_step_Pin, HIGH);
    delayMicroseconds(spd);
    digitalWrite(y_step_Pin, LOW);
  }
  else if (y_current == y_ref) {
    digitalWrite(y_step_Pin, LOW);
  }
}

//Functions -----------------------------------------------------------
//#####################################################################
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
