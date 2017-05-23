// Sonoff Controller
// Master Mode: Simple AP to advertise mains power availabilty.
// Slave Mode:  Simple client to switch power on when AP is available

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// WiFi Definitions //
const char WiFiSSID[] = "Sonoff-AP";  // Set the wifi SSID here
const char WiFiAPPSK[] = "password";  // Set the wifi passphrase here
const int WiFiCHAN = 1;         // Set the wifi Channel here
const int WiFiHIDE = 0;          // Set the wifi Hidden here

// Pin Definitions //
const int LED_PIN = 13;     // Sonoff green LED
const int RELAY_PIN = 12;   // Sonoff relay
const int BUTTON_PIN = 0;   // Sonoff button
const int LINK_PIN = 14;    // Sonoff link

int opmode;
int txcount;
int rxcount;
int rxoffcount;

int status = WL_IDLE_STATUS;     // the Wifi radio's status

void setup() 
{
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LINK_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, LOW);    // LED ON
  digitalWrite(RELAY_PIN, LOW);  //Relay OFF
  opmode = digitalRead(LINK_PIN);

  Serial.begin(115200);
  delay(1000);

  if (opmode == 0) //Master Mode
  {
    Serial.print("Mode: Master");
    Serial.println("");
  
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WiFiSSID, WiFiAPPSK, WiFiCHAN, WiFiHIDE);
    txcount=0;

    Serial.print("\nSonoff AP Ver 1.0 \n");
    Serial.print("SSID: ");
    Serial.print(WiFiSSID);
    Serial.print("   Channel: ");
    Serial.print(WiFiCHAN);
    Serial.print("   IP Addr: ");
    Serial.print(WiFi.softAPIP());
    Serial.println("");
  }
  else // Slave mode
  {
    // prepare GPIOs
    Serial.print("Mode: Slave");
    Serial.println("");
    
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);    // LED OFF
  
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);   //Relay OFF
 
    WiFi.mode(WIFI_STA);
    WiFi.begin(WiFiSSID, WiFiAPPSK, WiFiCHAN);
    rxcount = 0;
    rxoffcount=0;

    delay(5000);
    Serial.println("");
    Serial.println("Sonoff Power Client Ver 1.0 \n");
    Serial.println("Waiting for connection \n");
  }
}

void loop() 
{
  if (opmode == 0) //Master Mode
  {
    Serial.print("\nWiFi status: ");
    Serial.print(WiFi.status());
    Serial.print("   Connected stations: ");
    Serial.print(WiFi.softAPgetStationNum());
    Serial.println("");

    digitalWrite(RELAY_PIN, LOW); // Relay OFF
    digitalWrite(LED_PIN, HIGH);  // LED OFF
    delay (1800);
    digitalWrite(LED_PIN, LOW);   // LED ON
    delay(200);
    txcount = txcount + 1;
      if (txcount >= 450)             // Restart wifi after 15mins
      {
        txcount=0;
        WiFi.mode(WIFI_STA);
        WiFi.softAP(WiFiSSID, WiFiAPPSK, WiFiCHAN, WiFiHIDE);
        Serial.print("\n***** Restart master wifi\n");
        delay(300);
      }
  }
  else  //Slave mode
  {
    if (WiFi.status() != WL_CONNECTED) 
    {
      rxoffcount=rxoffcount + 1;
      if (rxoffcount >= 10)             // No signal for >= 20 secs. 
      {
        rxoffcount=10;                 // Keep in this state
        digitalWrite(RELAY_PIN, LOW);  // Relay OFF
        Serial.print("Power OFF\n");
        Serial.print("WiFi signal OFF\n");
        digitalWrite(LED_PIN, LOW);    // LED ON short pulse
        delay(50);
        digitalWrite(LED_PIN, HIGH);   // LED OFF
      }
      else
      {
      Serial.print("WiFi signal OFF\n"); // No signal for 0 - 10 secs.
      digitalWrite(LED_PIN, LOW);        // LED ON for medium pulse
      delay(250);
      digitalWrite(LED_PIN, HIGH);       // LED OFF
      }
      
      rxcount = rxcount + 1;
      if (rxcount >= 60)             // No connection for 120 secs - restart wifi
      {
        rxcount = 0;
        Serial.print("Restart slave wifi\n");
        WiFi.mode(WIFI_STA);
        WiFi.begin(WiFiSSID, WiFiAPPSK, WiFiCHAN);
        digitalWrite(LED_PIN, LOW);    // LED ON
        delay(500);
        digitalWrite(LED_PIN, HIGH);    // LED OFF
      }
      delay(1950);
    }
    if (WiFi.status() == WL_CONNECTED) 
    {
      rxoffcount=0;
      digitalWrite(RELAY_PIN, HIGH);  // Relay ON
      Serial.print("Power ON\n");
      digitalWrite(LED_PIN, HIGH);     // LED OFF
      delay(50);
      digitalWrite(LED_PIN, LOW);     // LED ON
      delay(1950);
    }
  }
}


