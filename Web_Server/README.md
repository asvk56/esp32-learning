# 10 Web Server

This folder contains the evolution of the ESP32 Web Server project, moving from a basic network actuator control to an integrated interrupt-driven security dashboard.

## 📁 Files inside this directory:
1. **`Web_Server.ino`**: Standard synchronous HTTP server to control the native on-board blue status LED (GPIO 2) via web buttons.
2. **`PIR_Integration.ino`**: Advanced integrated version featuring a hardware interrupt-driven PIR Motion Sensor (GPIO 22) that dynamically renders a live "Room Status" alert layer on the web page.

## 🛠️ Key Technical Learnings:
* Initializing standard Port 80 HTTP server stacks on the ESP32.
* Capturing high-accuracy physical hardware timestamps inside an ISR (`IRAM_ATTR`).
* Serving standard UTF-8 charset HTTP payloads to fix character encoding glitches (emojis layout).
* Embedding dynamic conditional C++ logic directly inside raw HTML string responses.
