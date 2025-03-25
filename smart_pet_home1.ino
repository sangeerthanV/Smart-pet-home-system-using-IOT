#include <Wire.h>

#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);

#define LED_RED 6
#define LED_YELLOW 5
#define LED_GREEN 4
#define BUZZER 3

void setup() {
  Serial.begin(9600);
  Wire.begin(); 
  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(LED_GREEN, HIGH);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(300);
}

void loop() {
  int inches = 0;
  int cm = 0;
  lcd.clear();


    Wire.requestFrom(8, 2);
    if (Wire.available()) {
      cm = Wire.read();

      Serial.print("Distance: ");
      Serial.print(cm);
      Serial.println(" cm");

      lcd.setCursor(0, 0);

      if (cm > 255 || cm == 74) { 
       
        
        lcd.print("Pet: Absent");
        Serial.println("Pet: Absent");
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_YELLOW, HIGH);
        tone(BUZZER, 50);
        delay(100);
        noTone(BUZZER);
      } else if (cm > 0 && cm <= 255) {
         lcd.print("Pet: Present");
        Serial.println("Pet: Present");
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_YELLOW, LOW);
        noTone(BUZZER);
      }
    } else {
      Serial.println("No responses from Slave 1");
      lcd.setCursor(0, 0);
      lcd.print("Pet: N/A");
      noTone(BUZZER);
    }



  delay(100);
  lcd.clear();
  
    // Request feeding status from Slave 2
  Wire.requestFrom(9, 2); // Slave 2 address: 9, request 2 bytes of data
  if (Wire.available() == 2) {
    int foodStatus = Wire.read();  
    int containerStatus = Wire.read(); 

    lcd.setCursor(0, 0);
    if (foodStatus == 0) {
      lcd.print("Empty Plate");
      Serial.println("Pet Not Eating");
    } else {
      lcd.print("Pet Eating");
      Serial.println("Pet Eating");
    }

    lcd.setCursor(0, 1);
    if (containerStatus == 0) {
      lcd.print("Food Low!");
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      Serial.println("Food Container Low!");
      tone(BUZZER, 50);
      delay(100);
      noTone(BUZZER);
    } else {
      lcd.print("Food OK");
      digitalWrite(LED_RED, LOW);
      Serial.println("Food Container OK");
      noTone(BUZZER);
    }
  }
  delay(100); // Polling interval
  lcd.clear();

  Wire.requestFrom(10, 1);
  if (Wire.available()) {
    int tempByte = Wire.read();
    double temperature = map(tempByte, 0, 255, -40, 124);
    Serial.print("Temperature: ");
    Serial.println(temperature);
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");
  }
  
}

