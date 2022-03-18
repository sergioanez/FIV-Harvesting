#include <Encoder.h> // https://www.pjrc.com/teensy/td_libs_Encoder.html

// This codes is responsible for reading encoder data and outputting it to serial
// define encoder pins A and B
Encoder xA_encoder(2, 3);
Encoder xB_encoder(6, 7);
Encoder y_encoder(4, 5);

float xA_current = 0;
float xB_current = 0;
float y_current = 0;
float xA_dist = 0;
float xB_dist = 0;
float y_dist = 0;
float spr = 400*64;

void setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  
  Serial.begin(20000000);
  CCM_CSCDR1 = 105450240;
}

void loop() {

  if ( round(xA_encoder.read() / 4000)*spr != xA_current || round(xB_encoder.read() * spr / 4000)*spr != xB_current || round(y_encoder.read() * spr / 4000)*spr != y_current) {
    Serial.print( String(float(millis())) + "," + String(xA_dist) + "," + String(xB_dist) + "," + String(y_dist) + "\n");
  }

  // put your main code here, to run repeatedly:
  xA_current = xA_encoder.read() ;
  xB_current = xB_encoder.read();
  y_current  = y_encoder.read() ;

  xA_dist = (xA_current / 4000)*spr;
  xB_dist = (xB_current / 4000)*spr;
  y_dist = (-y_current / 4000)*spr;




  delay(100);


}
