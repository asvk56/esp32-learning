// Define the GPIO pin numbers for the input button and output LED
const int buttonPin = 22;  // Push-button connected to GPIO 22
const int ledPin = 4;      // External LED connected to GPIO 4

// Global variables to track state changes and filter mechanical noise
int ledState = LOW;          // Stores the current functional state of the LED (Starts as OFF)
int buttonState;             // Temporarily stores the active digital reading of the button
int lastButtonState = HIGH;  // Tracks the previous loop's button state (Default HIGH due to INPUT_PULLUP)

void setup() {
  // Initialize the LED pin as a digital output
  pinMode(ledPin, OUTPUT);
  
  // Enable the internal pull-up resistor for the input pin to prevent floating states
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Apply the default startup state (OFF) to the physical LED
  digitalWrite(ledPin, ledState);
}

void loop() {
  // Read the immediate voltage state of the push-button pin (HIGH or LOW)
  int reading = digitalRead(buttonPin);

  // State Change Detection: Check if the button's physical state has changed since the last loop
  if (reading != lastButtonState) {
    
    // Software Debounce: Wait for 50 milliseconds to bypass erratic mechanical contact vibrations (bouncing)
    delay(50);
    
    // Confirm the state after the bounce window. If it settles at LOW, the button was genuinely pressed.
    if (reading == LOW) {
      // Toggle Logic: Invert the state of the LED using the logical NOT operator (!)
      ledState = !ledState; 
      
      // Push the newly updated toggle state to the physical LED pin
      digitalWrite(ledPin, ledState);
    }
  }

  // Update the tracking history so the current loop's state becomes the historical reference for the next loop
  lastButtonState = reading;
}
