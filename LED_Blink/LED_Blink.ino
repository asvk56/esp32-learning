// Define the GPIO pin number connected to the LED. 
// Using 'const' optimizes memory as this value will not change during runtime.
const int ledPin = 2;

// Define the delay time in milliseconds (500ms = 0.5 seconds)
int dt = 500;

void setup() {
  // Configure the specified GPIO pin as an OUTPUT to send voltage to the LED
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Turn the LED ON by supplying HIGH voltage (3.3V on ESP32) to the pin
  digitalWrite(ledPin, HIGH);
  
  // Pause the program execution for the duration specified by 'dt' (500ms)
  delay(dt);
  
  // Turn the LED OFF by bringing the voltage down to LOW (0V / Ground)
  digitalWrite(ledPin, LOW);
  
  // Pause the program again for 500ms before repeating the loop
  delay(dt);
}
