#include <WiFi.h>

// Network credential configurations
const char* ssid = "YOUR_WIFI_SSID";         // Replace with your actual Wi-Fi SSID
const char* password = "YOUR_WIFI_PASSWORD"; // Replace with your actual Wi-Fi password

// Set web server port number to 80 (Standard HTTP port)
WiFiServer server(80);

String header;

// Hardware Pins Layout
const int ledPin = 2;       // On-board native blue LED for status control
const int pirPin = 22;      // PIR Sensor digital output pin

// Interrupt and Timing Variables
volatile bool motionDetected = false;
unsigned long lastTriggerTime = 0;
const unsigned long alertTimeout = 5000; // Active alert latch duration in milliseconds (5 seconds)

// Interrupt Service Routine (ISR) - Locked in internal RAM for speed and safety
void IRAM_ATTR motionISR() {
  motionDetected = true;
  lastTriggerTime = millis(); // Capture the absolute hardware timestamp immediately
}

void setup() {
  // Initialize serial telemetry baud rate
  Serial.begin(115200);
  
  // Configure hardware pins
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Default state: LED OFF
  
  pinMode(pirPin, INPUT);
  // Attach hardware interrupt to detect a transition from LOW to HIGH (Motion Start)
  attachInterrupt(digitalPinToInterrupt(pirPin), motionISR, RISING);

  // Initialize Station (STA) Mode and connect to the local access point
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Telemetry output for connection confirmation
  Serial.println("\n🎉 Wi-Fi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Start the network server registry listen loop
  server.begin();
}

void loop() {
  // ⚡ LOGIC BACKBONE: Non-blocking evaluation to clear the motion alert flag after timeout expires
  if (motionDetected && (millis() - lastTriggerTime > alertTimeout)) {
    motionDetected = false;
    Serial.println("🔄 Room Status Reset: Safe");
  }

  // Poll for incoming remote network client requests (browsers)
  WiFiClient client = server.available();
  
  if (client) {                             
    Serial.println("New Client Connected.");
    String currentLine = ""; // Buffer string to parse data line-by-line
    
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read(); // Read incoming data byte-by-byte
        Serial.write(c);        // Stream raw request data to local Serial Monitor
        header += c;
        
        if (c == '\n') { // Check for line boundaries
          // If the current line is completely empty, it marks the HTTP blank delimiter line
          // This signifies the end of the client request header. Send response now.
          if (currentLine.length() == 0) {
            
            // Step 1: Send standard HTTP response headers with explicit UTF-8 encoding
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html; charset=utf-8");
            client.println("Connection: close");
            client.println(); // Mandatory blank delimiter line for HTTP protocol
            
            // Step 2: Route parsing engine - Actuate GPIO based on URI parameters
            if (header.indexOf("GET /LED/on") >= 0) {
              digitalWrite(ledPin, HIGH);
            } else if (header.indexOf("GET /LED/off") >= 0) {
              digitalWrite(ledPin, LOW);
            }
            
            // Step 3: Dynamic HTML/CSS UI Document Generation
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #f44336;}</style></head>");
            
            client.println("<body><h1>Anti-Theft Security Dashboard</h1>");
            
            // 🎯 Dynamic State Rendering Layer
            client.println("<hr>");
            if (motionDetected) {
              client.println("<p style=\"font-size: 26px; color: #f44336; font-weight: bold;\">Room Status: 🚨 MOTION DETECTED!</p>");
            } else {
              client.println("<p style=\"font-size: 26px; color: #4CAF50; font-weight: bold;\">Room Status: ✅ Safe</p>");
            }
            client.println("<hr><br>");
            
            // Actuator Interfacing Layout Layer
            if (digitalRead(ledPin) == HIGH) {
              client.println("<p>Control Actuator Status: ON</p>");
              client.println("<p><a href=\"/LED/off\"><button class=\"button button2\">TURN OFF</button></a></p>");
            } else {
              client.println("<p>Control Actuator Status: OFF</p>");
              client.println("<p><a href=\"/LED/on\"><button class=\"button\">TURN ON</button></a></p>");
            }
            client.println("</body></html>");
            client.println(); // Terminate response structure
            break;            // Break client session execution block
          } else { 
            currentLine = ""; // Flush read line buffer
          }
        } else if (c != '\r') {  
          currentLine += c;   // Append character stream to line evaluation buffer
        }
      }
    }
    // Resource cleanup and session de-allocation
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("---------------------");
  }
}
