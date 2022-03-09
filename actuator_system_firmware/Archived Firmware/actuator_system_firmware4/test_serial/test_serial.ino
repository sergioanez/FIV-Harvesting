#include <AccelStepper.h> // https://www.pjrc.com/teensy/td_libs_AccelStepper.html
#include <Encoder.h> // https://www.pjrc.com/teensy/td_libs_Encoder.html

AccelStepper xA_axis(1, 3, 2); // pin 3 = step, pin 2 = direction
//AccelStepper xB_axis(1, 3, 2); // pin 3 = step, pin 2 = direction
//AccelStepper y_axis(1, 3, 2); // pin 3 = step, pin 2 = direction

Encoder xA_encoder(10, 11); // pin 10, pin 11
//Encoder xB_encoder(10, 11); // pin 10, pin 11
//Encoder y_encoder(10, 11); // pin 10, pin 11

// Definitions
int xSpd = 40000;
int xAccel = 200;

int ySpeed = 40000;
int yAccel = 200;

long xA_pos  = 1;
long new_xA_pos;

int distance = 60;

int steps = (distance * 3200) / (2 * 3.1459265 * 6.3661977);





const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
char messageFromPC[numChars] = {0};
int integerFromPC = 0;
float floatFromPC = 0.0;

boolean newData = false;
boolean on = false;


//============

void setup() {
    Serial.begin(9600);
    xA_axis.setMaxSpeed(xSpd);
    xA_axis.setAcceleration(xAccel);
    Serial.println("This demo expects 3 pieces of data - text, an integer and a floating point value");
    Serial.println("Enter data in this style <HelloWorld, 12, 24.7>  ");
    Serial.println();
}

//============

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }
if (messageFromPC[0] == 's') {
  on = false;
}
if (messageFromPC[0] == 'g') {
  on = true;
}

if (on == true){
    xAxisForward(steps);
  xAxisBackward(-1*steps);
}

    
}

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    integerFromPC = atoi(strtokIndx);     // convert this part to an integer
    distance = integerFromPC;
    steps = (distance * 3200) / (2 * 3.1459265 * 6.3661977);

    strtokIndx = strtok(NULL, ",");
    floatFromPC = atof(strtokIndx);     // convert this part to a float

}

//============

void showParsedData() {
    Serial.print("Message ");
    Serial.println(messageFromPC);
    Serial.print("Integer ");
    Serial.println(integerFromPC);
    Serial.print("Float ");
    Serial.println(floatFromPC);
    
}



int xAxisForward (int dist) {
  xA_axis.setMaxSpeed(xSpd);
  xA_axis.setAcceleration(xAccel);
  xA_axis.move(dist);
  while (xA_axis.distanceToGo() > 0) {
    xA_axis.run();
    new_xA_pos = xA_encoder.read() *0.09;
    if (new_xA_pos != xA_pos) {
      Serial.print("count = ");
      Serial.print(new_xA_pos);
      Serial.println();
      xA_pos = new_xA_pos;
    }
  }
}

int xAxisBackward (int dist) {
  xA_axis.setMaxSpeed(xSpd);
  xA_axis.setAcceleration(xAccel);
  xA_axis.move(dist);
  while (xA_axis.distanceToGo() < 0) {
    xA_axis.run();
    new_xA_pos = xA_encoder.read() * 0.09;
    if (new_xA_pos != xA_pos) {
      Serial.print("count = ");
      Serial.print(new_xA_pos);
      Serial.println();
      xA_pos = new_xA_pos;
    }
  }
}
