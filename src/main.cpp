#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define SOUND_VELOCITY 0.034

const char* ssid = "TB-0001";
const char* password = "Not Your Bot!";

const int IN1 = 12;
const int IN2 = 14;
const int IN3 = 0;
const int IN4 = 4;
const int ENA = 13;
const int ENB = 2;
const int trigPin = 5;
const int echoPin = 15;

long duration;
float distanceCm;
int speed = 200;
int motion = 1;

//  TODO: implement SSL for API connections

ESP8266WebServer server(80);

void stop(){
  motion = 0;
  server.send(200, "text/plain", "OK");
  Serial.println("MOTION: STOP");
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void fwd(){
  stop();
  motion = 1;
//  int speed = server.arg(0).toInt();
  server.send(200, "text/plain", "OK");
  Serial.println("MOTION: FORWARD");
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

}

void rev(){
  stop();
  delay(100);
  motion = -1;
  server.send(200, "text/plain", "OK");
  Serial.println("MOTION: REVERSE");
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


void left(){
  server.send(200, "text/plain", "OK");
  Serial.println("MOTION: LEFT");
  analogWrite(ENA, speed);
  analogWrite(ENB, 75);
  if(motion == 1){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }else{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
}

void right(){
  server.send(200, "text/plain", "OK");
  Serial.println("MOTION: RIGHT");
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  if(motion == 1){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }else{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
}

void status(){
  server.send(200, "text/plain", "STATUS");
}

void notFound(){
  server.send(200, "text/plain", "STATUS");
}

void read_distance(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delay(50);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  Serial.print("Distance: ");
  Serial.println(distanceCm);
  if(distanceCm < 20.0){
    //stop();
  }
}

void setup() {

  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);

  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  delay(100);

  server.on("/api/fwd", fwd);
  server.on("/api/rev", rev);
  server.on("/api/left", left);
  server.on("/api/right", right);
  server.on("/api/stop", stop);
  server.on("/api/status", status);
  server.onNotFound(notFound);

  server.begin();
  Serial.println("TankBot v0.1.0 Webserver: ONLINE");
}

void loop() {
server.handleClient();
}
