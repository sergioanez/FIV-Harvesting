#include <Encoder.h> // https://www.pjrc.com/teensy/td_libs_Encoder.html

// This codes is responsible for reading encoder data and outputting it to serial 
// define encoder pins A and B
  Encoder xA_encoder(2, 3);
  Encoder xB_encoder(6, 7);
  Encoder y_encoder(4, 5);

int xA_current = 0;
int xB_current = 0;
int y_current = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  xA_current = xA_encoder.read();
  xB_current = xB_encoder.read();
  y_current  = -y_encoder.read();
  
//  Serial.println(millis());
  Serial.println( String(millis()) + "," + String(xA_current) + "," + String(xB_current) + "," + String(y_current) );
//  Serial.println(xA_current);
//  Serial.println(xB_current);

  
  
  
}
