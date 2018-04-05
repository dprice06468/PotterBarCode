//OTA includes
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
//MQTTincludes
#include <PubSubClient.h>
//Servo includesS
#include <Servo.h>

//set Candy servo pin
#define CANDY_SERVO_PIN 12
#define CANDY_TOP 50
#define CANDY_BOTTOM 120

/* WiFi Settings */
#define ssid "pricelan"
#define password "candyman"

Servo candyServo;

/* MQTT Settings */
const char* mqttTopic = "infrared1/receive";   // MQTT topic
IPAddress broker(192,168,1,31);             // Address of the MQTT broker
#define CLIENT_ID "infrared1"     // Client ID to send to the broker

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

//--------------------------------------------------------------
//numvar pushup(void) {
//--------------------------------------------------------------
void pushup(void) {
  Serial.println("mqttOTALinearServo.pushup");
  for (byte pos=CANDY_BOTTOM; pos>=CANDY_TOP; pos--)     // goes from 180 degrees to 0 degrees 
  {                                
    candyServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
}

//--------------------------------------------------------------
//numvar dropdown(void) {
//--------------------------------------------------------------
void dropdown(void) {
  Serial.println("mqttOTALinearServo.dropdown");
  candyServo.write(CANDY_BOTTOM);              // tell servo to go to position in variable 'pos' 
}

//--------------------------------------------------------------
// this function will be implemented in bitlash as the IR function
// for the candy store window.
//--------------------------------------------------------------
void upanddown() {
  Serial.println("mqttOTALinearServo.upanddown");
  pushup();
  delay(1000);
  dropdown();  
}

//--------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("mqttOTALinearServo.setup");
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

  candyServo.attach(CANDY_SERVO_PIN);
  candyServo.write(CANDY_BOTTOM);
  
  /* Prepare MQTT client */
  client.setServer(broker, 1883);
  client.setCallback(callback);
}

//--------------------------------------------------------------
void loop() {
  //Serial.println("basicLinearServo.loop");

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

  upanddown();
  delay(1000);
  
  if (client.connected())
    client.loop();
}
