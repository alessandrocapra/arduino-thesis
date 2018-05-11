/*
 *  This sketch sends data via HTTP POST requests to the server.
 *
 * 
 *
 */

#include <ESP8266WiFi.h>

const char* ssid     = "Bathroom Cam 1";
const char* password = "tomareomo69";
const char* host = "192.168.2.132";                 // change with correct host on Heroku, this is for dev only
//const char* streamId   = "....................";
//const char* privateKey = "....................";

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 5000;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/api/sensor";
//  url += streamId;
//  url += "?private_key=";
//  url += privateKey;
//  url += "&value=";
//  url += value;

  String data = "{\"title\":\"OSTIAVACA\"}";
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 //"Connection: close\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + data.length() + "\r\n" +
                 "\r\n" + 
                 data + "\n");

                 
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}

