#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

#include "DHT.h"
#define DHTPIN 14 
#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

const int PinTrigger = 10; // HC-SR04 Trigger Pin
const int PinEcho = 11; // HC-SR04 Echo Pin
const int Led = 13; // Pin LED of onboard Arduino UNO R1 board LED
const int SampleRate = 4; // Number of samples per second
const int TotalSamplesOK = 2; // number of samples that triggers a "cumpute average distance" event
const int distance_min=5; // minimum distance in cm
const int distance_max=390; // maximum distance in cm
const int speed_limit=20; // speed limit in cm/s

long DistancePoint[16], distance_point_error[16], duration, distance_cm, average_distance;
int led, i, error, text_size, average_distance_counter, display_counter, temperature, humidity;
String text;
float t, h;
 
void setup()
{

  LCD_Init();

  dht.begin();

  pinMode(PinTrigger, OUTPUT); // HC-SR04 Trigger
  pinMode(PinEcho, INPUT); // HC-SR04 Echo
  pinMode(Led, OUTPUT); // LED


  Serial.begin(9600);      // open the serial port at 9600 bps
  Serial.println("Sensuino v1.0a");
  Serial.println(" ");
  Serial.println("POWER ON!"); // Hello World!!!
  
  delay(1500); // take a break, enjoy the view....

  Serial.println(" ");
  Serial.println("Trabalho para a disciplina de Fisica Aplicada a Computacao,  ");
  Serial.println("professor Nuno Pereira, ESTIG - IPBEJA");
  Serial.println(" ");
  Serial.println("Desenvolvido por Joao Faria para o estudo dos sensores/modulos:");
  Serial.println("   HC-SR04 - Medidor de distancia por ultrason");
  Serial.println("   DHT21(AM2301) - Medidor de humidade e temperatura");
  Serial.println("   HC-05 - Modulo Bluetooth");
  Serial.println("   Hitachi 44780 - Modulo LCD");
  Serial.println("   PCF8574 - Remote 8-Bit I/O Expander for I2C Bus");
  Serial.println(" ");
  
  delay(5000); // take a break, enjoy the view....

  Serial.println("------------------------");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distancia: --- ");
  lcd.setCursor(0, 1);
  lcd.print("H: --   T: --  ");      

  i = 0;
  average_distance = 1;
  distance_cm = 1;
  display_counter=0;
 
}

void loop()
{


  HC_SR04_Distance();
  
  delay(1000/SampleRate);
  //digitalWrite(led, LOW);

  lcd.setBacklight(HIGH);

  error = 0; 
  if (distance_cm <= distance_min) { error = 1;}
  if (distance_cm >= distance_max) { error = 2;}
  DistancePoint[i] = (distance_cm);
  distance_point_error[i] = error;
  
  i++;
  if (i > (SampleRate)) {i = 0;}

  display_counter = display_counter + 1;

  if (display_counter == SampleRate)
  {
    display_counter = 0;
    CalculateAverageDistance();
    ShowAverageDistance();
    Check_Temperature_Humidity();
    Show_Temperature();

  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command
    Serial.println(" ");
    Serial.println("------------------------");
    Serial.println(" ");
  
    Serial.print("Distancia : ");
    Serial.print(average_distance);
    Serial.println(" cm");

    if (isnan(t) || isnan(h)) 
    {
        Serial.println("Failed to read from DHT");
    } 
    else
    {
      Serial.print("Humidade: "); 
      Serial.print(h);
      Serial.println(" %\t");
      Serial.print("Temperatura: "); 
      Serial.print(t);
      Serial.println(" *C");
    }
    Serial.println(" ");

    Serial.println("------------------------");

  }


}

void LCD_Init()
{
  lcd.begin(16, 2); // set lcd dimension
  lcd.setBacklight(HIGH);
  lcd.clear(); // clear LCD
  lcd.setCursor(0, 0);
  lcd.print("Sensuino v1.0a");
  lcd.setCursor(0, 1);
  lcd.print("  Joao Faria");  
}


void HC_SR04_Distance()
{
  digitalWrite(PinTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(PinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(PinTrigger, LOW);
  duration = pulseIn(PinEcho, HIGH, 23000);
  distance_cm = duration/58; //Distance in cm;
}

void CalculateAverageDistance()
{
  average_distance = 0;
  average_distance_counter = 0;
  for (int x = 0; x < (SampleRate+1); x++)
  {
    if (distance_point_error[x] == 0)
    {
      average_distance_counter++;
      average_distance = average_distance + DistancePoint[x];
    }
  }

  if (average_distance_counter >= TotalSamplesOK)
  {
    average_distance = average_distance / average_distance_counter;
  }
  else
  {
    average_distance = 0;
  }

}


void ShowAverageDistance()
{
    
    if (average_distance ==0 )
    { // Show Error 
      text = " --- ";
    }
    else
    {
      text = String(average_distance);
    }

  
    text_size=text.length();
    switch (text_size)
    {
      case 1:
        lcd.setCursor(11, 0);
        lcd.print(text);
        lcd.setCursor(12, 0);
        lcd.print("cm  ");
        break;
      case 2: // "xxcm"
        lcd.setCursor(11, 0);
        lcd.print(text);
        lcd.setCursor(13, 0);
        lcd.print("cm ");
        break;
      case 3:
        lcd.setCursor(11, 0);
        lcd.print(text);
        lcd.setCursor(14, 0);
        lcd.print("cm");
        break;
      default:
        lcd.setCursor(11, 0);
        lcd.print("---  ");
        break;
    }
  
  
}

void Check_Temperature_Humidity()
 {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!

}

void Show_Temperature()
{
    if (isnan(t) || isnan(h)) 
    {
        Serial.println("Failed to read from DHT");
        lcd.setCursor(0, 1);
        lcd.print("H: --   T: --  ");      
    } 
    else
    {

      humidity=(int) h;
      text = String(humidity);
      text_size=text.length();
      switch (text_size)
      {
        case 1:
          lcd.setCursor(3, 1);
          lcd.print(text);
          lcd.setCursor(4, 1);
          lcd.print("% ");
          break;
        case 2:
          lcd.setCursor(3, 1);
          lcd.print(text);
          lcd.setCursor(5, 1);
          lcd.print("% ");
          break;
        case 3:
          lcd.setCursor(2, 1);
          lcd.print(text);
          lcd.setCursor(5, 1);
          lcd.print("%");
          break;
        default:
          lcd.setCursor(2, 1);
          lcd.print(text);
          lcd.setCursor(5, 1);
          lcd.print("%");
          break;
      }

      //temperature=(int) t;
      //text = String(temperature);
      text = String(t);
      text_size=text.length();
      switch (text_size)
      {

        case 3:
          lcd.setCursor(13, 1);
          lcd.print(text);
          lcd.setCursor(15, 1);
          lcd.print("C");
          break;
        case 4:
          lcd.setCursor(12, 1);
          lcd.print(text);
          lcd.setCursor(15, 1);
          lcd.print("C");
          break;
        case 5:
          lcd.setCursor(11, 1);
          lcd.print(text);
          lcd.setCursor(15, 1);
          lcd.print("C");
          break;
        case 6:
          lcd.setCursor(10, 1);
          lcd.print(text);
          lcd.setCursor(15, 1);
          lcd.print("C");
          break;
        default:
          lcd.setCursor(11, 1);
          lcd.print(text);
          lcd.setCursor(15, 1);
          lcd.print("C");
          break;
      }



    }


}

