// By: Luis Santiago Mille Fragoso 
// part of IGEM TEC CEM 2018

///////////////////////////////////////////////////////////////This is the Arduino Nano's program////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Servo.h>

/////////////////////////We define constants to help the code readability
#define sensorOut 2
//      Servo     3
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define toStepper 12 

const int stepsPerRevolution = 200;  
int frequencyR = 0, frequencyG=0, frequencyB=0;
Servo myservo;  
int pos = 0;
int cambio=0;
char bufff[25] = {'t','r','u','e','t','r','u','e','t','r','u','e','t','r','u','e','t','r','u','e','t','r','u','e'};

void setup() {
 Wire.begin(8);                /* join i2c bus with address 8 */
 Wire.onReceive(receiveEvent); /* register receive event */
 Wire.onRequest(requestEvent); /* register request event */
 
 ///////////////////////////////////////////Set pins mode
 pinMode(S0, OUTPUT);
 pinMode(S1, OUTPUT);
 pinMode(S2, OUTPUT);
 pinMode(S3, OUTPUT);
 pinMode(toStepper, OUTPUT);
 pinMode(sensorOut, INPUT);

 // Setting color sensor frequency-scaling to 20%
 digitalWrite(S0,HIGH);
 digitalWrite(S1,LOW);
 digitalWrite(toStepper,LOW); 
 Serial.begin(115200);           // start serial for debug 
 myservo.write(95);				 //Setting the initial servo position to 95 degress
}

void loop() {
  // Setting Red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  frequencyR = pulseIn(sensorOut, LOW);
  Serial.print("R= ");//printing name
  Serial.print(frequencyR);//printing RED color frequency
  Serial.print("  ");
  delay(100);
  
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  frequencyG = pulseIn(sensorOut, LOW);
  Serial.print("G= ");//printing name
  Serial.print(frequencyG);//printing RED color frequency
  Serial.print("  ");
  delay(100);
  
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  frequencyB = pulseIn(sensorOut, LOW);
  Serial.print("B= ");//printing name
  Serial.print(frequencyB);//printing RED color frequency
  Serial.println("  ");
  delay(100);

  ////////////////////////////////////////////////Build the string to be sent to the master which will send it to the server 
  
  String final1 = "T:37C:0.5R:"; // This line is to replace the future implemetation of Temperature and CO2 sensing
  final1.concat(frequencyR);
  String final2 = "G:"; 
  final2.concat(frequencyG);
  String final3 = "B:"; 
  final3.concat(frequencyB);
  final1.concat(final2);
  final1.concat(final3);
  
  final1.toCharArray(bufff, 25);
  
  ////If the sensor detects this specific value ranges, it means that the medium is yellowish which means it needs to be changed.
  //// This values vary depending on sensor, light conditions, and more. 
  
  /////////This is the change sequence
  if(cambio==0&&frequencyR>=74&&frequencyR<=79&&frequencyG>=107&&frequencyG<=112&&frequencyB>=28&&frequencyB<=32){
    myservo.attach(3);

	//The servo opens the 3-way stopcock
    for (pos = 95; pos >= 0; pos -= 1) { 
      myservo.write(pos);    
      delay(15);                       
    }
    
	//It delays for 3 seconds which will allow the medium to flow outside the flask
    delay(3000);    
    
	//// Send signal to the stepper motor to move and inyect the new medium
    digitalWrite(toStepper,HIGH);
    delay(1000);
    digitalWrite(toStepper,LOW);
    delay(10000);
    digitalWrite(toStepper,LOW);
    
	/////The 3-ways stopcock closes
    for (pos = 0; pos <= 95; pos += 1) { 
      myservo.write(pos);              
      delay(15);                       
    }
    
	//// Set the servo pins to LOW to ensure it won't until the next change sequence
    myservo.write(1);
    myservo.detach();
    pinMode(3, OUTPUT);
    digitalWrite(3,LOW);
    
    cambio=1;
    delay(3000);
    
 
  }
  delay(4000);
}

//// Receive function for I2C
void receiveEvent(int howMany) {
 while (0 <Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */  
  }
}

//// Function called when ever the master request data from the slave, that is the sensing data
void requestEvent() {
    
  if(cambio==1){
    Wire.write("camb");
    cambio=0;
    return;
  }
  
  if(cambio==0&&frequencyR>=74&&frequencyR<=79&&frequencyG>=107&&frequencyG<=112&&frequencyB>=28&&frequencyB<=32){
    Wire.write(bufff);
    
  }else if(frequencyR>=57&&frequencyR<=70&&frequencyG>=82&&frequencyG<=90&&frequencyB>=20&&frequencyB<=25){
    Wire.write(bufff); 
    
  }else{
    Wire.write(bufff);
  }
}
