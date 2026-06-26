# 11 MQTT IoT Architecture

This folder documents the transition from local web hosting to enterprise-grade cloud telemetry using the MQTT Protocol (Message Queuing Telemetry Transport) on the ESP32.

## 📁 Embedded Firmware Evolution:
1. **`11_MQTT_Publish_Only.ino`**: One-Way Telemetry. The ESP32 acts strictly as an MQTT Client publishing live hardware interrupt triggers from a PIR Sensor to a public HiveMQ Broker channel.
2. **`11_MQTT_Full_Loop.ino`**: Two-Way Command & Control. Integrated an asynchronous Callback Engine allowing the ESP32 to simultaneously publish sensor data and subscribe to external cloud payloads to toggle GPIOs (LED Control) remotely.

## 🛠️ Core Technical Competencies:
* Implementing the Publish/Subscribe architecture over standard Port 1883.
* Managing connection robustness using non-blocking state evaluation (`client.connected()`).
* Dynamic runtime memory optimization utilizing character pointers (`char*`) instead of heavy String buffers.
* Decoding raw network byte-arrays into character arrays via memory typecasting inside MQTT callbacks.
