/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */
#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
#include <ArduinoJson.h>


StaticJsonBuffer<200> jsonBuffer;


SocketIOClient client;
const char* ssid     = "Bathroom Cam 1";
const char* password = "tomareomo69";

const char* host = "morning-lowlands-15038.herokuapp.com";
int port = 9707;
extern String RID;
extern String Rname;
extern String Rcontent;

unsigned long previousMillis = 0;
long interval = 5000;
unsigned long lastreply = 0;
unsigned long lastsend = 0;
String JSON;
JsonObject& root = jsonBuffer.createObject();

int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
#define BUTTON_PIN 5

void setup() {

  // set button pin as an input
  pinMode(BUTTON_PIN, INPUT);

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
  // read the pushbutton input pin:
  buttonState = digitalRead(BUTTON_PIN);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == LOW) {
      // if the current state is LOW then the button went from off to on:
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
      
      client.emit("sensor", "{\"message\":\"up\"}");
    } else {
      // if the current state is HIGH then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;
}
