// Define physical pin allocations based on the voltage divider circuit
const int trigPin = 32;  // Output pin to trigger the ultrasonic burst
const int echoPin = 34;  // Safe Input pin (via divider) to read the echo pulse

// Timing control variables for non-blocking execution
unsigned long previousMillis = 0;   // Stores the last time the sensor was sampled
const long interval = 200;          // Sampling interval in milliseconds (5 Hz refresh rate)

void setup() {
  // Initialize high-speed hardware UART telemetry at 115200 baud
  Serial.begin(115200);
  
  // Set pin modes explicitly
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Get the current system uptime clock ticks in milliseconds
  unsigned long currentMillis = millis();
  
  // Non-blocking condition checked against the defined time interval
  if (currentMillis - previousMillis >= interval) {
    // Save the execution time stamp for the current cycle
    previousMillis = currentMillis;
    
    // 1. Clear the trigger pin to ensure a clean HIGH pulse
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // 2. Generate a precise 10-microsecond trigger pulse to fire ultrasonic bursts
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // 3. Read the incoming pulse travel time duration in microseconds
    // pulseIn() blocks temporarily only for the wave return duration, not for fixed intervals
    long duration = pulseIn(echoPin, HIGH);
    
    // 4. Mathematical computation using explicit floating point arithmetic
    // Distance = (Time * Speed of Sound) / 2
    float distance = (duration * 0.0343) / 2.0;
    
    // 5. Stream calculated distance values out to the Serial Monitor
    Serial.print("Target Distance: ");
    if (duration == 0) {
      Serial.println("Out of Range!");
    } else {
      Serial.print(distance, 1); // Print with one decimal place precision
      Serial.println(" cm");
    }
  }
}
