#include <ESP32Servo.h>

#define TRIG_PIN 5
#define ECHO_PIN 18
#define RELAY_PIN 23
#define BUZZER_PIN 19
#define LED_PIN 2
#define SERVO_PIN 25   // Servo acts as the dam gate valve

long duration;
int distance;

Servo valveServo;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH); // Motor ON initially

  valveServo.attach(SERVO_PIN);
  valveServo.write(0);           // Valve OPEN initially

  Serial.begin(9600);
}

void loop() {
  // Ultrasonic trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Water Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // -------- TANK FULL --------
  if (distance <= 5) {
    digitalWrite(RELAY_PIN, LOW);    // Motor OFF
    digitalWrite(LED_PIN, HIGH);     // LED ON
    digitalWrite(BUZZER_PIN, HIGH);  // Buzzer ON

    valveServo.write(90);            // CLOSE valve

    Serial.println("DAM Full - Valve Closed");
  }

  // -------- TANK LOW --------
  else if (distance >= 10) {
    digitalWrite(RELAY_PIN, HIGH);   // Motor ON
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    valveServo.write(0);             // OPEN valve

    Serial.println("Tank Low - Valve Open");
  }

  // -------- NORMAL LEVEL --------
  else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);

    // Keep valve position unchanged
    Serial.println("Tank Level Normal");
  }

  delay(1000);
}
