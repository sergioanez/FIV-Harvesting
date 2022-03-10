#include <Encoder.h> // https://www.pjrc.com/teensy/td_libs_Encoder.html

// This codes is responsible for reading encoder data and outputting it to serial 
// define encoder pins A and B
  Encoder xA_encoder(2, 3);
  Encoder xB_encoder(6, 7);
  Encoder y_encoder(4, 5);

int xA_current = 0;
int xB_current = 0;
int y_current = 0;
int xA_dist = 0;
int xB_dist = 0;
int y_dist = 0;
int spr = 400;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
}

void loop() {
  // put your main code here, to run repeatedly:
  xA_current = round(xA_encoder.read() * spr / 1000);
  xB_current = round(xB_encoder.read() * spr / 1000);
  y_current  = -round(y_encoder.read() * spr / 1000);

  xA_dist = xA_current * (3.1459265 * 6.3661977)/spr ; 
  xB_dist = xB_current * (3.1459265 * 6.3661977)/spr ; 
  y_dist = y_current * (3.1459265 * 6.3661977)/spr ; 

  
//  Serial.println(millis());
  Serial.print( String(millis()) + "," + String(xA_dist) + "," + String(xB_dist) + "," + String(y_dist) +"\n");
//  Serial.println(xA_current);
//  Serial.println(xB_current);

  delay(1000);
  
  
}
