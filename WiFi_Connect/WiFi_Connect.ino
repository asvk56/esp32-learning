#include <WiFi.h>

// Network credential configurations
const char* ssid = "YOUR_WIFI_NAME";     // Name of your WiFi Hotspot
const char* password = "YOUR_PASSWORD"; // Password of WiFi Hotspot

void setup() {
  // Initialize serial telemetry
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n--------------------------------------");
  Serial.println("Network Layer Initialization...");
  Serial.println("--------------------------------------");

  // Step 1: Explicitly set ESP32 to Station Mode (Client)
  WiFi.mode(WIFI_STA);
  
  // Step 2: Trigger connection protocol
  WiFi.begin(ssid, password);
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  // Step 3: Blocking loop until connection authorization is successful
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); // Will keep printing dots until it gets connected
  }

  // Step 4: Connection Success Telemetry
  Serial.println("\n\n🎉 Wi-Fi Connected Successfully!");
  Serial.print("ESP32 Assigned Local IP Address: ");
  Serial.println(WiFi.localIP()); // IP which will be used further to open web pages
  Serial.println("--------------------------------------");
}

void loop() {
  // Static execution loop - network handle active in the background
}
