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
float spr = 400;

void setup() {
  Serial.begin(20000000);
  CCM_CSCDR1 = 105450240;
}

void loop() {

  if ( round(xA_encoder.read() * spr / 1000) != xA_current || round(xB_encoder.read() * spr / 1000) != xB_current || round(y_encoder.read() * spr / 1000) != y_current) {
    Serial.print( String(float(millis())) + "," + String(xA_dist) + "," + String(xB_dist) + "," + String(y_dist) + "\n");
  }

  // put your main code here, to run repeatedly:
  xA_current = round(xA_encoder.read() * spr / 1000);
  xB_current = round(xB_encoder.read() * spr / 1000);
  y_current  = -round(y_encoder.read() * spr / 1000);

  xA_dist = xA_current * (3.1459265 * 6.3661977) / spr ;
  xB_dist = xB_current * (3.1459265 * 6.3661977) / spr ;
  y_dist = y_current * (3.1459265 * 6.3661977) / spr ;


  delay(100);


}
