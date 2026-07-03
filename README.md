# VoltQ - LCR Diagnostic Hub
# IoT-Enabled Intelligent SparkScan System & Component Profiler

<img width="1024" height="559" alt="image" src="https://github.com/user-attachments/assets/5d5a85cb-ba66-48c2-9f9b-708adfff259a" />


An automated, software-defined hardware testing station that automatically detects, measures, and diagnoses electronic components. The system utilizes an Arduino Uno for precision real-time analog data capture and an ESP32 to serve a live web telemetry dashboard over a local wireless network.

##  Why This Project Exists
During electronic circuit design, manually inspecting components can be incredibly time-consuming. Hours are frequently lost cross-referencing resistor color codes, debugging multi-stage capacitor states with standard multimeters, and checking for damaged semiconductors. 

This repository provides an automated solution that shifts component profiling from tedious manual measurement to intelligent embedded firmware automation.

---

##  Key Features & Efficiency Design
* **Universal 2-Pin Measuring Slot:** An integrated auto-detect state machine replaces multi-dial multimeters, instantly differentiating between resistors, capacitors, and diodes in a single hardware slot.
* **Non-Linear Semiconductor Discrimination:** Features an advanced dual-current scanning method that monitors voltage behavior under changing current loads. This prevents high-forward-voltage components (like white LEDs) from being falsely read as resistors.
* **Active Protection Architecture:** Automatically triggers microsecond discharge cycles to bleed out passive energy from capacitors before running timing calculations, protecting the microcontroller pins.
* **Dynamic Range Stretching:** Optimizes the standard Arduino ADC thresholds to successfully read passive component values up to 180 kΩ using a fixed 1 kΩ reference bridge.
* **Active 3-Pin Regulator Diagnostics:** Uses a dedicated testing rail integrated with a pull-down load network to drain high-impedance phantom voltages, accurately isolating functional voltage regulators from dead ones.
* **Wireless Inventory Telemetry:** Employs a hardware level-shifting network (5V to 3.3V logic) to pipe real-time diagnostic data from the Arduino Uno directly to an ESP32 hosting an asynchronous, dark-themed local web dashboard.

---

##  System Architecture

### Hardware Components
* Arduino Uno (ATmega328P Core)
* ESP32 Development Module
* 16x2 I2C Liquid Crystal Display (LCD)
* Resistor Network ($1\text{ k}\Omega$ and $2\text{ k}\Omega$ for UART logic level-shifting)
* Dedicated Pull-down Network ($1\text{ k}\Omega$ / $10\text{ k}\Omega$ load resistor for static bleed)
* Custom 2-Pin and 3-Pin Breadboard Testing Slots
* Input compoenets to test

<img width="1600" height="1204" alt="image" src="https://github.com/user-attachments/assets/c42d95fa-5c02-4593-9f3f-9756fa2c1efa" />


### Software Stack
* **Firmware:** Embedded C++ (Arduino Core)
* **Frontend Web Dashboard:** HTML5, CSS3 (responsive dashboard), JavaScript (Fetch API for 1-second interval live updates)

<img width="2718" height="1568" alt="Untitled - July 03, 2026 at 18 52 34" src="https://github.com/user-attachments/assets/4aaa799b-c47c-423b-a44a-c0cb9e31d652" />



---
