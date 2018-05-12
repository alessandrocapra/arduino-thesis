/*
    This sketch sends data via HTTP POST requests to the server.
*/

#include <ESP8266WiFi.h>
#include <SocketIOClient.h>

const char* ssid     = "Bathroom Cam 1";
const char* password = "tomareomo69";

String host     = "192.168.2.132";                 // change with correct host on Heroku, this is for dev only
const int port  = 5001;
SocketIOClient client;

// implementation tryouts variables
long randNumber;

void setup() {
  Serial.begin(115200);
  delay(10);
  
  wifiConnection();

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
}

void loop() {
//  socket.monitor();
  sendDataToServer("bella");
}

void wifiConnection() {
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

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  
  if (client.connected())
  {
    client.emit("sensor", "{\"message\":\"bella\"}");
  }
}

void sendDataToServer(String key) {
  delay(1500);                                        // to tune and decide how to send data coming from pressure sensor

  randNumber = random(300);
  
  if(randNumber < 100){
    client.emit("sensor", "{\"message\":\"left\"}");
  } else if(randNumber >= 100 && randNumber < 200){
    client.emit("sensor", "{\"message\":\"right\"}");
  } else {
    client.emit("sensor", "{\"message\":\"up\"}");
  }
}
