#define BLYNK_PRINT Serial //Defines the object that is used for printing

#include <Wire.h>  //This library allows you to communicate with devices on the Arduino boards board.
#include <Adafruit_BMP085.h> // for bmp180
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h> 
Adafruit_BMP085 bmp;

//Blynk Aut
char auth[] = "af5d933193b64272bd7202c611cb0d25";

// WIFI connet.
char ssid[] = "Md. Moinul";
char pass[] = "12345678";

int apin = A0;
int mq3 = 2;
int mqdata;
int analogData;

// ultrasonic sensor
int trigPin = 14;    // Trigger;output
int echoPin = 12;    // Echo ; input
long duration;
int cm;

//int sensorData;
int pressure;
void setup()
{
  // Debug console
  Serial.begin(9600); //opens serial port, sets data rate to 9600 bps
  pinMode(apin, INPUT);// Configures the specified pin to behave either as an input or an output
  pinMode(mq3, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Blynk.begin(auth, ssid, pass);


  
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

}

void loop()
{
digitalWrite(trigPin, LOW); //Write a HIGH or a LOW value to a digital pin, sets the pin off
  delayMicroseconds(5);      //pauses for 5 microseconds
  digitalWrite(trigPin, HIGH);     //When the triggering pin is set high the sensor generates 40 kHz ultrasonic wave which is 8 periods long. 
  delayMicroseconds(10);	   //At that moment the echo pin becomes high and remains high until the reflected sound is reached back to the sensor.
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
 cm = (duration/2) / 29.1; // cm convertion
 Serial.print("Distance = ");
  Serial.print(cm);
  Serial.println(" cm");

  
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");


  // pressure of 1013.25 millibar = 101325 Pascal
  Serial.print("Altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" meters");

  Serial.print("Pressure at sealevel (calculated) = ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");

  // measurement of altitude
  
  Serial.print("Real altitude = ");
  Serial.print(bmp.readAltitude(101500));
  Serial.println(" meters");

  Serial.println();
  delay(2000);
  pressure = bmp.readPressure() / 100;
  analogData = analogRead(apin);

  
  mqdata = digitalRead(mq3);
  Serial.println(analogData);
  Blynk.virtualWrite(V5, bmp.readTemperature());
  Blynk.virtualWrite(V6, pressure); //sending the hardware to the server using virtual pin v6.
  Blynk.virtualWrite(V7, analogData);
  Blynk.virtualWrite(V8, cm);
  
 
  Serial.println(mqdata);
  
  if (bmp.readTemperature() > 30) { // compeare temperature
    Blynk.email("hs3024912@gmail.com", "ESP8266 Alert", "Temperature over 30C!");
    Blynk.notify("RAS ALERT - Temperature over 30C!");
  }
  
  if (analogData > 550) {// compeare gas
    Blynk.email("hs3024912@gmail.com", "ESP8266 Alert", "GAS OR SMOKE DETECTED!");
    Blynk.notify("RAS ALERT - GAS OR SMOKE DETECTED!");
  }
  if (cm < 20) {// distance
    Blynk.email("hs3024912@gmail.com", "ESP8266 Alert", "Object Coming close!");
    Blynk.notify("RAS ALERT - Object Coming close!");
  }
  Blynk.run();
}