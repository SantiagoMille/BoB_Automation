// By: Luis Santiago Mille Fragoso 
// part of IGEM TEC CEM 2018

///////////////////////////////////////////////////////////////This is the Arduino Nano's Stepper motor program////////////////////////////////////////////////////////////

#include <Stepper.h>

const int stepsPerRevolution = 200;  // We set the number of steps of our stepper motor as a constant (Change if yours has different number)

// Initialize 'stepper' on pins 8 & 11
Stepper myStepper = Stepper(stepsPerRevolution, 9,10,11, 12); 

//// Boolean flag for then the stepper needs to be activated
int inPin = 4;

////Set up the input pin to receive the boolean variable
void setup() {
 pinMode(inPin, INPUT);
 myStepper.setSpeed(55);
 Serial.begin(9600);           /* start serial for debug */
}

void loop() {
	
  /////If the pin recieves a HIGH input from the Master Arduino then the stepper motor sequence starts
	
  if(digitalRead(inPin)==HIGH){
	
	//Send washing medium
    myStepper = Stepper(stepsPerRevolution, 9,10,11, 12); 
    myStepper.setSpeed(55);
    Serial.println("2ml");
    myStepper.step(stepsPerRevolution*0.7);//2ml
   
    // Disattach the stepper motor due to current consumption and driver overheating
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    digitalWrite(12,LOW);
    myStepper = Stepper(stepsPerRevolution, 4,5,6,7); 
    
	//At this point we are waiting for all the medium to flow 
    delay(20000); 
	
	//We re-attach the stepper and inject aprox 6.5 ml of medium to the flask
    myStepper = Stepper(stepsPerRevolution, 9,10,11, 12);
    myStepper.setSpeed(55);
    Serial.println("6.5ml");
    myStepper.step(stepsPerRevolution*1.5);//6.5ml

	// Disattach the stepper motor due to current consumption and driver overheating
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
