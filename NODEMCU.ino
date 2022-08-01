//ThatsEngineering
//Sending Data from Arduino to NodeMCU Via Serial Communication
//NodeMCU code

#include <ESP8266WiFi.h>
 
String apiKey = "YJUOGEHZ3VR0TJ8K";     //  Enter your Write API key from ThingSpeak
 
const char *ssid =  "Krishna";     // replace with your wifi ssid and wpa2 key
const char *pass =  "098760987";
const char* server = "api.thingspeak.com";
WiFiClient client;

#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);


void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  nodemcu.begin(9600);

  WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
  while (!Serial) continue;
}

void loop() {
  
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu);

  if (data == JsonObject::invalid()) {
    //Serial.println("Invalid Json Object");
    jsonBuffer.clear();
    return;
  }

  Serial.println("JSON Object Recieved");
  Serial.print("Recieved Pulse:  ");
  float hum = data["humidity"];
  Serial.println(hum);
  Serial.print("Recieved Temperature:  ");
  float temp = data["temperature"];
  Serial.println(temp);
  Serial.println("-----------------------------------------");







  if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(temp);
                             postStr +="&field2=";
                             postStr += String(hum);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(temp);
                             Serial.print(" degrees Celcius, BPM  ");
                             Serial.print(hum);
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
}
