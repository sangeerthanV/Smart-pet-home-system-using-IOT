#include <Wire.h>

#include <Servo.h>


#define SERVO_PIN 4
#define FORCE_SENSOR A0
#define TRIG_PIN 6
#define ECHO_PIN 7

Servo foodServo;

int foodDispensed = 0; // Tracks if food is dispensed
unsigned long lastDispenseTime = 0; // Tracks the last food dispense time

void setup() {
  Wire.begin(9); // Initialize I2C with address 9
  Wire.onRequest(sendFoodStatus);
  Serial.begin(9600);

  foodServo.attach(SERVO_PIN);
  foodServo.write(0);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Check if it's time to dispense food
  if (foodDispensed == 0) {
    dispenseFood();
    foodDispensed = 1;
    lastDispenseTime = millis();
  }

  // Check if the pet has eaten within a specific time frame
  if (foodDispensed == 1 && millis() - lastDispenseTime > 5000){ //5 seconds
    float plateWeight = analogRead(FORCE_SENSOR);
    if (plateWeight < 50.0) { // Threshold to determine if food was eaten
      Serial.println("Empty plade");
    } else {
      Serial.println("Pet Eating");
    }
    foodDispensed = 0; // Reset for the next cycle
  }
}

void dispenseFood() {
  Serial.println("Dispensing Food...");
  foodServo.write(90); // Rotate servo to dispense food
  delay(500); //spense food for 3 seconds
  foodServo.write(0); // Reset servo position
}

void sendFoodStatus() {
  // Check plate weight
  int plateWeight = analogRead(FORCE_SENSOR);
  plateWeight = map(plateWeight, 0, 1023, 0, 255);
  int foodStatus = (plateWeight < 5.0) ? 0 : 1;

  // Check container level
  long duration;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  
  if (duration == 0) {
  	duration = 30000;
  }
  float distance = (duration * 0.034) / 2;
  int containerStatus = (distance > 200.0) ? 0 : 1; // 5 cm threshold

  // Send data to master
  Wire.write(foodStatus);
  Wire.write(containerStatus);
}