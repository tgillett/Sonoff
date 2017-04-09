// Sonoff AP
// Simple AP to advertise mains power availabilty.

#include <ESP8266WiFi.h>

// WiFi Definitions //
const char WiFiAPPSK[] = "password";  // Set the wifi passphrase here
const char WiFiSSID[] = "Sonoff-AP";  // Set the wifi SSID here
const int WiFiCHAN = 13;         // Set the wifi Channel here
const int WiFiHIDE = 1;          // Set the wifi Hidden here

// Pin Definitions //
const int LED_PIN = 13;     // Sonoff green LED
const int RELAY_PIN = 12;   // Sonoff relay

void setup() 
{
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);    // LED ON
  digitalWrite(RELAY_PIN, LOW);  //Relay OFF

  Serial.begin(115200);
  delay(1000);
  
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

void loop() 
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


