// Define the GPIO pin connected to the external LED
const int ledPin = 22;

// PWM Properties Configuration for the LEDC Peripheral
const int frequency = 5000;  // 5kHz PWM frequency to ensure flicker-free dimming
const int resolution = 8;    // 8-bit resolution establishes a duty cycle range of 0 to 255

void setup() {
  // New ESP32 v3.0+ API: Configures the hardware timer and maps it directly to the GPIO pin
  // This eliminates manual channel management required in older core versions
  ledcAttach(ledPin, frequency, resolution);
}

void loop() {
  // Fade In: Linearly step up the duty cycle from 0 (OFF) to 255 (Full Brightness)
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    // Directly update the PWM duty cycle for the specified physical pin
    ledcWrite(ledPin, dutyCycle);
    delay(5); // 5ms step delay for a smooth breathing/fading transition (~1.2s total duration)
  }

  // Fade Out: Linearly step down the duty cycle from 255 (Full Brightness) to 0 (Complete OFF)
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    // Directly update the PWM duty cycle for the specified physical pin
    ledcWrite(ledPin, dutyCycle);
    delay(5); // 5ms step delay
  }
}
