#include <Wire.h>
#include <Servo.h>

const int trigPin = 9;
const int echoPin = 10;

long duration;
int distinCM;

Servo radarServo;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  
  // Initialize the servo motor on pin 11
  radarServo.attach(11);
  
  // Start I2C communication as a slave with address 9
  Wire.begin(8); // Address 9 for the second slave
  
  // Register the function to handle requests from the master
  Wire.onRequest(sendDistanceData); 
}

void loop() {
  // Rotate the servo and measure distance for each angle
  for (int i = 0; i <= 180; i++) {
    radarServo.write(i);
    delay(50);
    
    // Trigger the ultrasonic sensor and measure the distance
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distinCM = duration * 0.034 / 2;
    
    // Optionally print the data to the serial monitor for debugging
    Serial.print(i);
    Serial.print("*");
    Serial.print(distinCM);
    Serial.println("#");
  }

  for (int i = 180; i >= 0; i--) {
    radarServo.write(i);
    delay(50);
    
    // Trigger the ultrasonic sensor and measure the distance
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distinCM = duration * 0.034 / 2;
    
    // Optionally print the data to the serial monitor for debugging
    Serial.print(i);
    Serial.print("*");
    Serial.print(distinCM);
    Serial.println("#");
  }
}

// Function to send distance data to the master via I2C
void sendDistanceData() {
  Wire.write(distinCM);
  
}
