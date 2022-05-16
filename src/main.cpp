#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

#define PIN1        25
#define UPPIXELS 58
#define DELAYVAL  50
Adafruit_NeoPixel upline(UPPIXELS, PIN1, NEO_GRBW + NEO_KHZ800);

int light_status = 0;
int R_val = 0;
int G_val = 0;
int B_val = 0;
int W_val = 0;

uint32_t firstPixelHue = 0;

const char *SSID = "Breath of the Wifi";
const char *PWD = "supervolcano55";
const char *HUTAO = "192.168.0.193";
const char *TOPIC_CORN = "corn";
const char *TOPIC_BOOLY = "booly";
const char *TOPIC_LUX = "lux";
const char *TOPIC_RGBW_R = "rgbw/r";
const char *TOPIC_RGBW_G = "rgbw/g";
const char *TOPIC_RGBW_B = "rgbw/b";
const char *TOPIC_RGBW_W = "rgbw/w";
int mqttPort = 1616;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 

void connectToWiFi();
void callback(char* topic, byte* payload, unsigned int length);
void setupMQTT();
void reconnect();
void neo_Rainbow();
void neo_White();
void neo_Slide();

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
  if(light_status == 3)
  {
    neo_Slide();
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

if(strcmp(topic, TOPIC_RGBW_R) == 0)
{
  callbackmode = 5;
}
if(strcmp(topic, TOPIC_RGBW_G) == 0)
{
  callbackmode = 6;
}
if(strcmp(topic, TOPIC_RGBW_B) == 0)
{
  callbackmode = 7;
}
if(strcmp(topic, TOPIC_RGBW_W) == 0)
{
  callbackmode = 8;
}

//Primary switch statement for different functions by topic
//Master list
  switch(callbackmode)
  {
    case 4:
      num_payload = str_payload.toInt();
      light_status = num_payload;
      break;

    case 5:
      num_payload = str_payload.toInt();
      R_val = num_payload;
      break;
    case 6:
      num_payload = str_payload.toInt();
      G_val = num_payload;
      break;
    case 7:
      num_payload = str_payload.toInt();
      B_val = num_payload;
      break;
    case 8:
      num_payload = str_payload.toInt();
      W_val = num_payload;
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
        mqttClient.subscribe("rgbw/r");
        mqttClient.subscribe("rgbw/g");
        mqttClient.subscribe("rgbw/b");
        mqttClient.subscribe("rgbw/w");
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

void neo_Slide()
{
  for(int i=0 ; i<UPPIXELS ; i++)
  {
    upline.setPixelColor(i, upline.Color(R_val,G_val,B_val,W_val));
  }
  upline.show();
}