
float input_signal = 0;
float calibration = 1;
float distance = 0;
void setup() {
  pinMode(0, INPUT);
  // 81.0 for 50 mm
  // 314.0 for 80 mm
  Serial.begin(9600);
}

void loop() {

  input_signal = analogRead(A0);
  distance = 50 + (input_signal-81) * 0.129;
  //distance = 0 + (input_signal - 0) * 1;
  Serial.println(distance);
  delay(100);

}
