#include <WiFi.h>

// Replace with your network credentials
const char* ssid     = "Project-Beer2";
const char* password = "123456789";

// // Set web server port number to 80
// WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  Serial.print(WiFi.begin(ssid, password) ? "Ready" : "Failed!");
  Serial.println("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  
  // server.begin();
}

void loop() {
}