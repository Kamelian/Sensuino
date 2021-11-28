const int PinTrigger=10;
const int PinEcho=11;
const int Led=13;

void setup() 
{
  pinMode(PinTrigger,OUTPUT); // HC-SR04 Trigger
  pinMode(PinEcho,INPUT); // HC-SR04 Echo
  pinMode(Led,OUTPUT); // On-Board LED
  
  Serial.begin(9600); // open the serial port at 9600 bps
  Serial.println("");
  Serial.println("POWER ON!"); // Hello World!!!
  Serial.println("");
  
  delay(250); // take a break, enjoy the view....
}

void loop()
{
  Serial.print("Distancia - ");
  Serial.print(Distance());
  Serial.println(" cm");
  Serial.println("------------------");
  
  delay(2000);
}

long Distance() //HC_SR04 Distance 
{
  long duration, distance;
  
  digitalWrite(PinTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(PinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(PinTrigger, LOW);
  
  duration = pulseIn(PinEcho,HIGH); // mesuares the time the PinEcho is recieving 
  distance = duration /58; // Distance in cm. 
                           // The value 1/58 is given in the data sheet has the value to obtain mesurements in cm.
    
  return distance;
}
