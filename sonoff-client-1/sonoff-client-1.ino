
// Sonoff Client Ver 1.0

// Simple client to detect Sonoff AP to switch power on

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define CHANNEL 13

// Replace with your network credentials
const char* ssid = "Sonoff-AP";
const char* password = "password";

int LED_PIN = 13;
int RELAY_PIN = 12;

void setup(void){

  // preparing GPIOs
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);    // LED OFF
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);   //Relay OFF
 
  Serial.begin(115200); 
  WiFi.begin(ssid, password, CHANNEL);

  delay(2000);
  Serial.println("");
  Serial.println("Sonoff Power Client Ver 1.0 \n");
  Serial.println("Waiting for connection \n");
  
}
 
void loop(void){
  
//  server.handleClient();

    if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(RELAY_PIN, LOW);   // Relay OFF
    Serial.print("Power OFF\n");
    digitalWrite(LED_PIN, LOW);    // LED ON
    delay(50);
    digitalWrite(LED_PIN, HIGH);    // LED OFF
    delay(2000);
  }
    if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(RELAY_PIN, HIGH);  // Relay ON
    Serial.print("Power ON\n");
    digitalWrite(LED_PIN, HIGH);     // LED OFF
    delay(50);
    digitalWrite(LED_PIN, LOW);     // LED ON
    delay(2000);
  }

} 


