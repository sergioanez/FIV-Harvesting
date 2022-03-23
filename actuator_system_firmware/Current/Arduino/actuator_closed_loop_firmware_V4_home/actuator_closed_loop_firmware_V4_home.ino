#include <Encoder.h> // https://www.pjrc.com/teensy/td_libs_Encoder.html

// define step and direction pins
const int xA_step_Pin = 6;
const int xA_dir_Pin = 5;
const int xB_step_Pin = 4;
const int xB_dir_Pin = 3;
const int y_step_Pin = 2;
const int y_dir_Pin = 1;
const int homing_Pin = 33;
const int xA_lim_Pin = 38;
const int xB_lim_Pin = 39;
const int y_lim_Pin = 40;


// define inputs from neural network
float x_ref = 0;
float y_ref = 0;
int spd = 100;


float xA_current = 0;
float xB_current = 0;
float y_current = 0;

float xA_enc_step = 0;
float xB_enc_step = 0;
float y_enc_step = 0;

float spr = 400 * 8; //400 full steps per revolution with 64 microstepping enabled
String dataIn = "";

// define encoder pins A and B
Encoder xA_encoder(7, 8);
Encoder xB_encoder(11, 12);
Encoder y_encoder(9, 10);

int count = 0;
float tolerance = 5;


void setup() {
  // Set the stepper pins as Outputs
  pinMode(xA_step_Pin, OUTPUT);
  pinMode(xA_dir_Pin, OUTPUT);
  pinMode(xB_step_Pin, OUTPUT);
  pinMode(xB_dir_Pin, OUTPUT);
  pinMode(y_step_Pin, OUTPUT);
  pinMode(y_dir_Pin, OUTPUT);
  pinMode(homing_Pin, INPUT);
  pinMode(xA_lim_Pin, INPUT);
  pinMode(xB_lim_Pin, INPUT);
  pinMode(y_lim_Pin, INPUT);

  xA_encoder.write(0);
  xB_encoder.write(0);
  y_encoder.write(0);

  Serial.begin(20000000);
  CCM_CSCDR1 = 105450240;
}


void loop() {
  //check if homing command has been given
  if (digitalRead( homing_Pin) == HIGH) {
    homingSequence();
  }


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


  //Logic
  if (xA_enc_step > x_ref + tolerance) {
    digitalWrite(xA_dir_Pin, LOW);
    digitalWrite(xA_step_Pin, HIGH);
  } else if (xA_enc_step < x_ref - tolerance) {
    digitalWrite(xA_dir_Pin, HIGH);
    digitalWrite(xA_step_Pin, HIGH);
  }
  else {
    digitalWrite(xA_step_Pin, LOW);
  }

  if (xB_enc_step > x_ref + tolerance) {
    digitalWrite(xB_dir_Pin, LOW);
    digitalWrite(xB_step_Pin, HIGH);
  }
  else if (xB_enc_step < x_ref - tolerance) {
    digitalWrite(xB_dir_Pin, HIGH);
    digitalWrite(xB_step_Pin, HIGH);
  }
  else {
    digitalWrite(xB_step_Pin, LOW);
  }

  if (y_enc_step > y_ref + tolerance) {
    digitalWrite(y_dir_Pin, LOW);
    digitalWrite(y_step_Pin, HIGH);
  }
  else if (y_enc_step < y_ref - tolerance) {
    digitalWrite(y_dir_Pin, HIGH);
    digitalWrite(y_step_Pin, HIGH);
  }
  else  {
    digitalWrite(y_step_Pin, LOW);
  }

  delayMicroseconds(spd);
  digitalWrite(xA_step_Pin, LOW);
  digitalWrite(xB_step_Pin, LOW);
  digitalWrite(y_step_Pin, LOW);

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


void homingSequence() {
  do {
    if (digitalRead(xA_lim_Pin) == LOW) {
      digitalWrite(xA_dir_Pin, LOW);
      digitalWrite(xA_step_Pin, HIGH);
    }
    else {
      digitalWrite(xA_dir_Pin, LOW);
      digitalWrite(xA_step_Pin, LOW);
    }

    if (digitalRead(xB_lim_Pin) == LOW) {
      digitalWrite(xB_dir_Pin, LOW);
      digitalWrite(xB_step_Pin, HIGH);
    }
    else {
      digitalWrite(xB_dir_Pin, LOW);
      digitalWrite(xB_step_Pin, LOW);
    }


    if (digitalRead(y_lim_Pin) == LOW) {
      digitalWrite(y_dir_Pin, LOW);
      digitalWrite(y_step_Pin, HIGH);
    }
    else {
      digitalWrite(y_dir_Pin, LOW);
      digitalWrite(y_step_Pin, LOW);
    }

    delayMicroseconds(spd);
    digitalWrite(xA_step_Pin, LOW);
    digitalWrite(xB_step_Pin, LOW);
    digitalWrite(y_step_Pin, LOW);

    //Serial.println(digitalRead(xA_lim_Pin));


  } while (digitalRead(xA_lim_Pin) == LOW || digitalRead(xB_lim_Pin) == LOW || digitalRead(y_lim_Pin) == LOW );


  xA_current = 0;
  xB_current = 0;
  y_current = 0;
  x_ref = 0;
  y_ref = 0;
  
  xA_encoder.write(0);
  xB_encoder.write(0);
  y_encoder.write(0);

  delay(1000);

  //debounce
  x_ref = 200*8/0.1;
  y_ref = 160*8/0.1;


}
