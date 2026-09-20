# Automatic_Water_Level_Controller
Automatic Water Level Controller using ESP32
# 💧 ESP32 Automatic Water Level Controller

An IoT-based **Automatic Water Level Controller** using ESP32, an ultrasonic sensor, and a relay-controlled water pump. The system monitors the water level in a tank and automatically controls the pump based on the water level.

It also provides a **Wi-Fi Access Point and Web Interface** for monitoring and manually controlling the pump.

---

## 📌 Project Features

* Automatic water level monitoring
* Ultrasonic sensor-based water level measurement
* Automatic pump ON when water level is **20% or below**
* Automatic pump OFF when water level reaches **90% or above**
* Manual pump control
* Automatic and Manual mode
* ESP32 built-in Wi-Fi Access Point
* Web-based water level monitoring
* Displays:

  * Water level percentage
  * Sensor distance
  * Current mode
  * Pump status

---

## 🛠️ Components Required

| Component                 |    Quantity |
| ------------------------- | ----------: |
| ESP32 Development Board   |           1 |
| HC-SR04 Ultrasonic Sensor |           1 |
| 5V Relay Module           |           1 |
| DC Water Pump             |           1 |
| Water Tank                |           1 |
| Jumper Wires              | As required |
| Power Supply              |           1 |

---

## 🔌 Pin Configuration

| Component       | ESP32 Pin |
| --------------- | --------- |
| Ultrasonic TRIG | GPIO 5    |
| Ultrasonic ECHO | GPIO 18   |
| Relay           | GPIO 23   |

### Pin Definitions

```cpp
#define TRIG_PIN 5
#define ECHO_PIN 18
#define RELAY_PIN 23
```

---

## ⚙️ Working Principle

The ultrasonic sensor measures the distance between the sensor and the water surface.

The ESP32 calculates the water level percentage from the measured distance.

### Automatic Mode

The system works according to the following conditions:

```text
Water Level ≤ 20%
        ↓
    Pump ON
        ↓
Water Level increases
        ↓
Water Level ≥ 90%
        ↓
    Pump OFF
```

This helps maintain the water level automatically.

---

## 🌐 Web Interface

The ESP32 creates its own Wi-Fi Access Point.

### Wi-Fi Information

```text
SSID: Water_Controller
Password: 12345678
```

After connecting your phone or computer to the ESP32 Wi-Fi network, open the IP address shown in the Arduino Serial Monitor.

Usually the Access Point IP is:

```text
192.168.4.1
```

The web interface displays:

* 💧 Water Level
* 📏 Distance
* ⚙️ Operating Mode
* 🚰 Pump Status

The user can also switch between:

```text
AUTOMATIC
MANUAL
```

In Manual Mode, the pump can be turned ON or OFF from the web interface.

---

## 📐 Tank Configuration

The current code uses:

```cpp
const float TANK_DEPTH = 9.0;
const float SENSOR_OFFSET = 3.0;
```

Where:

* `TANK_DEPTH` = Empty tank depth from sensor to bottom
* `SENSOR_OFFSET` = Distance from sensor to maximum water level

These values should be adjusted according to the actual tank dimensions.

---

## 💻 Software Requirements

* Arduino IDE
* ESP32 Board Package
* ESP32 WiFi Library
* ESP32 WebServer Library

### Required Libraries

```cpp
#include <WiFi.h>
#include <WebServer.h>
```

---

## 🚀 How to Upload the Code

1. Install **Arduino IDE**.
2. Install the **ESP32 board package**.
3. Connect the ESP32 to your computer.
4. Open `Water_Level_Controller.ino`.
5. Select the correct ESP32 board.
6. Select the correct COM port.
7. Click **Upload**.
8. Open **Serial Monitor** at `115200` baud.
9. Connect your phone to:

```text
Water_Controller
```

10. Enter the password:

```text
12345678
```

11. Open the ESP32 IP address in your browser.

---

## 📁 Project Structure

```text
Automatic-Water-Level-Controller/
│
├── Automatic-Water_Level_Controller.ino
│
├── circuit/
│   └── circuit_diagram.png
│
├── README.md
│
└── presentation/
    └── project_presentation.pdf
```

---

## 🔄 System Flow

```text
        Start
          ↓
      ESP32 Setup
          ↓
   Start Wi-Fi Access Point
          ↓
   Read Ultrasonic Sensor
          ↓
 Calculate Water Level (%)
          ↓
    ┌───────────────┐
    │ Automatic Mode│
    └───────┬───────┘
            ↓
     Level ≤ 20% ?
       /          \
     Yes           No
      ↓             ↓
   Pump ON     Level ≥ 90% ?
                    ↓
                  Yes
                    ↓
                 Pump OFF

          ↓
     Web Interface
          ↓
 Monitor / Manual Control
```

---

## 👥 Group Project

This project is developed as a group project.

### Team Members

* Member 1:Jahanara_Sultana_Nipa_761
* Member 2:Subarna_Roy_989
* Member 3:Rajoanul _Islam_1009
* Member 4: Gazi_Enamul_Haque_Ratul_1008

---

## 🎯 Project Objective

The main objective of this project is to develop a simple and low-cost system that can automatically monitor and control the water level of a tank.

The ESP32 provides both **automatic pump control** and **web-based monitoring**, making the system suitable for a basic IoT-based water management application.

---

## 📜 License

This project is developed for educational and academic purposes.
