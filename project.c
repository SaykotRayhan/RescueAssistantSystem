#define BLYNK_PRINT Serial

#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
Adafruit_BMP085 bmp;

char auth[] = "af5d933193b64272bd7202c611cb0d25";
char ssid[] = "Md. Moinul";
char pass[] = "********";
int apin = A0;
int mq3 = 14;
int mqdata;
int analogData;
int pressure;

void setup()
{
  Serial.begin(9600);
  pinMode(apin, INPUT);
  pinMode(mq3, INPUT);
  Blynk.begin(auth, ssid, pass);

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

}

void loop()
{

  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print("Altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" meters");

  Serial.print("Pressure at sealevel (calculated) = ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");

 Serial.print("Real altitude = ");
  Serial.print(bmp.readAltitude(101500));
  Serial.println(" meters");

  Serial.println();
  delay(500);
  pressure = bmp.readPressure() / 100;
  analogData = analogRead(apin);

  mqdata = digitalRead(mq3);
  Serial.println(analogData);
  Blynk.virtualWrite(V5, bmp.readTemperature());
  Blynk.virtualWrite(V6, pressure);
  Blynk.virtualWrite(V7, analogData);
  
  Serial.println(mqdata);
  
  if (bmp.readTemperature() > 30) { // compeare temperature
    Blynk.email("hs3024912@gmail.com", "ESP8266 Alert", "Temperature over 30C!");
    Blynk.notify("ESP8266 Alert - Temperature over 30C!");
  }
  
  if (analogData > 550) {// compeare gas
    Blynk.email("hs3024912@gmail.com", "ESP8266 Alert", "GAS OR SMOKE DETECTED!");
    Blynk.notify("ESP8266 Alert - GAS OR SMOKE DETECTED!");
  }
  Blynk.run();
}