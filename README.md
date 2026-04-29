# 🚪 Smart Doorstep Parcel Monitor

An IoT-style alert system using Arduino, an ultrasonic sensor, buzzer, LED, and push button. Detects when a package is left at your doorstep and alerts you — with a mute button that auto-resets after parcel removal.

## 📦 Features
- Ultrasonic parcel detection (< 30 cm)
- LED + buzzer alert
- One-press mute
- Auto re-arm after parcel picked up
- Same hardware as Smart Pet Feeder — just different code

## 🛠 Hardware
- Arduino Uno/Nano
- HC-SR04 Ultrasonic Sensor
- Active Buzzer
- 5mm Red LED + 330Ω resistor
- Momentary Push Button
- Breadboard & jumper wires

## 🔌 Wiring
| Component | Arduino Pin |
|-----------|-------------|
| Trigger (HC-SR04) | D11 |
| Echo (HC-SR04) | D10 |
| Buzzer (+) | D5 |
| LED Anode | D6 |
| Button | D4 |
| All GNDs | GND |

## 🧠 Logic Overview
1. Measure distance every 200ms.
2. If distance ≤ 30cm → alert ON (LED + buzzer).
3. Button press → mute buzzer, keep LED on.
4. If distance > 30cm → reset mute flag, turn off LED.

## Circuit
<img width="1887" height="1230" alt="PBL_Example_Circuit_Diagram" src="https://github.com/user-attachments/assets/cd892601-912d-45f3-b175-3d6e9503ebbf" />


## 📜 License
GNU General Public License v3.0
