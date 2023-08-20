
// *** MAIN SETTINGS ***
// Replace this block with correct template settings.
// You can find it for every template here:
//
//   https://blynk.cloud/dashboard/templates

#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKE"

// Your WiFi credentials.
// Set password to "" for open networks.
#define WIFI_SSID             "YOUR_SSID"
#define WIFI_PASS             "YOUR_SSID_PASSWORD"

#define BLYNK_PRINT Serial  // Comment this out to disable serial monitor prints

// Adding the required libraries
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <ESP32Servo.h>
#define DHTTYPE DHT11
#define DHTPIN 25
#define GAS_SENSOR_PIN 33
#define SERVO_PIN 18
 int enAPin = 14;   // ENA
 int in1Pin = 27;   // Input 1
 int in2Pin = 26;   // Input 2
 int doorPin = 15;




const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

#define BUZZER_PIN 19  // Pin connected to the buzzer

int ledpin = 4;
int ledpin2 = 2;
DHT dht(DHTPIN, DHTTYPE);
int thresholdValue = 1500;
Servo myservo;
int servoPosition = 90;  // Initial position of the servo
int doorState;
BlynkTimer timer;

void setup()
{
  // Debug console
  Serial.begin(115200);                         // Make sure you have the same baud rate in your serial monitor set up
  pinMode(ledpin, OUTPUT);                    // Setting LED PIN to control it
  pinMode(ledpin2,OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(doorPin,INPUT_PULLUP);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
  dht.begin();
  timer.setInterval(1000L, checkGasValue);
   pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);


    ledcSetup(pwmChannel, freq, resolution);
    ledcAttachPin(enAPin, pwmChannel);
  myservo.setPeriodHertz(50);  // Set the servo motor frequency
  myservo.attach(SERVO_PIN, 500, 2500);  // Attach the servo motor to the pin and set pulse width limits
  myservo.write(0);  // Set the initial position of the servo to 0 degrees

 

}

void loop()
{
  Blynk.run(); // Blynk magic happens here
   float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
 
  //  Serial.print("Temperature: ");
  // Serial.print(temperature);
  // Serial.print(" °C, Humidity: ");
  // Serial.print(humidity);
  // Serial.println(" %");

  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);

  doorState = digitalRead(doorPin);
  if(doorState == HIGH){
    Serial.println("The door is open");
     Blynk.logEvent("your_event");
  } else {
    // Serial.println("The door is closed");
    
  }
  // Serial.println(enAPin);
  // //  Serial.println(in1Pin);
  // //   Serial.println(in2Pin);



  timer.run();


  
}

BLYNK_WRITE(V2){
  int value = param.asInt();  // Get the value from the app
    if (value == 1)
  {
    digitalWrite(ledpin, HIGH);
  }
  else
  {
    digitalWrite(ledpin, LOW);
   // Set the LED pin based on the value
}
}
BLYNK_WRITE(V3){
  int value = param.asInt();  
    if (value == 1)
  {
    digitalWrite(ledpin2, HIGH);
  }
  else
  {
    digitalWrite(ledpin2, LOW);
   // Set the LED pin based on the value
}
}
void checkGasValue()
{
  int gasValue = analogRead(GAS_SENSOR_PIN); 
  // Serial.println(gasValue);
  Blynk.virtualWrite(V4, gasValue);  

  if (gasValue > thresholdValue)
  {
    Blynk.logEvent("your_event");  
  }
}
BLYNK_WRITE(V5)  
{
  int buttonState = param.asInt();
  if (buttonState == HIGH) {
    myservo.write(90);  
 

  } else {
    myservo.write(0);  

  }
    
}
BLYNK_WRITE(V7)
{
 int motorSpeed = param.asInt();
  dutyCycle = map(motorSpeed, 0, 255, 0, 255);
  ledcWrite(pwmChannel, dutyCycle);

  // Move the DC motor forward
  if (dutyCycle > 0) {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
  } else {

    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
  }
}
BLYNK_WRITE(V8){
  int value = param.asInt();
  if(value==1){
    digitalWrite(BUZZER_PIN, HIGH);
  }
  else
  {
    digitalWrite(BUZZER_PIN, LOW);
   
}
}




