#include <WiFi.h>

// Network credential configurations
const char* ssid = "YOUR_WIFI_NAME";     // Name of your WiFi Hotspot
const char* password = "YOUR_PASSWORD"; // Password of WiFi Hotspot

// Set web server port number to 80 (Standard HTTP port)
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Assign output variable to the on-board LED pin
const int ledPin = 2;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Start with LED turned off

  // Connect to Wi-Fi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\n🎉 Wi-Fi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Start the HTTP web server registry
  server.begin();
}

void loop() {
  // Listen for incoming network clients (like your mobile browser)
  WiFiClient client = server.available();
  
  if (client) {                             
    Serial.println("New Client Connected.");
    String currentLine = "";                // Make a String to hold incoming data from client
    
    while (client.connected()) {            // Loop while the client's connection stays active
      if (client.available()) {             // If there's data to read from the client
        char c = client.read();             // Read a byte
        Serial.write(c);                    // Print it out the serial monitor
        header += c;
        if (c == '\n') {                    // If the byte is a newline character
          // If the current line is blank, you got two newline characters in a row.
          // That's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Route parsing logic: Turns the GPIOs ON and OFF
            if (header.indexOf("GET /LED/on") >= 0) {
              Serial.println("GPIO 2 is ON");
              digitalWrite(ledPin, HIGH);
            } else if (header.indexOf("GET /LED/off") >= 0) {
              Serial.println("GPIO 2 is OFF");
              digitalWrite(ledPin, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #f44336;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Wireless Control</h1>");
            
            // Display current state, and ON/OFF buttons 
            if (digitalWrite(ledPin) == HIGH) {
              client.println("<p>LED State: ON</p>");
              client.println("<p><a href=\"/LED/off\"><button class=\"button button2\">TURN OFF</button></a></p>");
            } else {
              client.println("<p>LED State: OFF</p>");
              client.println("<p><a href=\"/LED/on\"><button class=\"button\">TURN ON</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("---------------------");
  }
}
