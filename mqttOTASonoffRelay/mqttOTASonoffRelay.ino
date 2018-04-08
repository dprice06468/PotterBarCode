#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

//needed for library
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

// select which pin will trigger the configuration portal when set to LOW
// ESP-01 users please note: the only pins available (0 and 2), are shared 
// with the bootloader, so always set them HIGH at power-up
#define TRIGGER_PIN 0

/* WiFi Settings */
const char* ssid     = "pricelan";
const char* password = "candyman";

/* Sonoff Outputs */
//const int relayPin = 12;  // Active high
//const int ledPin   = 13;  // Active low
#define RELAY_PIN 12  // Active high
#define LED_PIN   13  // Active low

/* MQTT Settings */
#define MQTT_SONOFF1_RELAY1 "/relay1/#"
#define MQTT_START          "/relay1/start"
#define MQTT_STOP           "/relay1/stop"
#define MQTT_ON             "/relay1/on"
#define MQTT_OFF            "/relay1/off"
#define MQTT_PULSE          "/relay1/pulse"
IPAddress broker(192,168,1,31);             // Address of the MQTT broker
#define CLIENT_ID "sonoff_relay1"           // Client ID to send to the broker

boolean running = false;

//setup MQTT client
WiFiClient wificlient;
PubSubClient mqttClient(wificlient);

//--------------------------------------------------------------
// MQTT callback to process messages
//--------------------------------------------------------------
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((strcmp(topic, MQTT_START)==0) && !running) {
    running = true;
    Serial.println("started");
  }  else if ((strcmp(topic, MQTT_STOP)==0) && running) {
    running = false;
    digitalWrite(LED_PIN, HIGH);     // LED is active-low, so this turns it off
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("stopped");
  } else if ((strcmp(topic, MQTT_ON)==0) && running) {
    digitalWrite(LED_PIN, LOW);      // LED is active-low, so this turns it on
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("on");
  } else if ((strcmp(topic, MQTT_OFF)==0) && running) {
    digitalWrite(LED_PIN, HIGH);     // LED is active-low, so this turns it off
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("off");
  } else if ((strcmp(topic, MQTT_PULSE)==0) && running) {
    digitalWrite(LED_PIN, LOW);      // LED is active-low, so this turns it on
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("on");
    delay(5000);
    digitalWrite(LED_PIN, HIGH);     // LED is active-low, so this turns it off
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("off");
  } else {
    Serial.println("Unknown topic");
  }
}

//--------------------------------------------------------------
// Attempt connection to MQTT broker and subscribe to command topic
//--------------------------------------------------------------
void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(CLIENT_ID)) {
      Serial.println("connected");
      mqttClient.subscribe(MQTT_SONOFF1_RELAY1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//--------------------------------------------------------------
// Setup
//--------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
//  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, password);
//  Serial.println("WiFi begun");
//  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
//    Serial.println("Connection Failed! Rebooting...");
//    delay(5000);
//    ESP.restart();
//  }
//  Serial.println("Proceeding");

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");
  
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if      (error == OTA_AUTH_ERROR   ) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR  ) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR    ) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
//  Serial.println("Ready");
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());
  
  /* Set up the outputs. LED is active-low */
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(RELAY_PIN, LOW);

  /* Prepare MQTT client */
  mqttClient.setServer(broker, 1883);
  mqttClient.setCallback(callback);
}

//--------------------------------------------------------------
// Main Loop
//--------------------------------------------------------------
void loop() {
  // is configuration portal requested?
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    Serial.println("wifimanagaer triggered");
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    //reset settings - for testing
    //wifiManager.resetSettings();

    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep
    //in seconds
    //wifiManager.setTimeout(120);

    //it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration

    //WITHOUT THIS THE AP DOES NOT SEEM TO WORK PROPERLY WITH SDK 1.5 , update to at least 1.5.1
    //WiFi.mode(WIFI_STA);
    
    if (!wifiManager.startConfigPortal("OnDemandAP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
    }

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }

  ArduinoOTA.handle();
  
//  if (WiFi.status() != WL_CONNECTED)
//  {
//    Serial.print("Connecting to ");
//    Serial.print(ssid);
//    Serial.println("...");
//    WiFi.begin(ssid, password);

//    if (WiFi.waitForConnectResult() != WL_CONNECTED)
//      return;
//    Serial.println("WiFi connected");
//  }

//  if (WiFi.status() == WL_CONNECTED) {
    if (!mqttClient.connected()) {
      reconnect();
    }
//  }
  
  if (mqttClient.connected())
  {
    mqttClient.loop();
  }
}
