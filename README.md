# ESP32 Web-Based Servo Control for 3 Servos

This project allows you to control three servo motors connected to an ESP32 microcontroller through a web interface. Using Wi-Fi, the ESP32 sets up a web server, and you can access it via any device on the same network. The web page provides sliders to adjust each servo motor's angle independently.

## Features

- Web-based interface for controlling 3 servo motors.
- Sliders to control each servo's position from 0° to 180°.
- Real-time feedback on slider values.

## Components

- **ESP32**: Used to host the web server and control the servos.
- **3 Servo Motors**: Controlled via GPIO pins on the ESP32.
- **Wi-Fi Network**: Required to connect to the ESP32’s web server.

## Circuit Diagram

Connect each servo to a GPIO pin on the ESP32:
- **Servo 1**: Connect to GPIO 13.
- **Servo 2**: Connect to GPIO 12.
- **Servo 3**: Connect to GPIO 14.
  
Ensure each servo has its own power source if they require more current than the ESP32 can supply.

## Code Overview

1. **Libraries Used**
   - `WiFi.h`: For connecting the ESP32 to Wi-Fi.
   - `ESP32Servo.h`: For controlling servo motors on the ESP32.

2. **Web Server Setup**
   - Sets up a web server on port 80, allowing clients to connect and control the servos through a web interface.

3. **HTML Interface**
   - The ESP32 serves an HTML page with three sliders for adjusting the position of each servo.
   - JavaScript code on the web page sends requests to the ESP32 to update the servo positions in real-time.

4. **Servo Control**
   - The code reads GET requests from the client to adjust servo positions based on slider values.

## Getting Started

### Prerequisites

- **Arduino IDE**: Install the ESP32 board support package in the Arduino IDE.
- **ESP32 Servo Library**: Install the `ESP32Servo` library via the Library Manager.

### Installation

1. Clone or download this repository.
2. Open the `ESP32_Servo_Control.ino` file in the Arduino IDE.
3. Update the following Wi-Fi credentials with your network information:
   ```cpp
   const char* ssid = "YOUR_SSID";
   const char* password = "YOUR_PASSWORD";
