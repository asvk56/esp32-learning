// Define the GPIO pin for the PIR motion sensor output
const int pirPin = 13;

// Volatile flag shared between the ISR and the main loop execution thread
volatile bool motionDetected = false;

// Variables to keep track of timing for alert resetting
unsigned long lastTriggerTime = 0;
const unsigned long alertTimeout = 5000; // Keep the alert active for 5 seconds

// ==========================================
// HARDWARE INTERRUPT SERVICE ROUTINE (ISR)
// Loaded directly into internal SRAM via IRAM_ATTR
// ==========================================
void IRAM_ATTR motionISR() {
  motionDetected = true;
}

void setup() {
  // Initialize high-speed hardware UART telemetry at 115200 baud rate
  Serial.begin(115200);

  // Set PIR pin as INPUT (it relies on internal sensor pull dynamics)
  pinMode(pirPin, INPUT);

  // Attach the hardware interrupt to trigger on a RISING edge (LOW to HIGH transition)
  attachInterrupt(digitalPinToInterrupt(pirPin), motionISR, RISING);

  Serial.println("Security System Activated... Waiting for motion.");
}

void loop() {
  // Check if the hardware interrupt raised the security flag
  if (motionDetected) {
    Serial.println("🚨 ALERT!!! Motion Detected in the Room!");
    
    // Store the exact time when motion was detected
    lastTriggerTime = millis();
    
    // Clear the volatile flag so we can catch the next hardware trigger
    motionDetected = false;
  }

  // Optional: Non-blocking routine to check if 5 seconds have passed since last motion
  if (lastTriggerTime > 0 && (millis() - lastTriggerTime > alertTimeout)) {
    Serial.println("System Secure again.");
    lastTriggerTime = 0; // Reset timer tracking
  }
}
