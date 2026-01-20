# Arduino Floor Wax Dispenser Robot

An Arduino-based robotic car designed to dispense and spread liquid floor wax efficiently.  
The robot uses a car robot kit with DC motors for movement, a servo motor for controlling a dispensing valve, and IR remote navigation for manual control.

This project demonstrates practical applications of embedded systems, robotics, and basic automation using Arduino.

---

## 🚗 Project Overview

The Floor Wax Dispenser Robot is a mobile platform capable of:
- Navigating floors via IR remote control
- Dispensing liquid wax through a servo-controlled open/close mechanism
- Spreading wax evenly as it moves

All logic and control are implemented using **Arduino code only**.

---

## 🛠️ Hardware Components

- Arduino board (Uno / Nano / compatible)
- Car robot chassis kit
- 2× TT DC gear motors
- L298N motor driver module
- Servo motor (for valve or door mechanism)
- IR receiver module
- IR remote control
- Liquid container (for floor wax)
- Tubing / nozzle for dispensing
- Power supply (battery pack)
- Jumper wires

---

## ⚙️ System Functionality

### 🔹 Movement Control
- Two TT DC motors controlled via the **L298N motor driver**
- Supports forward, backward, left, and right movement

### 🔹 Wax Dispensing Mechanism
- Servo motor acts as an **open/close valve**
- Controls the flow of liquid wax from the container

### 🔹 Navigation
- Robot is navigated using an **IR remote**
- IR signals are decoded by the Arduino to control movement and dispensing actions

---

## 🧠 Control Logic

- IR remote buttons are mapped to:
  - Robot movement directions
  - Start/stop wax dispensing
- Servo motor rotates to specific angles to open or close the wax outlet
- Motors are driven through PWM signals for controlled movement

---

## 📂 Project Structure
arduino-floor-wax-dispenser-robot/
│
├── src/
│ └── floor_wax_robot.ino
│
├── docs/
│ └── wiring_diagram.png
│
└── README.md


---

## 🚀 Getting Started

1. Assemble the robot chassis and motors
2. Connect the L298N motor driver to the Arduino
3. Attach the servo motor to the dispensing mechanism
4. Wire the IR receiver to the Arduino
5. Upload the Arduino sketch
6. Control the robot using the IR remote

---

## 🔌 Libraries Used

- `IRremote`
- `Servo`

*(Install via Arduino Library Manager)*

---

## 📌 Notes

- This project focuses on **manual control via IR remote**
- Can be extended to include:
  - Autonomous navigation
  - Timed dispensing
  - Sensor-based obstacle avoidance

---

## 📜 License

This project is open-source and intended for educational and experimental use.

