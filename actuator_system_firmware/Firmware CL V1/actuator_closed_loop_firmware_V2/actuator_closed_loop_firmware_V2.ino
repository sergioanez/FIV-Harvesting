#include <Encoder.h> // https://www.pjrc.com/teensy/td_libs_Encoder.html

// define step and direction pins
const int xA_step_Pin = 6;
const int xA_dir_Pin = 5;
const int xB_step_Pin = 4;
const int xB_dir_Pin = 3;
const int y_step_Pin = 2;
const int y_dir_Pin = 1;

// define inputs from neural network
float x_ref = 0;
float y_ref = 0;

float xA_current = 0;
float xB_current = 0;
float y_current = 0;

float xA_enc_step = 0;
float xB_enc_step = 0;
float y_enc_step = 0;

float spr = 400 * 64; //400 full steps per revolution with 64 microstepping enabled
String dataIn = "";

// define encoder pins A and B
Encoder xA_encoder(7, 8);
Encoder xB_encoder(11, 12);
Encoder y_encoder(9, 10);


int count = 0;
int spd = 10;



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

  Serial.begin(20000000);
  CCM_CSCDR1 = 105450240;
}


void loop() {
  // read and convert encoder into count into steps
  xA_current = xA_encoder.read() ;
  xB_current = xB_encoder.read();
  y_current  = y_encoder.read() ;

  xA_enc_step = (xA_current / 4000) * spr;
  xB_enc_step = (xB_current / 4000) * spr;
  y_enc_step = (-y_current / 4000) * spr;

  //Read Serial reference signal
  if (Serial.available()) {       // Check if there's data
    // dataIn = String(Serial.readBytes(512, 50)); // Read data into the variable "in" <X345.3Y34532.3>
    dataIn = Serial.readStringUntil('\n');
    // Serial.println(dataIn);
    x_ref =  parseInputPos(dataIn, 'x'); //reads reference x position and converts mm position input into step domain
    y_ref =  parseInputPos(dataIn, 'y'); // reads reference y position adn converts mm position input into step domain
    spd = parseInputPos(dataIn, 'v');
  }


  //Round
  xA_enc_step = round(xA_enc_step);
  xB_enc_step = round(xB_enc_step);
  x_ref = round(x_ref);
  y_enc_step = round(y_enc_step);
  y_ref = round(y_ref);

  //Serial.println(String(x_ref) + ","+ snprintf (20, sizeof(20), "%f", xA_enc_step) + ","+ String(xB_enc_step));

  //Logic
  if (xA_enc_step > x_ref) {
    digitalWrite(xA_dir_Pin, LOW);
    digitalWrite(xA_step_Pin, HIGH);
    delayMicroseconds(spd);
    digitalWrite(xA_step_Pin, LOW);
  } else if (xA_enc_step < x_ref) {
    digitalWrite(xA_dir_Pin, HIGH);
    digitalWrite(xA_step_Pin, HIGH);
    delayMicroseconds(spd);
    digitalWrite(xA_step_Pin, LOW);
  }
  else if (xA_enc_step == x_ref) {
    digitalWrite(xA_step_Pin, LOW);
  }

  if (xB_enc_step > x_ref) {
    digitalWrite(xB_dir_Pin, LOW);
    digitalWrite(xB_step_Pin, HIGH);
    delayMicroseconds(spd);
    digitalWrite(xB_step_Pin, LOW);
  }
  else if (xB_enc_step < x_ref) {
    digitalWrite(xB_dir_Pin, HIGH);
    digitalWrite(xB_step_Pin, HIGH);
    delayMicroseconds(spd);
    digitalWrite(xB_step_Pin, LOW);
  }
  else if (xB_enc_step == x_ref) {
    digitalWrite(xB_step_Pin, LOW);
  }

  if (y_enc_step > y_ref) {
    digitalWrite(y_dir_Pin, LOW);
    digitalWrite(y_step_Pin, HIGH);
    delayMicroseconds(spd);
    digitalWrite(y_step_Pin, LOW);
  }
  else if (y_enc_step < y_ref) {
    digitalWrite(y_dir_Pin, HIGH);
    digitalWrite(y_step_Pin, HIGH);
    delayMicroseconds(spd);
    digitalWrite(y_step_Pin, LOW);
  }
  else if (y_enc_step == y_ref) {
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
    sub  = in.substring(in.indexOf('Y') + 1, in.indexOf('V'));
    val = sub.toFloat();
  }
    if (coord == 'v' && in.substring(0, 1) == '<') {
    sub  = in.substring(in.indexOf('V') + 1, in.indexOf('>'));
    val = sub.toFloat();
  }
  return val ;
}
