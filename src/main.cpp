#include <WiFi.h>

const char* ssid = "ESP32_Host";  // Must match the Host's SSID
const char* password = "12345678";  // Must match the Host's password
const char* hostIP = "192.168.4.1";  // Default SoftAP IP of the Host
const int port = 8080;  // Port number for TCP communication
const int relayPin = 12;  // GPIO pin for the relay
const float threshold = 2.0;  // Voltage threshold to activate the relay

WiFiClient client;

void setup() {
    Serial.begin(115200);
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);  // Ensure relay is OFF at start

    // Connect to the Host's WiFi AP
    Serial.println("Connecting to Host ESP32...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to Host!");

    // Connect to TCP Server
    if (client.connect(hostIP, port)) {
        Serial.println("Connected to Host Server.");
    } else {
        Serial.println("Connection to Host Server failed.");
    }
}

void loop() {
    if (client.connected() && client.available()) {
        String sensorData = client.readStringUntil('\n');  // Read until newline
        Serial.print("Received pressure data: ");
        Serial.println(sensorData);
        client.write("HELLO FROM THE OTHER SIDE!\n");

        // Convert received data to float
        float pressureValue = sensorData.toFloat();

        // Check if the pressure value exceeds the threshold
        if (pressureValue >= threshold) {
            Serial.println("Threshold exceeded! Turning relay ON.");
            digitalWrite(relayPin, HIGH);  // Activate relay
        } else {
            Serial.println("Below threshold. Relay OFF.");
            digitalWrite(relayPin, LOW);  // Deactivate relay
        }
    } else if (!client.connected()) {
        Serial.println("Lost connection. Reconnecting...");
        client.stop();
        delay(2000);
        client.connect(hostIP, port);  // Attempt to reconnect
    }
}