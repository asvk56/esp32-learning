// Define the GPIO pin numbers for the peripheral components
const int buttonPin = 22;  // Push-button connected to GPIO 22
const int ledPin = 4;      // External LED connected to GPIO 4

void setup() {
  // Initialize the LED pin as an OUTPUT to control the voltage level (3.3V/0V)
  pinMode(ledPin, OUTPUT);
  
  // Initialize the button pin as an INPUT with the internal pull-up resistor enabled.
  // This keeps the pin state at HIGH (3.3V) by default when the switch is open.
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // Read the current digital state of the push-button pin (returns HIGH or LOW)
  int buttonState = digitalRead(buttonPin);

  // Active-LOW Logic: When the button is pressed, the pin is shorted to Ground (LOW)
  if (buttonState == LOW) {
    digitalWrite(ledPin, HIGH);  // Turn the external LED ON
  } else {
    digitalWrite(ledPin, LOW);   // Turn the external LED OFF (when button is released)
  }
}
