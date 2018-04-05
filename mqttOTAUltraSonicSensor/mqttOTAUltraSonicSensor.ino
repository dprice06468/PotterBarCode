//OTA includes
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
//MQTTincludes
#include <PubSubClient.h>

//set distance sensor pins
#define TRIG_PIN 5 
#define ECHO_PIN 4
#define LED_PIN  2

/* WiFi Settings */
#define ssid "pricelan"
#define password "candyman"

#define RANGE 50
int dist;

/* MQTT Settings */
const char* mqttTopic = "proximity1/inrange";   // MQTT topic
IPAddress broker(192,168,1,31);             // Address of the MQTT broker
#define CLIENT_ID "esp8266-proximity1"     // Client ID to send to the broker

//-------------------------------------------------------------------
// MQTT callback to process messages
//-------------------------------------------------------------------
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

/*  // Examine only the first character of the message
  if(payload[0] == 49)              // Message "1" in ASCII (turn outputs ON)
  {
    digitalWrite(ledPin, LOW);      // LED is active-low, so this turns it on
    digitalWrite(relayPin, HIGH);
  } else if(payload[0] == 48)       // Message "0" in ASCII (turn outputs OFF)
  {
    digitalWrite(ledPin, HIGH);     // LED is active-low, so this turns it off
    digitalWrite(relayPin, LOW);
  } else {
    Serial.println("Unknown value");
  }
*/  
}

WiFiClient wificlient;
PubSubClient client(wificlient);

//-------------------------------------------------------------------
// Attempt connection to MQTT broker and subscribe to command topic
//-------------------------------------------------------------------
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(CLIENT_ID)) {
      Serial.println("connected");
//      client.subscribe(mqttTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//-------------------------------------------------------------------
//numvar getdist(void) 
//-------------------------------------------------------------------
int getdist(void) {
  long duration;
  int distance;

  //init trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  //issue echo signal
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  //capture echo and calculate duration
  duration = pulseIn(ECHO_PIN, HIGH);
  //Serial.println(duration);

  //calculate distance in cm (for inches divide by 148)
  distance = duration/58;  
  //distance = duration*0.034/2;

  return distance;
}

//-------------------------------------------------------------------
void getDistanceAndSend() {
  //determine whether someone is in range 
  dist = getdist();
  if (dist < RANGE) {
    //publish MQTT "inrange" topic
    String s = String(dist);
    char carr[3];
    s.toCharArray(carr,3);
    client.publish(mqttTopic, carr);

    Serial.print("Distance: "); Serial.println(dist);
    //analogWrite(LED_PIN, dist);
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
  }
}

//-------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("in basicUltraSonicSensor.setup");
  
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Wifi connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("wifi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //pin setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(TRIG_PIN, LOW);

  /* Prepare MQTT client */
  client.setServer(broker, 1883);
  client.setCallback(callback);
}

//-------------------------------------------------------------------
void loop() {
//  Serial.println("in basicUltraSonicSensor.loop");
  ArduinoOTA.handle();

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, password);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      reconnect();
    }
  }

  getDistanceAndSend();

  if (client.connected())
    client.loop();
}




