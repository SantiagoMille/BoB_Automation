#include <Stepper.h>

const int stepsPerRevolution = 200;  // cambie este valor por el numero de pasos de su motor

// inicializa la libreria 'stepper' en los pines 8 a 11
Stepper myStepper = Stepper(stepsPerRevolution, 9,10,11, 12); 
int inPin = 4;

void setup() {
 pinMode(inPin, INPUT);
 myStepper.setSpeed(55);
 Serial.begin(9600);           /* start serial for debug */
}

void loop() {
  if(digitalRead(inPin)==HIGH){
    myStepper = Stepper(stepsPerRevolution, 9,10,11, 12); 
    myStepper.setSpeed(55);
    Serial.println("3ml");
    myStepper.step(stepsPerRevolution*0.7);//3ml
   
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    digitalWrite(12,LOW);
    myStepper = Stepper(stepsPerRevolution, 4,5,6,7); 
    
    delay(20000); 
    myStepper = Stepper(stepsPerRevolution, 9,10,11, 12);
    myStepper.setSpeed(55);
    Serial.println("7ml");
    myStepper.step(stepsPerRevolution*1.5);//7ml

    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    digitalWrite(12,LOW);
    myStepper = Stepper(stepsPerRevolution, 4,5,6,7); 
    delay(5000); 
  }
}
