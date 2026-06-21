// Define the safe ADC1 channel pin (GPIO 34) for analog input operations
const int analogPin = 34;

// Global tracking variable for loop execution pacing (100 milliseconds)
int dt = 100;

void setup() {
  // Initialize the primary hardware UART interface at a high-speed transmission rate of 115200 baud
  Serial.begin(115200);

  // Configure the designated GPIO pin explicitly as an input stage
  pinMode(analogPin, INPUT);
}

void loop() {
  // Sample the internal 12-bit successive-approximation register (SAR) ADC (Returns 0 to 4095)
  int rawVal = analogRead(analogPin);
  
  // Convert the digitized quantization steps back into physical absolute voltage values
  // Implicit type promotion handles precision fractional calculation due to the 4095.0 float literal
  float voltage = (rawVal / 4095.0) * 3.3;

  // Stream telemetry data back to the host machine over the virtual COM port (USB-UART bridge)
  Serial.print("Raw ADC Value: ");
  Serial.print(rawVal);

  Serial.print(" | Calculated Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  // Non-blocking wait interval to throttle system telemetry frequency and optimize CPU overhead
  delay(dt);
}
