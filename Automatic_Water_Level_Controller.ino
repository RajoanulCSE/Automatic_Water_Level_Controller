#include <WiFi.h> 
#include <WebServer.h> 
 
// Wi-Fi Access Point information 
const char *ssid = "Water_Controller"; 
const char *password = "12345678"; 
 
// Pin configuration 
#define TRIG_PIN 5 
#define ECHO_PIN 18 
#define RELAY_PIN 23 
 
// Tank measurement (in centimeters) 
const float TANK_DEPTH = 9.0;   // Empty tank depth (from sensor to bottom) 
const float SENSOR_OFFSET = 3.0; // Distance from sensor to maximum water level 
 
// Control variables 
bool autoMode = true; 
bool pumpState = false; 
float waterLevelPercent = 0.0; 
float distanceCM = 0.0; 
 
unsigned long lastSensorRead = 0; 
const long interval = 1500; 
 
WebServer server(80); 
 
// Relay control function (Active-Low safety trick) 
void setPump(bool state) { 
  pumpState = state; 
  if (pumpState) { 
    pinMode(RELAY_PIN, OUTPUT); 
    digitalWrite(RELAY_PIN, LOW); // Relay ON 
  } else { 
    pinMode(RELAY_PIN, INPUT);     // Relay OFF (High-Z) 
  } 
} 
 
// Ultrasonic reading function 
void readUltrasonic() { 
  digitalWrite(TRIG_PIN, LOW); 
  delayMicroseconds(2); 
  digitalWrite(TRIG_PIN, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(TRIG_PIN, LOW); 
 
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout 
  if (duration > 0) { 
    distanceCM = (duration * 0.0343) / 2.0; 
     
    // Calculate water level percentage 
    float effectiveHeight = TANK_DEPTH - SENSOR_OFFSET; 
    float currentWaterHeight = TANK_DEPTH - distanceCM; 
    waterLevelPercent = (currentWaterHeight / effectiveHeight) * 100.0; 
 
    if (waterLevelPercent > 100.0) waterLevelPercent = 100.0; 
    if (waterLevelPercent < 0.0) waterLevelPercent = 0.0; 
  } 
} 
 
// HTML webpage 
String getHTML() { 
  String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>"; 
  html += "<title>Water Monitor</title>"; 
  html += "<style>body{font-family:Arial;text-align:center;margin-top:20px;background:#f4f7f6;}"; 
  html += ".card{background:white;padding:20px;margin:auto;max-width:350px;border-radius:12px;box-shadow:0 4px 10px rgba(0,0,0,0.1);}"; 
  html += ".btn{padding:12px 24px;font-size:16px;margin:8px;border:none;border-radius:6px;cursor:pointer;color:white;}"; 
  html += ".btn-on{background:#28a745;}.btn-off{background:#dc3545;}.btn-mode{background:#007bff;}"; 
  html += ".gauge{font-size:36px;font-weight:bold;color:#17a2b8;margin:15px 0;}</style></head><body>"; 
   
  html += "<div class='card'>"; 
  html += "<h2>Water Monitor</h2>"; 
  html += "<div class='gauge'>" + String((int)waterLevelPercent) + " %</div>"; 
  html += "<p>Distance: " + String(distanceCM, 1) + " cm</p>"; 
  html += "<p>Mode: <b>" + String(autoMode ? "AUTOMATIC" : "MANUAL") + "</b></p>"; 
  html += "<p>Pump Status: <b>" + String(pumpState ? "RUNNING" : "STOPPED") + "</b></p><hr>"; 
 
  html += "<a href='/toggleMode'><button class='btn btn-mode'>Switch Mode</button></a><br>"; 
  if (!autoMode) { 
    if (pumpState) { 
      html += "<a href='/pumpOff'><button class='btn btn-off'>Turn Pump OFF</button></a>"; 
    } else { 
      html += "<a href='/pumpOn'><button class='btn btn-on'>Turn Pump ON</button></a>"; 
    } 
  } else { 
    html += "<p style='color:gray;'><small>Manual buttons disabled in Auto mode</small></p>"; 
  } 
  html += "</div></body></html>"; 
  return html; 
} 
 
// Web root handler 
void handleRoot() { 
  server.send(200, "text/html", getHTML()); 
} 
 
void handleMode() { 
  autoMode = !autoMode; 
  server.sendHeader("Location", "/"); 
  server.send(303); 
} 
 
void handlePumpOn() { 
  if (!autoMode) setPump(true); 
  server.sendHeader("Location", "/"); 
  server.send(303); 
} 
 
void handlePumpOff() { 
  if (!autoMode) setPump(false); 
  server.sendHeader("Location", "/"); 
  server.send(303); 
} 
 
void setup() { 
  Serial.begin(115200); 
 
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT); 
  setPump(false); // Keep the pump OFF at startup 
 
  // Start the Wi-Fi Hotspot (Access Point) 
  WiFi.softAP(ssid, password); 
  IPAddress IP = WiFi.softAPIP(); 
  Serial.print("Access Point IP: "); 
  Serial.println(IP); 
 
  // Server routing 
  server.on("/", handleRoot); 
  server.on("/toggleMode", handleMode); 
  server.on("/pumpOn", handlePumpOn); 
  server.on("/pumpOff", handlePumpOff); 
 
  server.begin(); 
  Serial.println("Web server started"); 
} 
 
void loop() { 
  server.handleClient(); 
 
  // Read the sensor and run automation logic at a fixed interval 
  unsigned long currentMillis = millis(); 
  if (currentMillis - lastSensorRead >= interval) { 
    lastSensorRead = currentMillis; 
    readUltrasonic(); 
 
    // Automatic mode logic 
    if (autoMode) { 
      if (waterLevelPercent <= 20.0 && !pumpState) { 
        setPump(true);  // Turn the pump ON when the water level falls below 20% 
        Serial.println("Auto: Water low! Pump started."); 
      }  
      else if (waterLevelPercent >= 90.0 && pumpState) { 
        setPump(false); // Turn the pump OFF when the water level rises above 90% 
        Serial.println("Auto: Water full! Pump stopped."); 
      } 
    } 
  } 
}
```