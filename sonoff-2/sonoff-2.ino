// Sonoff Controller
// Master Mode: Simple AP to advertise mains power availabilty.
// Slave Mode:  Simple client to switch power on when AP is available

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// WiFi Definitions //
const char WiFiAPPSK[] = "password";  // Set the wifi passphrase here
const char WiFiSSID[] = "Sonoff-AP";  // Set the wifi SSID here
const int WiFiCHAN = 13;         // Set the wifi Channel here
const int WiFiHIDE = 1;          // Set the wifi Hidden here

//const char* ssid = "Sonoff-AP";
//const char* password = "password";


// Pin Definitions //
const int LED_PIN = 13;     // Sonoff green LED
const int RELAY_PIN = 12;   // Sonoff relay
const int BUTTON_PIN = 0;   // Sonoff button
const int LINK_PIN = 14;   // Sonoff link

int opmode;

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
 
    WiFi.begin(WiFiSSID, WiFiAPPSK, WiFiCHAN);

    delay(2000);
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
  }
  else  //Slave mode
  {
    if (WiFi.status() != WL_CONNECTED) 
    {
      digitalWrite(RELAY_PIN, LOW);   // Relay OFF
      Serial.print("Power OFF\n");
      digitalWrite(LED_PIN, LOW);    // LED ON
      delay(50);
      digitalWrite(LED_PIN, HIGH);    // LED OFF
      delay(1950);
    }
    if (WiFi.status() == WL_CONNECTED) 
    {
      digitalWrite(RELAY_PIN, HIGH);  // Relay ON
      Serial.print("Power ON\n");
      digitalWrite(LED_PIN, HIGH);     // LED OFF
      delay(50);
      digitalWrite(LED_PIN, LOW);     // LED ON
      delay(1950);
    }
  }
}


