/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

*/
#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
#include <ArduinoJson.h>


StaticJsonBuffer<200> jsonBuffer;


SocketIOClient client;
//const char* ssid     = "Bathroom Cam 1";
//const char* password = "tomareomo69";

const char* ssid     = "eduroam bridge";
const char* password = "vaenmona";

const char* host = "duchennegame.herokuapp.com";
int port = 9707;

// variables to read pressure sensor
int sensorPin = A0;
int sensorValue = 0;
float Vout = 0;
float P = 0;

// counter var for keeping socketIO connection alive
unsigned long startTime;

unsigned long previousMillis = 0;
long interval = 5000;
unsigned long lastreply = 0;
unsigned long lastsend = 0;

void setup() {

  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (!client.connect(host)) Serial.println(F("Not connected."));

  if (client.connected())
  {
    client.emit("sensor", "{\"message\":\"Arduino connected!\"}");
  }

}

void loop() {
  // refresh socketIO connection every 30 secs
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis;
    client.heartbeat(0);

    lastsend = millis();
  }

  //  if (millis() - startTime < 3000) {
  //    client.emit("hello", "{\"message\":\"Ciao server!\"}");
  //  }

  if (client.monitor()) {
    // do something
  }

  //  if (!client.connected()) {
  //    client.reconnect(host);
  //    client.emit("sensor", "{\"message\":\"Arduino re-connected!\"}");
  //  }

  // Reading the pressure sensor values

  //// calculate the offset
  int i = 0;
  int sum = 0;
  int offset = 0;
  Serial.println("init...");
  for (i = 0; i < 10; i++)
  {
    sensorValue = analogRead(sensorPin) - 512;
    sum += sensorValue;
  }

  // take the average value
  offset = sum / 10.0;
  Serial.println("Ok");

  while (1)
  {
    sensorValue = analogRead(sensorPin) - offset;
    Vout = (5 * sensorValue) / 1024.0;
    P = Vout - 2.5;
    Serial.print("Pressure = " );
    Serial.print(P * 1000);
    Serial.println("Pa");

    if (P * 1000 > 800.0) {
      String messageContents = "{\"message\":\"up\"}";
      client.emit("sensor", messageContents);
    } else if (P * 1000 > 200) {
      String messageContents = "{\"message\":\"right\"}";
      client.emit("sensor", messageContents);
    } else if (P * 1000 < -200.0) {
      String messageContents = "{\"message\":\"left\"}";
      client.emit("sensor", messageContents);
    } else {
      String messageContents = "{\"message\":\"turn\"}";
      client.emit("sensor", messageContents);
    }

    String messageContents = "{\"pressure\":\"";
    messageContents += P * 1000;
    messageContents += "\"}";
    client.emit("pressure", messageContents);

    delay(10);

  }
}
