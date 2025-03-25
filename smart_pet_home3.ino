#include <Wire.h>

#define TEMP_SENSOR_PIN A0
#define DC_MOTOR_PIN 13

double temperature = 0;

void setup() {
  Serial.begin(9600);
  pinMode(TEMP_SENSOR_PIN, INPUT);
  pinMode(DC_MOTOR_PIN, OUTPUT);
  Wire.begin(10); 
  Wire.onRequest(sendTemperature); 
}

void loop() {
  double value = analogRead(TEMP_SENSOR_PIN);
  value = (value * 5.0) / 1024.0;
  temperature = (value - 0.5) * 100.0; 
  Serial.println(temperature);

  if (temperature > 30.0) { 
    int pwmValue = map(temperature, 30, 124, 100, 255); 
    pwmValue = constrain(pwmValue, 0, 255); 
    analogWrite(DC_MOTOR_PIN, pwmValue);
  } else {
    analogWrite(DC_MOTOR_PIN, 0); 
  }

}

void sendTemperature() {
  int tempToSend = map(temperature, -40, 124, 0, 255); 
  Wire.write(tempToSend); 
}