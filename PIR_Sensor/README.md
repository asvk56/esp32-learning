# 08 Motion Interrupt

* **Concept:** Hardware Interrupt driven asynchronous event handling to optimize CPU overhead.
* **Hardware:** HC-SR501 PIR Motion Sensor powered via 5V (VIN) with a safe 3.3V native logic output routed to GPIO 13.
* **Learning:** Utilizing the `IRAM_ATTR` compiler attribute to lock the Interrupt Service Routine (ISR) inside the internal SRAM, preventing flash cache synchronization crashes on ESP32.
