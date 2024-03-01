#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>  
#include <HC_SR04.h>          
#include <MFRC522.h>        
#include <SPI.h>             


#define ULTRASONIC_TRIGGER_PIN 5
#define ULTRASONIC_ECHO_PIN 6
#define SERVO_PIN 9
#define RFID_SS_PIN 10
#define RFID_RST_PIN 9

#define ACCESS_DISTANCE 30  
#define RFID_CHECK_INTERVAL 5000  
#define SERVO_ROTATION_ANGLE 90   

Servo doorServo;
HC_SR04 ultrasonic(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN);
MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);

void setup() {
  Serial.begin(9600);
  doorServo.attach(SERVO_PIN);
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if (isSomeoneInRoom()) {
    grantAccess();
  }

  checkRFID();

  delay(1000);
}

bool isSomeoneInRoom() {
  float distance = ultrasonic.distance();
  return distance < ACCESS_DISTANCE;
}

void grantAccess() {
  doorServo.write(SERVO_ROTATION_ANGLE);
  delay(5000);
  doorServo.write(0);
}

void checkRFID() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    Serial.println("RFID Card Detected!");
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
