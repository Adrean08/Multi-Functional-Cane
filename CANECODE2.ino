#include <SoftwareSerial.h>
#include <TinyGPS++.h>
 
int buttonpin = 6;
int buzzer = 8;

SoftwareSerial mySerial(3, 2);

const int trigPin1 = A1;
const int echoPin1 = A0;
long duration1;

int distance1;
const int trigPin2 = A2;
//const int echoPin2 = A3;
long duration2;
int distance2;
int buttonState=0;

int playback = 4;

//gps
static const int RXPin = 10, TXPin = 11;// Here we make pin 4 as RX of arduino & pin 3 as TX of arduino 
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);


void setup()
 
{
  //buzzer
  pinMode(buzzer, OUTPUT);
 //ultra sonic 1
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(5, OUTPUT);
  pinMode(playback, OUTPUT);
  pinMode(A4, INPUT_PULLUP);// water sensor
 
  Serial.begin(9600);
 
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(buttonpin, INPUT);
 
   Serial.begin(9600);
  
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  delay(1000);
 
  mySerial.println("AT+CNMI=1,2,0,0,0");
  delay(1000);

  //gps
   Serial.begin(9600);
    ss.begin(GPSBaud);
}
 
void loop()
 
{ 
  
 
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  Serial.print("Distance1: ");
  Serial.println(distance1);
  delay(1000);
  
  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  
  duration2 = pulseIn(echoPin1, HIGH);
  distance2 = duration2 * 0.034 / 2;  //distance formula
 
  Serial.print("Distance2: ");
  Serial.println(distance2);

  digitalWrite(playback,LOW);

   int sensorValue = digitalRead(A4);
 
  if (sensorValue == 1) {
 
    digitalWrite(5, HIGH);
    Serial.println("wet condition");
    delay(500);

    digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer,LOW);
    delay(500);
 
  }
 
 


  buttonState = digitalRead(buttonpin);
 
  if (digitalRead(buttonpin) == HIGH)
 
  {
 
    Serial.println("button pressed");
    delay(2000);
    
    SendMessage();
    Serial.println(buttonState);
  delay(10);

   
  }
/////
 //////

   if (distance1 <= 75 && distance1>=51 || distance2 <= 75 && distance2>=51) {
   
 
    

    digitalWrite(buzzer,HIGH);
    delay(300);
    digitalWrite(buzzer,LOW);
    delay(500);

    digitalWrite(playback,HIGH);
   
  
  }

  else if (distance1 <= 50 && distance1>=26 || distance2 <= 50 && distance2>=26) {
   
 
   

    digitalWrite(buzzer,HIGH);
    delay(300);
    digitalWrite(buzzer,LOW);
    delay(300);
    digitalWrite(playback,HIGH);
  
 
  }
 
  else if (distance1 <= 25 && distance1>=1 || distance2 <= 25 && distance2>=1) {
 
    

    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer,LOW);
    digitalWrite(playback,HIGH);
    
 
  }
 
 else

    digitalWrite(buzzer,LOW);
    digitalWrite(5, LOW);
    digitalWrite(playback,LOW);

  
 
 
}
  
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
} 

void SendMessage(){
 

  mySerial.println("AT+CMGF=1"); 
  updateSerial();  //Sets the GSM Module in Text Mode
 
 
  mySerial.println("AT+CMGS=\"+6394690u92862\""); // enter your phone number here (prefix country code)
  updateSerial();
  mySerial.print("Notice!"); // enter your message here
  updateSerial();
  mySerial.write(26);
 
  
  
  }

  
void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    mySerial.print(gps.location.lat(), 6);
    mySerial.print(F(","));
    mySerial.print(gps.location.lng(), 6);
  }
  else
  {
    mySerial.print(F("INVALID"));
  }

  Serial.print(F("  Date "));
  if (gps.date.isValid())
  {
    mySerial.print(gps.date.month());
    mySerial.print(F("/"));
    mySerial.print(gps.date.day());
    mySerial.print(F("/"));
    mySerial.print(gps.date.year());
  }
  else
  {
    mySerial.print(F("INVALID"));
  }

  
  Serial.println();
}
