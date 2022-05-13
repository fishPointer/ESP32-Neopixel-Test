#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

#define PIN1        27
#define UPPIXELS 58
#define DELAYVAL  50
Adafruit_NeoPixel upline(UPPIXELS, PIN1, NEO_GRBW + NEO_KHZ800);

int light_status = 0;

uint32_t firstPixelHue = 0;

const char *SSID = "Breath of the Wifi";
const char *PWD = "supervolcano55";
const char *HUTAO = "192.168.0.193";
const char *TOPIC_CORN = "corn";
const char *TOPIC_BOOLY = "booly";
const char *TOPIC_LUX = "lux";
int mqttPort = 1616;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 

void connectToWiFi();
void callback(char* topic, byte* payload, unsigned int length);
void setupMQTT();
void reconnect();
void neo_Rainbow();
void neo_White();

////////////////////////

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  setupMQTT();
  pinMode(27, OUTPUT);
  upline.begin();
  upline.show();
}

void loop() {

  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();

  if(light_status == 1)
  {
    neo_Rainbow();
  }
  if(light_status == 2)
  {
    neo_White();
  }


  else
  {
      upline.clear();
      upline.show();
  }


}

////////////////////////

void callback(char* topic, byte* payload, unsigned int length) {

//Initialize some Variables for switching and payload conversion
int callbackmode = 0;
int num_payload = -1;
String str_payload;
char buffer_payload[length];
char out_msg[40];
//Saving Payload into a buffer, and String
for (int i = 0; i < length; i++) 
{
  buffer_payload[i] = char(payload[i]);
}
str_payload = String(buffer_payload);

if(strcmp(topic, TOPIC_LUX) == 0)
{
  callbackmode = 4;
}

//Primary switch statement for different functions by topic
//Master list
  switch(callbackmode)
  {
    case 4:
      num_payload = str_payload.toInt();
      light_status = num_payload;
      break;

    //Default Functionality, read off message and topic
    default:
      Serial.print("Callback - ");
      Serial.println(topic);
      Serial.print("Message:");

      for (int i = 0; i < length; i++) 
      {
        Serial.print(char(payload[i]));
      }
      Serial.println();
  }
  
}

////////////////////////

void connectToWiFi() {
  Serial.print("Connecting to ");
 
  WiFi.begin(SSID, PWD);
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected.");
}

void setupMQTT() {
  mqttClient.setServer(HUTAO, mqttPort);
  mqttClient.setCallback(callback);
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "Crumble-";
      clientId += String(random(0xff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        mqttClient.subscribe("corn");
        mqttClient.subscribe("potato");
        mqttClient.subscribe("booly");
        mqttClient.subscribe("lux");
      }
      
  }
}

/////////////////////////////////////////
// Neopixel functions //

void neo_Rainbow()
{
  for(int i=0 ; i<UPPIXELS ; i++)
  {
    int pixelHue = firstPixelHue + (i*65536L/UPPIXELS);
    upline.setPixelColor(i, upline.gamma32(upline.ColorHSV(pixelHue,255,255)));   
    firstPixelHue += 1;
  }
  upline.show();
}

void neo_White()
{
  for(int i=0 ; i<UPPIXELS ; i++)
  {
    upline.setPixelColor(i, upline.Color(0,0,0,255));
  }
  upline.show();
}