#include <Encoder.h> // https://www.pjrc.com/teensy/td_libs_Encoder.html

// define step and direction pins
const int xA_step_Pin = 2;
const int xA_dir_Pin = 3;
const int xB_step_Pin = 4;
const int xB_dir_Pin = 5;
const int y_step_Pin = 6;
const int y_dir_Pin = 7;

// define inputs from neural network
int xA_ref = 0;
int xB_ref = 0;
int y_ref = 0;

int xA_current = 0;
int xB_current = 0;
int y_current = 0;

// define encoder pins A and B
Encoder xA_encoder(8, 9);
Encoder xB_encoder(10, 11);
Encoder y_encoder(12, 13);

void setup() {
  // Set the stepper pins as Outputs
  pinMode(xA_step_Pin, OUTPUT);
  pinMode(xA_dir_Pin, OUTPUT);
  pinMode(xB_step_Pin, OUTPUT);
  pinMode(xB_dir_Pin, OUTPUT);
  pinMode(y_step_Pin, OUTPUT);
  pinMode(y_dir_Pin, OUTPUT);
}


void loop() {
  // random signal
  xA_ref = random(xA_current - 1, xB_current + 2);
  xB_ref = xA_ref
           y_ref = random(y_current - 1, y_current + 2);

  // convert encoder into position in mm (encoder has 1000 cpr and radius of pulley is 6.3661977)
  //  xA_current = xA_encoder.read() * 2 * 3.1459265 * 6.3661977 / 1000;
  //  xB_current = xB_encoder.read() * 2 * 3.1459265 * 6.3661977 / 1000;
  //  y_current = y_encoder.read() * 2 * 3.1459265 * 6.3661977 / 1000;

  // convert encoder into count into steps
  xA_current = round(xA_encoder.read() * 400 / 1000);
  xB_current = round(xB_encoder.read() * 400 / 1000);
  y_current = round(y_encoder.read() * 400 / 1000);

  // calculate positional error
  //  xA_error = xA_current - xA_ref;
  //  xB_error = xB_current - xB_ref;
  //  y_error = y_current - y_ref;

  if (xA_current > xA_ref) {
    digitalWrite(xA_dir_Pin, LOW);
    digitalWrite(xA_step_Pin, HIGH);
    delayMicroseconds(500);
    digitalWrite(xA_step_Pin, LOW);
    else if (xA_current < xA_ref) {
      digitalWrite(xA_dir_Pin, HIGH);
      digitalWrite(xA_step_Pin, HIGH);
      delayMicroseconds(500);
      digitalWrite(xA_step_Pin, LOW);
    }
  }

  if (xB_current > xB_ref) {
    digitalWrite(xB_dir_Pin, LOW);
    digitalWrite(xB_step_Pin, HIGH);
    delayMicroseconds(500);
    digitalWrite(xB_step_Pin, LOW);
  }
  else if (xB_current < xB_ref) {
    digitalWrite(xB_dir_Pin, HIGH);
    digitalWrite(xB_step_Pin, HIGH);
    delayMicroseconds(500);
    digitalWrite(xB_step_Pin, LOW);
  }


  if (y_current > y_ref) {
    digitalWrite(y_dir_Pin, LOW);
    digitalWrite(y_step_Pin, HIGH);
    delayMicroseconds(500);
    digitalWrite(y_step_Pin, LOW);
  }
  else if (y_current < y_ref) {
    digitalWrite(y_dir_Pin, HIGH);
    digitalWrite(y_step_Pin, HIGH);
    delayMicroseconds(500);
    digitalWrite(y_step_Pin, LOW);
  }

}
