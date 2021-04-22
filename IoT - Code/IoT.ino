/*
  PubNub over WiFi Example using WiFi Shield 101

  This sample client will publish raw (string) PubNub messages from
  boards like Arduino Uno and Zero with WiFi Shield 101

  See https://www.arduino.cc/en/Reference/WiFi101 for more info

*/
#include <Adafruit_SSD1327.h>

// Used for software SPI
#define OLED_CLK 13
#define OLED_MOSI 11

// Used for software or hardware SPI
#define OLED_CS 10
#define OLED_DC 8

// Used for I2C or SPI
#define OLED_RESET -1

// I2C
Adafruit_SSD1327 display(128, 128, &Wire, OLED_RESET, 1000000);

#include <SPI.h>

#include <WiFiNINA.h>
#define PubNub_BASE_CLIENT WiFiClient
#include <PubNub.h>
#include <ArduinoJson.h>

using namespace ArduinoJson;

static char ssid[] = "id";  // your network SSID (name)
static char pass[] = "password"; // your network password
int         status = WL_IDLE_STATUS;       // the Wifi radio's status

const static char pubkey[]  = "publish key";        // your publish key
const static char subkey[]  = "subscribe key";        // your subscribe key
const static char channel[] = "channel name"; // channel to use
const static String channelName = "channel name"; // channel to use
boolean     wasRead = true;
String      message = "";
int         lightValue;
int         light_treshold = 100;
int         motorPin = 3;
int         lightSensorPin = 0;


void setup()
{
    pinMode(motorPin, OUTPUT);

    setupSerial();
    setupWifi();
    setupPubNub();
    setupDisplay();
    delay(1000);
    drawMessage();
}

void loop()
{
  lightValue = analogRead(lightSensorPin);
  if(lightValue < light_treshold) {
    Serial.println("\n\nClearing Display");
    display.clearDisplay();
    display.display();
    Serial.println("Display Cleared\n\n");

  }

  if(wasRead){
      Serial.println("Getting Message");
      getMessage();
      Serial.println("Message retrieved\n\n");
    }


  while(!wasRead){
    Serial.println("Waiting to display the message");
    spinServo();    // Drehe Herz
    lightValue = analogRead(lightSensorPin);      // Lese Helligkeitswert
    Serial.print("Light value sensed: ");
  Serial.println(lightValue);

    if(lightValue > light_treshold) {
      Serial.println("Displaying Message");
      drawMessage();
      wasRead = true;
      messageRead();

      while(lightValue > light_treshold){lightValue = analogRead(lightSensorPin);}
    }

    Serial.println("Message Displayed\n\n");
  }

  Serial.print("Light value sensed: ");
  Serial.println(lightValue);
}




/*#########################################################################################################*/
/*############ Spin Heart #################################################################################*/
/*#########################################################################################################*/

void spinServo(){
    analogWrite(motorPin, 255);
    delay(500);    // Warte 50ms um den Servo zu drehen
    analogWrite(motorPin, 0);
    delay(800);
}

/*#########################################################################################################*/
/*############ PubNub #################################################################################*/
/*#########################################################################################################*/

void getMessage() {
    PubSubClient *sclient = PubNub.subscribe(channel);
    if (!sclient) return; // error
    String msg;
    SubscribeCracker ritz(sclient);
    DynamicJsonDocument doc(1024);
    while (!ritz.finished()) {
        ritz.get(msg);
        if (msg.length() > 0) {
            message = msg;

            deserializeJson(doc, msg);

            String device = doc["Device"];
            String pubnubMessage = doc["Message"];

            if(device != channelName){
              Serial.print("Received: ");
              Serial.println(pubnubMessage);
              message = pubnubMessage;
              wasRead = false;
            }
        }
    }
    sclient->stop();

    delay(500);
}

void messageRead() {
    char msg[]  = "{ \"Device\": \"" + channelName + "\", \"Message\": \"Received\"}";
    auto client = PubNub.publish(channel, msg);
    if (!client) {
        Serial.println("publishing error");
        delay(1000);
        return;
    }
    if (PubNub.get_last_http_status_code_class() != PubNub::http_scc_success) {
        Serial.print("Got HTTP status code error from PubNub, class: ");
        Serial.print(PubNub.get_last_http_status_code_class(), DEC);
    }

    client->stop();
    Serial.println("Response Sent");
}

/*#########################################################################################################*/
/*############ Spin Heart #################################################################################*/
/*#########################################################################################################*/


void drawMessage() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(30,30);
    display.println(message);
    display.display();
}
















































/*#########################################################################################################*/
/*############ Set Up #####################################################################################*/
/*#########################################################################################################*/

void setupWifi(){
  // attempt to connect using WPA2 encryption:
  Serial.println("Attempting to connect to WPA network...");
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 5 seconds for connection:
    delay(5000);
  }

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
  printCurrentNet();
  printWiFiData();
  Serial.print("WiFi connecting to SSID: ");
  Serial.println(ssid);
}

void setupPubNub(){
  PubNub.begin(pubkey, subkey);
  Serial.println("PubNub set up");
}

void setupSerial(){
  // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("Serial set up");
}

void setupDisplay(){
   if ( ! display.begin(0x3C) ) {
     Serial.println("Unable to initialize OLED");
     while (1) yield();
  }
  display.clearDisplay();
  display.display();
}


/*#########################################################################################################*/
/*############ WiFi Helper Functions ######################################################################*/
/*#########################################################################################################*/

void printWiFiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP address : ");
  Serial.println(ip);

  Serial.print("Subnet mask: ");
  Serial.println((IPAddress)WiFi.subnetMask());

  Serial.print("Gateway IP : ");
  Serial.println((IPAddress)WiFi.gatewayIP());

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI): ");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type: ");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
