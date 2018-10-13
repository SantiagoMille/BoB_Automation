#include <Wire.h>
//#include <Stepper.h>
#include <Servo.h>

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
 
 pinMode(S0, OUTPUT);
 pinMode(S1, OUTPUT);
 pinMode(S2, OUTPUT);
 pinMode(S3, OUTPUT);
 pinMode(toStepper, OUTPUT);
 pinMode(sensorOut, INPUT);

 // Setting frequency-scaling to 20%
 digitalWrite(S0,HIGH);
 digitalWrite(S1,LOW);
 digitalWrite(toStepper,LOW); 
 Serial.begin(115200);           /* start serial for debug */
 myservo.write(95);
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

  String final1 = "T:37C:0.5R:"; 
  final1.concat(frequencyR);
  String final2 = "G:"; 
  final2.concat(frequencyG);
  String final3 = "B:"; 
  final3.concat(frequencyB);
  final1.concat(final2);
  final1.concat(final3);
  
  final1.toCharArray(bufff, 25);

  Serial.println(bufff);
  
  if(cambio==0&&frequencyR>=74&&frequencyR<=79&&frequencyG>=107&&frequencyG<=112&&frequencyB>=28&&frequencyB<=32){
    myservo.attach(3);

    for (pos = 95; pos >= 0; pos -= 1) { 
      myservo.write(pos);    
      delay(15);                       
    }
    
    delay(3000);    
    
    digitalWrite(toStepper,HIGH);
    delay(1000);
    digitalWrite(toStepper,LOW);
    delay(10000);
    digitalWrite(toStepper,LOW);
    
    for (pos = 0; pos <= 95; pos += 1) { 
      myservo.write(pos);              
      delay(15);                       
    }
    
    myservo.write(1);
    myservo.detach();
    pinMode(3, OUTPUT);
    digitalWrite(3,LOW);
    
    cambio=1;
    delay(3000);
    
 
  }
  delay(4000);
}

void receiveEvent(int howMany) {
 while (0 <Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    Serial.print(c);           /* print the character */
  }
  Serial.println();
}

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
