//including the libraries
#include <AFMotor.h>
#include <SoftwareSerial.h>
#include <dht.h>
#define dht_apin A2
#define buzzerPin A3

//defining motors
AF_DCMotor motor4(4);
AF_DCMotor motor3(3);

// defining variables and classes
dht DHT;
SoftwareSerial bt(A0, A1); // RX / TX
String state;
String carStatus;

void setup() {
  bt.begin(9600);
}

void loop() {
  // read temperature and humidity values
  DHT.read11(dht_apin);
  double temp = DHT.temperature;
  double humidity = DHT.humidity;

  // trigger buzzer on overheating
  if (temp > 40) {
    carStatus = "Overheat";
    tone(buzzerPin, 500);
  } else {
    carStatus = "Good";
    noTone(buzzerPin);
  }

  // send data to mobile app
  bt.print(temp);
  bt.print(" C");
  bt.print("&");
  bt.print(humidity);
  bt.print(" %");
  bt.print("&");
  bt.print(carStatus);
  bt.print("&");

  // read command from mobile app
  while (bt.available())
  {
    char c = bt.read();
    state += c;
  }

  if (state == "up") {
    motor4.run(FORWARD);
    motor4.setSpeed(150);
    motor3.run(FORWARD);
    motor3.setSpeed(150);
  }

  if (state == "down") {
    motor4.run(BACKWARD);
    motor4.setSpeed(150);
    motor3.run(BACKWARD);
    motor3.setSpeed(150);
  }

  if (state == "left") {
    motor4.run(BACKWARD);
    motor4.setSpeed(150);
    motor3.run(FORWARD);
    motor3.setSpeed(150);
  }

  if (state == "right") {
    motor4.run(FORWARD);
    motor4.setSpeed(150);
    motor3.run(BACKWARD);
    motor3.setSpeed(150);
  }

  if (state == "stop") {
    motor4.run(RELEASE);
    motor4.setSpeed(0);
    motor3.run(RELEASE);
    motor3.setSpeed(0);
  }

  state = "";
  delay(1000);
}