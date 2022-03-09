
float input_signal = 0;
float calibration = 1;

void setup() {
  pinMode(2, INPUT);

Serial.begin(9600);
}

void loop() {
  
input_signal = analogRead(A0);
distance = input_signal * calibration;
serial.println(distance)

}
