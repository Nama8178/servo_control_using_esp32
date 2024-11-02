#include <WiFi.h>
#include <ESP32Servo.h>

Servo servo1;  // Create servo object for servo 1
Servo servo2;  // Create servo object for servo 2
Servo servo3;  // Create servo object for servo 3

// GPIO pins for the servos
static const int servoPin1 = 13;
static const int servoPin2 = 12;
static const int servoPin3 = 14;

// Replace with your network credentials
const char* ssid     = "ssid";
const char* password = "password";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Decode HTTP GET values for each servo
String valueString1 = String(90); // Default position 90
String valueString2 = String(90);
String valueString3 = String(90);

// Time management for client connection
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  // Attach each servo to its pin
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available(); // Listen for incoming clients

  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = ""; // To hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) { // If there are bytes to read
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Send HTTP response headers
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // HTML and JavaScript to control three servos
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
            client.println("</head><body><h1>ESP32 with 3 Servos</h1>");

            // Servo 1 slider
            client.println("<p>Servo 1 Position: <span id=\"servoPos1\"></span></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider1\" onchange=\"updateServo('1', this.value)\" value=\"" + valueString1 + "\"/>");

            // Servo 2 slider
            client.println("<p>Servo 2 Position: <span id=\"servoPos2\"></span></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider2\" onchange=\"updateServo('2', this.value)\" value=\"" + valueString2 + "\"/>");

            // Servo 3 slider
            client.println("<p>Servo 3 Position: <span id=\"servoPos3\"></span></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider3\" onchange=\"updateServo('3', this.value)\" value=\"" + valueString3 + "\"/>");

            // JavaScript to handle slider inputs and update positions
            client.println("<script>");
            client.println("function updateServo(servo, pos) {");
            client.println("$.get(\"/?servo=\" + servo + \"&value=\" + pos + \"&\"); }");
            client.println("document.getElementById('servoSlider1').oninput = function() { document.getElementById('servoPos1').innerHTML = this.value; };");
            client.println("document.getElementById('servoSlider2').oninput = function() { document.getElementById('servoPos2').innerHTML = this.value; };");
            client.println("document.getElementById('servoSlider3').oninput = function() { document.getElementById('servoPos3').innerHTML = this.value; };");
            client.println("</script>");
            client.println("</body></html>");

            // Parse GET request and control servos
            if (header.indexOf("GET /?servo=") >= 0) {
              int servoIndex = header.substring(header.indexOf("servo=") + 6, header.indexOf("&value=")).toInt();
              int value = header.substring(header.indexOf("value=") + 6, header.indexOf("& HTTP")).toInt();

              // Rotate the appropriate servo based on servoIndex
              if (servoIndex == 1) {
                servo1.write(value);
                valueString1 = String(value);
              } else if (servoIndex == 2) {
                servo2.write(value);
                valueString2 = String(value);
              } else if (servoIndex == 3) {
                servo3.write(value);
                valueString3 = String(value);
              }
              Serial.print("Servo ");
              Serial.print(servoIndex);
              Serial.print(" set to ");
              Serial.println(value);
            }

            client.println(); // End of HTTP response
            break;
          } else { // New line received; reset currentLine
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = ""; // Clear header variable for next request
    client.stop();
    Serial.println("Client disconnected.");
  }
}
