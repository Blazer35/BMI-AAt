//ThatsEngineering
//Sending Data from Arduino to NodeMCU Via Serial Communication
//Arduino code
#include <PulseSensorPlayground.h>
//DHT11 Lib
#include <DHT.h>

//Arduino to NodeMCU Lib
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
int PulseSensorPurplePin = A0;
int myBPM;
int Threshold = 550;  
int Signal;

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(5, 6);

//Initialisation of DHT11 Sensor
#define DHTPIN 4
DHT dht(DHTPIN, DHT11);
float temp;
float hum;

void setup() {
  Serial.begin(9600);

  dht.begin();
  nodemcu.begin(9600);
  delay(1000);

  Serial.println("Program started");
}

void loop() {

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  //Obtain Temp and Hum data
  dht11_func();
  Signal = analogRead(PulseSensorPurplePin);


  //Assign collected data to JSON Object
  data["humidity"] = hum;
  data["temperature"] = temp; 

  //Send data to NodeMCU
  data.printTo(nodemcu);
  jsonBuffer.clear();

  delay(2000);
}

void dht11_func() {

  hum = analogRead(PulseSensorPurplePin);
  temp = dht.readTemperature();
  Serial.print("Pulse: ");
  Serial.println(hum);
  Serial.print("Temperature: ");
  Serial.println(temp);

}
