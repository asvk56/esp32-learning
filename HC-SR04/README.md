# 06 Ultrasonic Sensor

* **Concept:** Non-blocking hardware timing using `millis()` and acoustic distance measurement.
* **Hardware:** HC-SR04 sensor connected via a safe 3-resistor (1kΩ each) voltage divider to scale the 5V Echo signal down to 3.33V on input-only pin (GPIO 34), with Trig on GPIO 32.
* **Learning:** Implementing time-elapsed arithmetic (`currentMillis - previousMillis`) for non-blocking task pacing to replace `delay()`. Dynamically resetting timestamps to shift execution baselines, and applying physics formulas for pulse travel time.
