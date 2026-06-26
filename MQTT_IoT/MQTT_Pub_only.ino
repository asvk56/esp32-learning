#include <WiFi.h>
#include <PubSubClient.h> // MQTT library

// Wi-Fi Network Credentials Placeholders
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// MQTT Broker Settings
const char* mqtt_broker = "broker.hivemq.com"; // Public MQTT broker
const int mqtt_port = 1883;                    // Default MQTT port
const char* mqtt_topic = "your/mqtt/topic";    // Change this to your unique topic

// Hardware Pin Configuration
const int pirPin = 22;      // Digital input pin connected to PIR sensor
const int ledPin = 2;       // On-board LED pin for status display

// Variables for Motion Detection and Timing
volatile bool motionDetected = false;
unsigned long lastTriggerTime = 0;
const unsigned long alertTimeout = 5000; // Time in milliseconds to keep the alert active (5 seconds)
bool stateSent = false;                  // Flag to ensure MQTT message is sent only once per trigger

// Create network client objects
WiFiClient espClient;
PubSubClient client(espClient);

// Interrupt Service Routine (ISR) - Runs instantly when physical motion is detected
void IRAM_ATTR motionISR() {
  motionDetected = true;
  lastTriggerTime = millis(); // Save the exact current hardware timestamp
}

// Function to handle Wi-Fi connection
void setup_wifi() {
  delay(10);
  Serial.println("\nConnecting to Wi-Fi...");
  WiFi.mode(WIFI_STA); // Set ESP32 to station mode
  WiFi.begin(ssid, password);

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// Function to handle MQTT broker connection and automatic retries
void reconnect() {
  // Loop until the client successfully connects
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Generate a unique random Client ID to avoid conflicts on the broker
    String clientId = "ESP32Client-" + String(random(0, 1000));
    
    // Try to connect to the broker
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected to MQTT Broker successfully!");
    } else {
      // If connection fails, print the error state code and try again
      Serial.print("Connection failed, state code = ");
      Serial.print(client.state());
      Serial.println(" Retrying in 2 seconds...");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200); // Initialize serial monitoring baud rate
  
  // Set up pin modes
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Turn off LED by default
  
  pinMode(pirPin, INPUT);
  // Attach interrupt to monitor the PIR pin transitioning from LOW to HIGH
  attachInterrupt(digitalPinToInterrupt(pirPin), motionISR, RISING);

  // Initialize network and MQTT settings
  setup_wifi();
  client.setServer(mqtt_broker, mqtt_port);
}

void loop() {
  // Check if connection to the MQTT broker is lost
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // Internal processing loop to keep the MQTT connection alive

  // Real-time motion evaluation logic
  if (motionDetected) {
    digitalWrite(ledPin, HIGH); // Turn on physical LED to show active alert
    
    // Publish data to the cloud only ONCE when motion starts
    if (!stateSent) {
      Serial.println("Sending MQTT Payload: [ALERT: Motion Detected]");
      client.publish(mqtt_topic, "ALERT: Motion Detected");
      stateSent = true;
    }

    // Check if the 5-second timeout window has passed since the last movement
    if (millis() - lastTriggerTime > alertTimeout) {
      motionDetected = false;     // Reset the motion flag
      digitalWrite(ledPin, LOW);  // Turn off the physical LED
      
      Serial.println("Sending MQTT Payload: [STATUS: Safe]");
      client.publish(mqtt_topic, "STATUS: Safe");
      stateSent = false;          // Reset flag to prepare for the next detection event
    }
  }
}
