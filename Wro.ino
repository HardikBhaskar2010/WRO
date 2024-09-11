#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_GPS.h>

Servo driveMotor;
Servo flyMotor;
Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_GPS GPS(&Wire);

const int trigPin = 9;
const int echoPin = 10;
const int driveMotorPin = 3;
const int flyMotorPin = 5;

void setup() {
  Serial.begin(9600);
  driveMotor.attach(driveMotorPin);
  flyMotor.attach(flyMotorPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  if(!bno.begin()) {
    Serial.print("No BNO055 detected");
    while(1);
  }
  
  GPS.begin(9600);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  
  if (distance < 20) {
    driveMotor.write(0); // Stop the drive motor
    flyMotor.write(180); // Start the fly motor
  } else {
    driveMotor.write(180); // Move the drive motor
    flyMotor.write(0); // Stop the fly motor
  }
  
  sensors_event_t event;
  bno.getEvent(&event);
  Serial.print("Orientation: ");
  Serial.print(event.orientation.x);
  Serial.print(" ");
  Serial.print(event.orientation.y);
  Serial.print(" ");
  Serial.print(event.orientation.z);
  Serial.println();
  
  char c = GPS.read();
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) {
      return;
    }
  }
  
  Serial.print("Location: ");
  Serial.print(GPS.latitude, 4);
  Serial.print(GPS.lat);
  Serial.print(", ");
  Serial.print(GPS.longitude, 4);
  Serial.println(GPS.lon);
  
  delay(500);
}
