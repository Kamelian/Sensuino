const int PinTrigger = 10; // HC-SR04 Trigger Pin
const int PinEcho = 11; // HC-SR04 Echo Pin
const int Led = 13; // Pin LED of onboard Arduino UNO R1 board LED
const int SampleRate = 4; // Number of samples per second
const int TotalSamplesOK = 2; // number of samples that triggers a "cumpute average distance" event
const int distance_min=5; // minimum distance in cm
const int distance_max=390; // maximum distance in cm
const long f_smooth=1.0; //smoothening factor

long DistancePoint[16], duration, distance_cm, average_distance, speed_cm_s, position_1, position_2, speed_1, speed_2, smoothedSpeed;
int led, i, average_distance_counter, display_counter;

void setup() 
{
  pinMode(PinTrigger, OUTPUT); // HC-SR04 Trigger
  pinMode(PinEcho, INPUT); // HC-SR04 Echo
  pinMode(Led, OUTPUT); // LED

  Serial.begin(9600); // open the serial port at 9600 bps

  delay(250); // take a break, enjoy the view....

  i = 0;
  average_distance = 1;
  distance_cm = 1;
  display_counter=0;
}

void loop() 
{
  HC_SR04_Distance();
  delay((25/4)*SampleRate);

  DistancePoint[i] = (distance_cm);

  i++;
  if (i > (SampleRate)) {i = 0;}

  if (i == (SampleRate))
  {
    display_counter = 0;
    CalculateAverageDistance();
    CalculateSpeed();
    SpeedSmoothening();

    Serial.println("------------------------");
    Serial.print("Distancia = ");
    Serial.print(average_distance);
    Serial.println(" cm");
    Serial.print("Velocidade = ");
    Serial.print(speed_cm_s);
    Serial.println(" cm/s");
    Serial.print("Velocidade Suavizada = ");
    Serial.print(smoothedSpeed);
    Serial.println(" cm/s");
  }
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
      average_distance_counter++;
      average_distance = average_distance + DistancePoint[x];
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

void CalculateSpeed()
{
  position_1=position_2;
  position_2=average_distance;
  if (average_distance!=0)
  {
    speed_cm_s=position_2-position_1;
  }
  else
  {
    speed_cm_s=0;
  }
}

void SpeedSmoothening()
{
 speed_1 = speed_2;
 speed_2 = speed_cm_s;
 smoothedSpeed = speed_1 + (speed_2 - speed_1) * f_smooth;
}
