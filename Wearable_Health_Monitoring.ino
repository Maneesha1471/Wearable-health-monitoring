const int PULSE_SENSOR_PIN = A0;  
const int LED_PIN = 13;     // On-board LED 
const int THRESHOLD = 550; // Threshold 
String number="9398786846";

#include <PulseSensorPlayground.h>
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
#include <SoftwareSerial.h>
SoftwareSerial GSMSerial(2,3);
// Create PulseSensorPlayground object
PulseSensorPlayground pulseSensor;
 
void setup() 
{
lcd.begin(16,2);
Serial.begin(9600);
GSMSerial.begin(9600);
dht.begin();
 
  // Configure PulseSensor
pulseSensor.analogInput(PULSE_SENSOR_PIN);
pulseSensor.blinkOnPulse(LED_PIN);
pulseSensor.setThreshold(THRESHOLD);
 
  // Check if PulseSensor is initialized
if (pulseSensor.begin()) 
  {
Serial.println("PulseSensor object created successfully!");
lcd.setCursor(0,0);
lcd.print("  pulse sensor  ");
lcd.setCursor(0,1);
lcd.print("     begins     ");
delay(3000);
lcd.clear();

  }
}
void SendMessage(String msg , String number)
{
  GSMSerial.println("AT+CMGF=1");                                 //Sets the GSM Module in Text Mode
  delay(1000);                                                    // Delay of 1000 milli seconds or 1 second
  GSMSerial.println("AT+CMGS=\"+91"+number+"\"\r");               // Replace x with mobile number
  delay(1000);
  GSMSerial.println(msg);                                         // The SMS text you want to send
  delay(1000);
  GSMSerial.println((char)26);                                    // ASCII code of CTRL+Z
  delay(1000);
}
 
void loop() 
{
float t = dht.readTemperature();
  // Get the current Beats Per Minute (BPM)
int currentBPM = pulseSensor.getBeatsPerMinute();
 
  // Check if a heartbeat is detected
  
if (pulseSensor.sawStartOfBeat()) 
  {
//Serial.println("HeartBeat Happened!");
//Serial.print("BPM: ");
//Serial.println(currentBPM);
//Serial.print("Temp:");
//Serial.println(t);
lcd.setCursor(0,0);
lcd.print("BPM: ");
lcd.print(currentBPM);
lcd.setCursor(0,1);
lcd.print("Temp:");
lcd.print(t);
delay(3000);
lcd.clear();
if(t<=45 && currentBPM<=100){
    lcd.setCursor(0,0);
    lcd.print(" Status : Safe  ");
  }
  else{
    lcd.setCursor(0,0);
    lcd.print(" Status : Alert ");
    SendMessage("He/She has High Temperature and BPM",number);
  }

  }
delay(20);
}
