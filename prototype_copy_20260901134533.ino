#include <QTRSensors.h>
#include <SD.h>
#include <L298NX2.h>

//line sennsor array
QTRSensors qtr;

const uint8_t SensorCount = 4;

uint16_t sensorValues[SensorCount];


//ultrasonic pins
const int trig = A0;
const int echo = A1;

  // Define color sensor pins
 
#define S0 6
#define S1 5
#define S2 7
#define S3 8

#define sensorOut 9
//Motor A
int IN1 = 4; int IN2 = 2;

//Motor B
int IN3 = 3; int IN4 = 1;
//motor pin assignment
              //       motor A            motor B
              // |------------------||------------------|
L298NX2 myMotors(IN1,            IN2, IN3,            IN4);
// Calibration Values
// Get these from Calibration Sketch
 
int redMin = 42; // Red minimum value
int redMax = 203; // Red maximum value
int greenMin = 47; // Green minimum value
int greenMax = 236; // Green maximum value
int blueMin = 42; // Blue minimum value
int blueMax = 191; // Blue maximum value

 
// Variables for Color Pulse Width Measurements
 
int redPW = 0;
int greenPW = 0;
int bluePW = 0;
 
// Variables for final Color values
 
int redValue;
int greenValue;
int blueValue;

//white and black readings after calibration
//White [243, 242, 245]
//Black [18, 11, 9]

int green[] = {183, 195, 158};
bool greenFlag[] = {false, false, false};

int purple[] = {202, 193, 213};
bool purpleFlag[] = {false, false, false};

int yellow[] = {240, 223, 185};
bool yellowFlag[] = {false, false, false};


int difference = 0;
 
//SD card reader
File myFile;

const int CSPin = 10;

String test = "";
char moves[64];

void setup()

{
  Serial.begin(9600);
  LineSensorSetup();
  SDCardSetup();
  ColourSensorSetup();
  MotorSetup();
  writeToSDCard();
  readFromSDCard();
  //reading instructions from sd card=[]=
  test.toCharArray(moves, test.length());

  for (int i = 0; i < test.length(); i++) {
    switch(moves[i]) {
      case 'L':
      left();
      case 'R':
      right();
      case 'F':
      forward();
      case 'B':
      backward();
    }
  }
}

void MotorSetup(){

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

}

void LineSensorSetup() {
  // configure the sensors

  qtr.setTypeRC();

  qtr.setSensorPins((const uint8_t[]){A2, A3, A4, A5}, SensorCount);

}

void SDCardSetup(){


  Serial.print("Initializing SD card...");

  

// Note that even if it's not used as the CS pin, the hardware SS pin 

  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 

  // or the SD library functions will not work. 

   pinMode(CSPin, OUTPUT);\

    if (!SD.begin(CSPin)) {

    Serial.println("initialization failed!");

    return;

  }

  Serial.println("initialization done.");
}

void writeToSDCard(){
  // open the file for writing

  myFile = SD.open("test.txt", FILE_WRITE);

 

  // if the file opened okay, write to it:

  if (myFile) {

    Serial.print("writing to test.txt");
    myFile.println("LRFBLRFB");

    myFile.close();

  } else {



  }
}

void readFromSDCard(){
   // open the file for reading:

  myFile = SD.open("test.txt");

  if (myFile) {

    Serial.println("test.txt:");

 

    // read from the file until there's nothing else in it:

    while (myFile.available()) {

      Serial.write(myFile.read());

    }

    // close the file:

    myFile.close();

  } else {

    // if the file didn't open, print an error:

    Serial.println("error opening test.txt");

  }
}

void loop()

{
  LineSensor();
  ColourSensorCalibration();

}

void LineSensor() {
    // read raw sensor values

  qtr.read(sensorValues);



  // print the sensor values as numbers from 0 to 1023, where 0 means maximum

  // reflectance and 1023 means minimum reflectance

  for (uint8_t i = 0; i < SensorCount; i++)

  {

    Serial.print(sensorValues[i]);

    Serial.print('\t');

  }

  Serial.println();



  delay(250);
}
void GoStraight(){

}

void GoLeft(){

}

void GoRight(){
  
}

void ColourSensorCalibration(){
  // Read Red value
  redPW = getRedPW();
  // Map to value from 0-255
  redValue = map(redPW, redMin,redMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Read Green value
  greenPW = getGreenPW();
  // Map to value from 0-255
  greenValue = map(greenPW, greenMin,greenMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Read Blue value
  bluePW = getBluePW();
  // Map to value from 0-255
  blueValue = map(bluePW, blueMin,blueMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Print output to Serial Monitor
  Serial.print("Red = ");
  Serial.print(redValue);
  Serial.print(" - Green = ");
  Serial.print(greenValue);
  Serial.print(" - Blue = ");
  Serial.println(blueValue);
  
  // // Check for Yellow: High Red, High Green, Low Blue
  // if ((redValue > 210) && (greenValue > 200) && (blueValue < 180)) {
  //   Serial.println("YELLOW");
  // } 
  // // Check for Purple: High Red, High Blue, Low Green
  // else if ((redValue > 180) && (greenValue < 150) && (blueValue > 160)) {
  //   Serial.println("PURPLE");
  // }

}
 
 
// Function to read Red Pulse Widths
int getRedPW() {
 
  // Set sensor to read Red only
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
 
}
 
// Function to read Green Pulse Widths
int getGreenPW() {
 
  // Set sensor to read Green only
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
 
}
 
// Function to read Blue Pulse Widths
int getBluePW() {
 
  // Set sensor to read Blue only
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
 
}
void ColourSensorSetup() {
 
  // Set S0 - S3 as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Set Sensor output as input
  pinMode(sensorOut, INPUT);
  
  // Set Frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
}

//sd card instruction reading
void left(){
  Serial.println("Left move code");
}

void right(){
  Serial.println("Right move");
}

void forward(){
  Serial.println("Forward move");
}

void backward(){
  Serial.println("Backward move");
}