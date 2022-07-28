//Basic - Libraries:
  #include <Arduino.h>
  #include <WiFi.h>
  #include <PubSubClient.h>

//Specific - Libraries:
  #include <Adafruit_NeoPixel.h>

//Basic - Declarations
  const char *SSID = "Fish Tank";
  const char *PWD = "jellyfish64";
  const char *HUTAO = "10.0.0.143";
  int mqttPort = 1616;
  WiFiClient wifiClient;
  PubSubClient mqttClient(wifiClient); 
  void connectToWiFi();
  void callback(char* topic, byte* payload, unsigned int length);
  void setupMQTT();
  void reconnect();

//Specific - Declarataions
  ////Misc
    #define PIN1 25
    #define PIN2 27
    #define UPPIXELS 58
    #define DOWNPIXELS 43
    #define DELAYVAL  50
    Adafruit_NeoPixel upline(UPPIXELS, PIN1, NEO_GRBW + NEO_KHZ800);
    Adafruit_NeoPixel downline(DOWNPIXELS, PIN2, NEO_GRBW + NEO_KHZ800);
    uint32_t firstPixelHue = 0;
    int light_status_top = 0;
    int light_status_bot = 0;
    int R_val = 0;
    int G_val = 0;
    int B_val = 0;
    int W_val = 0;
  ////Topics
    const char *TOPIC_LUX_BOT = "lux/bot";
    const char *TOPIC_LUX_TOP = "lux/top";
    const char *TOPIC_RGBW_R = "rgbw/r";
    const char *TOPIC_RGBW_G = "rgbw/g";
    const char *TOPIC_RGBW_B = "rgbw/b";
    const char *TOPIC_RGBW_W = "rgbw/w";
  ////Functions
    void neo_Rainbow_top();
    void neo_White_top();
    void neo_Slide_top();
    void neo_Off_top();
    void neo_Rainbow_bot();
    void neo_White_bot();
    void neo_Slide_bot();
    void neo_Off_bot();

////////////////////////

void setup() {
  //Basic - setup()
    Serial.begin(115200);
    connectToWiFi();
    setupMQTT();
  //Specific - setup()
    pinMode(27, OUTPUT);
    pinMode(25, OUTPUT);
    upline.begin();
    upline.show();
    downline.begin();
    downline.show();
}

void loop() {
  //Basic - loop()
    if (!mqttClient.connected())
      reconnect();
    mqttClient.loop();
  //Specific - loop()
  ////Upper Lights Switch
    switch(light_status_top)
    {
      case 0:
      neo_Off_top();
      break;

      case 1:
      neo_Rainbow_top();
      break;

      case 2:
      neo_White_top();
      break;

      case 3:
      neo_Slide_top();
      break;

      default:
      neo_Off_top();
      Serial.println(light_status_top);
    }
  ////Lower Lights Switch
    switch(light_status_bot)
    {
      case 0:
      neo_Off_bot();
      break;

      case 1:
      neo_Rainbow_bot();
      break;

      case 2:
      neo_White_bot();
      break;

      case 3:
      neo_Slide_bot();
      break;

      default:
      neo_Off_bot();
      Serial.println(light_status_bot);
    }

}

////////////////////////

void callback(char* topic, byte* payload, unsigned int length) {

  //Basic - Callback Payload Pre-Processing
  ////Converts Payload from byte* to String
    int callbackmode = 0;
    int num_payload = -1;
    String str_payload;
    char buffer_payload[length];
    char out_msg[40];
    for (int i = 0; i < length; i++) 
    {
      buffer_payload[i] = char(payload[i]);
    }
    str_payload = String(buffer_payload);
  //Specific - Callback Topic Switching
  ////Not sure how to turn this into a switch statement
    if(strcmp(topic, TOPIC_LUX_BOT) == 0)
    {
      callbackmode = 3;
    }
    if(strcmp(topic, TOPIC_LUX_TOP) == 0)
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
  //Specific - Callback Payload Processing
    switch(callbackmode)
    {
      case 3:
        num_payload = str_payload.toInt();
        light_status_bot = num_payload;
        break;
      case 4:
        num_payload = str_payload.toInt();
        light_status_top = num_payload;
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

//Basic - Connection Functions
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
//Important:
        //Specific - Topic Subscriptions
        mqttClient.subscribe("lux/bot");
        mqttClient.subscribe("lux/top");
        mqttClient.subscribe("rgbw/r");
        mqttClient.subscribe("rgbw/g");
        mqttClient.subscribe("rgbw/b");
        mqttClient.subscribe("rgbw/w");
      }
      
  }
}


//Specific - Function Declarations
////Upper Lights Functions
  void neo_Rainbow_top()
  {
    for(int i=0 ; i<UPPIXELS ; i++)
    {
      int pixelHue = firstPixelHue + (i*65536L/UPPIXELS);
      upline.setPixelColor(i, upline.gamma32(upline.ColorHSV(pixelHue,255,255)));   
      firstPixelHue += 1;
    }
    upline.show();
  }
  void neo_White_top()
  {
    for(int i=0 ; i<UPPIXELS ; i++)
    {
      upline.setPixelColor(i, upline.Color(0,0,0,255));
    }
    upline.show();
  }
  void neo_Slide_top()
  {
    for(int i=0 ; i<UPPIXELS ; i++)
    {
      upline.setPixelColor(i, upline.Color(R_val,G_val,B_val,W_val));
    }
    upline.show();
  }
  void neo_Off_top()
  {
    upline.clear();
    upline.show();
  }
////Lower Lights Functions
  void neo_Rainbow_bot()
  {
    for(int i=0 ; i<DOWNPIXELS ; i++)
    {
      int pixelHue = firstPixelHue + (i*65536L/DOWNPIXELS);
      downline.setPixelColor(i, downline.gamma32(downline.ColorHSV(pixelHue,255,255)));   
      firstPixelHue += 1;
    }
    downline.show();
  }
  void neo_White_bot()
  {
    for(int i=0 ; i<DOWNPIXELS ; i++)
    {
      downline.setPixelColor(i, downline.Color(0,0,0,255));
    }
    downline.show();
  }
  void neo_Slide_bot()
  {
    for(int i=0 ; i<DOWNPIXELS ; i++)
    {
      downline.setPixelColor(i, downline.Color(R_val,G_val,B_val,W_val));
    }
    downline.show();
  }
  void neo_Off_bot()
  {
    downline.clear();
    downline.show();
  }