#include <CheapStepper.h>
#include <Servo.h>

Servo servo1;
#define ir 5
#define buzzer 12        // Passive buzzer signal pin
#define waterSensor A1   // Water sensor pin
#define potPin A0        // Soil moisture sensor pin

int soil = 0;
int fsoil = 0;

CheapStepper stepper(8, 9, 10, 11);

void setup() {
  Serial.begin(9600);
  pinMode(ir, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(waterSensor, INPUT);
  servo1.attach(7);
  stepper.setRpm(17);

  // Initialize Servo Position
  servo1.write(180);  
  delay(1000);
  servo1.write(70);
  delay(1000);
}

void loop() {

  // IR Sensor Detection
  if (digitalRead(ir) == 0) {
    Serial.println("IR Sensor Triggered!");
    tone(buzzer, 1000);  // Beep at 1kHz for IR detection
    delay(500);
    noTone(buzzer);      // Stop the buzzer
    delay(1000);

    // Read Soil Moisture Level
    int soil = 0;
    for (int i = 0; i < 3; i++) {
      soil = analogRead(potPin);
      soil = constrain(soil, 485, 1023);
      fsoil = (map(soil, 485, 1023, 100, 0)) + fsoil;
      delay(75);
    }
    fsoil = fsoil / 3;
    
    Serial.print("Soil Moisture: ");
    Serial.print(fsoil);
    Serial.println("%");

    // Soil Moisture Check
    if (fsoil > 20) {
      Serial.println("Soil is Dry, Watering...");
      stepper.moveDegreesCW(120);
      delay(1000);
      servo1.write(180);
      delay(1000);
      servo1.write(70);
      delay(1000);
      stepper.moveDegreesCCW(120);
      delay(1000);
    } else {
      Serial.println("Soil is Wet, No Watering Needed!");
      tone(buzzer, 1500);  // Higher-pitched beep for dry soil
      delay(500);
      noTone(buzzer);
      delay(1000);
      servo1.write(180);
      delay(1000);
      servo1.write(70);
      delay(1000);
    }
  }

  // Read Water Sensor Value   
  int waterValue = analogRead(waterSensor);
  Serial.print("Water Sensor: ");
  Serial.println(waterValue);

  // Water Detection
  if (waterValue > 800) {  // Adjust threshold based on testing
    Serial.println("Water Detected!");
    tone(buzzer, 2000);  // Higher-pitched beep for water detection
    delay(500);
    noTone(buzzer);
  }

  delay(1000);
}
