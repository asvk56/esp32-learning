#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi Network Credentials Placeholders
const char* ssid = "YOUR_WIFI_SSID";         
const char* password = "YOUR_WIFI_PASSWORD"; 

// MQTT Broker Settings
const char* mqtt_broker = "broker.hivemq.com";
const int mqtt_port = 1883;

// Bidirectional MQTT Topics (Publishing and Subscribing channels)
const char* pub_topic = "your/room/telemetry"; // Channel where ESP32 talks (sends sensor data)
const char* sub_topic = "your/room/control";   // Channel where ESP32 listens (receives commands)

// Hardware Pins Layout
const int pirPin = 22;      
const int ledPin = 2;       

// Variables for State Management and Timing Loops
volatile bool motionDetected = false;
unsigned long lastTriggerTime = 0;
const unsigned long alertTimeout = 5000; 
bool stateSent = false;                  

WiFiClient espClient;
PubSubClient client(espClient);

// Interrupt Service Routine (ISR) for high-priority hardware events
void IRAM_ATTR motionISR() {
  motionDetected = true;
  lastTriggerTime = millis();
}

// Function to establish local Wi-Fi connectivity
void setup_wifi() {
  delay(10);
  Serial.println("\nConnecting to Wi-Fi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected successfully!");
}

// Asynchronous Callback Function - Triggered automatically by library when a cloud message drops
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Incoming message on channel: ");
  Serial.println(topic);

  // Reconstruct incoming raw byte array into a standard usable C++ string
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i]; // Typecast raw ASCII bytes to characters line-by-line
  }
  Serial.println("Decoded Message Content: " + message);

  // Command Execution Router Layer
  if (message == "LED_ON") {
    digitalWrite(ledPin, HIGH);
    Serial.println("Command Executed: Remote trigger activated on-board LED");
  } 
  else if (message == "LED_OFF") {
    digitalWrite(ledPin, LOW);
    Serial.println("Command Executed: Remote trigger deactivated on-board LED");
  }
}

// Function to build/restore network connectivity states with the broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Generate isolated random identifier names for the central server engine registry
    String clientId = "ESP32Client-" + String(random(0, 1000));
    
    // Request network verification passage from the broker
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected to broker successfully!");
      
      // Crucial: Re-register the listening stream node immediately upon fresh connection instance
      client.subscribe(sub_topic);
      Serial.println("Control channel subscription verified.");
    } else {
      Serial.print("Handshake failed, state return code = ");
      Serial.print(client.state());
      Serial.println(" Retrying sequence in 2 seconds...");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  pinMode(pirPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(pirPin), motionISR, RISING);

  setup_wifi();
  
  // Set up broker location tracking variables
  client.setServer(mqtt_broker, mqtt_port);
  
  // Hand over the local handler function pointer to the background listening engine
  client.setCallback(callback);
}

void loop() {
  // Enforce background connection persistence loops
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // Continuous heartbeat routine to pump telemetry streams and check callback queues

  // Standalone Hardware Automation Routing Thread
  if (motionDetected) {
    digitalWrite(ledPin, HIGH); 
    
    if (!stateSent) {
      Serial.println("Sending MQTT Payload: [ALERT: Motion Detected]");
      client.publish(pub_topic, "ALERT: Motion Detected");
      stateSent = true;
    }

    if (millis() - lastTriggerTime > alertTimeout) {
      motionDetected = false;
      digitalWrite(ledPin, LOW); 
      
      Serial.println("Sending MQTT Payload: [STATUS: Safe]");
      client.publish(pub_topic, "STATUS: Safe");
      stateSent = false; 
    }
  }
}
