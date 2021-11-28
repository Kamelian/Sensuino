#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

const int Led = 13; // Pin LED of onboard Arduino UNO R1 board LED
float t, h;

void setup()
{
  dht.begin();
  pinMode(Led, OUTPUT); // LED

  Serial.begin(9600);      // open the serial port at 9600 bps
  Serial.println("POWER ON!"); // Hello World!!!
  Serial.println("Sensuino v4.0a");
  Serial.println(" ");

  delay(1000); // take a break, enjoy the view....
}

void loop()
{
  h = dht.readHumidity();
  t = dht.readTemperature();

  Serial.println("------------------------");

  if (isnan(t) || isnan(h))
  {
    Serial.println("Failed to read from DHT");
  }
  else
  {
    Serial.print("Humidade: ");
    Serial.print(h);
    Serial.println("%");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println("*C");
  }
  delay(100);
}
