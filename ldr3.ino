const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 11;
const int ledPin = 13;
const int ledPin_ldr = 8;   //the number of the LED pin
const int ldrPin = A0;  //the number of the LDR pin
// defines variables
long duration;
int distance;
int safetyDistance;
 
int calibrationTime = 30;       
long unsigned int lowIn;        
long unsigned int pause = 5000; 
boolean lockLow = true;
boolean takeLowTime;  
int pirPin = 3;    //the digital pin connected to the PIR sensor's output
int ledPinpir = 12;

void setup() {
Serial.begin(9600);
pinMode(ledPin_ldr, OUTPUT);  //initialize the LED pin as an output
pinMode(ldrPin, INPUT);   //initialize the LDR pin as an input
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(buzzer, OUTPUT);
pinMode(ledPin, OUTPUT);
Serial.begin(9600); // Starts the serial communication

pinMode(pirPin, INPUT);
pinMode(ledPinpir, OUTPUT);
digitalWrite(pirPin, LOW);
Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
Serial.println(" done");
Serial.println("SENSOR ACTIVE");
delay(50);
}
 
 
void loop() {
int ldrStatus = analogRead(ldrPin);   //read the status of the LDR value
 
  //check if the LDR status is <= 300
  //if it is, the LED is HIGH
 
   if (ldrStatus <=300) {
 
    digitalWrite(ledPin_ldr, HIGH);               //turn LED on
    Serial.println("LDR is DARK, LED is ON");
    
   }
  else {
 
    digitalWrite(ledPin_ldr, LOW);          //turn LED off
    Serial.println("ITS OK,DON'T TURN ON THE LIGHT");
  }

  
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
 
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(5);
digitalWrite(trigPin, LOW);
 
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
 
// Calculating the distance
distance= duration*0.034/2;
 
safetyDistance = distance;
if (safetyDistance <= 5){
  digitalWrite(buzzer, HIGH);
  digitalWrite(ledPin, HIGH);
}
else{
  digitalWrite(buzzer, LOW);
  digitalWrite(ledPin, LOW);
}
 
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
//

     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPinpir, HIGH);   //the led visualizes the sensors output pin state
       if(lockLow){ 
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;           
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec");
         delay(50);
         }        
         takeLowTime = true;
       }
 
     if(digitalRead(pirPin) == LOW){      
       digitalWrite(ledPinpir, LOW);  //the led visualizes the sensors output pin state
 
       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause,
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){ 
           //makes sure this block of code is only executed again after
           //a new motion sequence has been detected
           lockLow = true;                       
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           delay(50);
           }
       }
}
