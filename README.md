[README.md](https://github.com/user-attachments/files/29653026/README.md)
# VoltQ - LCR Diagnostic Hub
### IoT-Enabled Intelligent SparkScan System & Component Profiler

<img width="1024" height="559" alt="Untitled - July 04, 2026 at 10 00 51" src="https://github.com/user-attachments/assets/ff64b6c6-0329-4f0f-91df-6b8646e4cca0" />


An automated, software-defined hardware testing station that automatically detects, measures, and diagnoses electronic components. The system utilizes an **Arduino Uno** for precision real-time analog data capture and an **ESP32** to serve a live web telemetry dashboard over a local wireless network — turning what used to be a manual, multimeter-driven process into a fully automated diagnostic pipeline.

---

## Why This Project Exists

During electronic circuit design and prototyping, manually inspecting components is one of the most time-consuming and error-prone parts of the workflow. Engineers and hobbyists frequently lose hours to:

- Cross-referencing resistor color codes by eye, especially on faded or unmarked components
- Manually switching multimeter dial modes (Ω / capacitance / diode) for every single component
- Debugging multi-stage capacitor states, since most multimeters can't safely discharge a cap before reading it
- Checking semiconductors (LEDs, diodes) for damage without accidentally misreading them as resistors due to similar low-current behavior
- Verifying whether a voltage regulator is actually dead or just showing a phantom/floating voltage

VoltQ was built to eliminate this friction entirely. Instead of a human deciding *what* a component is and *which* test mode to use, the firmware itself runs a detection state machine, classifies the component, executes the correct measurement routine, and pushes the result to a browser dashboard in real time — no manual mode-switching, no guesswork, no risk of misreading a component due to human error.

---

## Key Features & Efficiency Design

* **Universal 2-Pin Measuring Slot** — An integrated auto-detect state machine replaces multi-dial multimeters, instantly differentiating between resistors, capacitors, and diodes in a single hardware slot. The firmware applies a sequence of test conditions and inspects the component's electrical response curve (linear vs. exponential vs. forward-voltage-drop) to determine what's actually connected before selecting the correct measurement routine.

* **Non-Linear Semiconductor Discrimination** — Features an advanced dual-current scanning method that monitors voltage behavior under changing current loads. Since resistors follow Ohm's Law (linear V–I relationship) while diodes and LEDs exhibit a fixed forward-voltage knee, comparing readings at two different drive currents reliably separates the two — preventing high-forward-voltage components (like white LEDs) from being falsely read as high-value resistors.

* **Active Protection Architecture** — Automatically triggers microsecond discharge cycles to bleed out any residual passive energy from capacitors before running timing calculations. This protects both the accuracy of the RC-constant measurement and the microcontroller's analog input pins from unexpected voltage spikes.

* **Dynamic Range Stretching** — Optimizes the standard Arduino ADC thresholds to successfully read passive component values up to 180 kΩ using a fixed 1 kΩ reference bridge, extending the effective measurement range well beyond what a naive voltage-divider approach would normally allow.

* **Active 3-Pin Regulator Diagnostics** — Uses a dedicated testing rail integrated with a pull-down load network to drain high-impedance phantom voltages, accurately isolating genuinely functional voltage regulators from dead or leaking ones (a common false-positive with passive-only test rigs).

* **Wireless Inventory Telemetry** — Employs a hardware level-shifting network (5V to 3.3V logic) to safely pipe real-time diagnostic data from the Arduino Uno directly to an ESP32, which hosts an asynchronous, dark-themed local web dashboard accessible from any device on the same Wi-Fi network.

---


## How It Works — Detection Logic Overview

1. **Component inserted** into the universal 2-pin slot.
2. **Initial probe stage** — a known reference voltage/current is applied, and the Arduino measures the resulting response.
3. **Classification** — the firmware compares the response shape against known signatures:
   - Linear, proportional response → **resistor** (value calculated via the reference-bridge ratio)
   - Exponential charge/discharge curve → **capacitor** (RC time constant extracted after a safe discharge cycle)
   - Fixed forward-voltage knee under dual-current scan → **diode / LED**
4. **Routine dispatch** — based on the classification, the correct measurement subroutine runs (resistance calc, capacitance calc, or forward-voltage/health check).
5. **Result packaging** — the Arduino sends the structured result over the level-shifted serial link to the ESP32.
6. **Telemetry push** — the ESP32 serves the result to the web dashboard, where the JavaScript Fetch API polls at 1-second intervals to keep the UI live without page reloads.
7. **Session logging** — every reading is appended to a scrollable, chronological history feed for later review.

---
<img width="1280" height="963" alt="WhatsApp Image 2026-07-04 at 10 49 53 AM" src="https://github.com/user-attachments/assets/e6bd5584-9aaa-4a1f-8f21-54aa4ba1fd4b" />


## System Architecture

### Hardware Components
* Arduino Uno (ATmega328P Core) — handles all analog measurement and component classification logic
* ESP32 Development Module — handles Wi-Fi networking and web dashboard hosting
* 16x2 I2C Liquid Crystal Display (LCD) — local at-a-glance readout
* Resistor Network (1 kΩ and 2 kΩ) — used for UART logic level-shifting between the 5V Arduino and 3.3V ESP32
* Dedicated Pull-down Network (1 kΩ / 10 kΩ) — load resistors for static/phantom voltage bleed on the regulator rail
* Custom 2-Pin and 3-Pin Breadboard Testing Slots — universal component slot and regulator test rail respectively
* Input components to test (resistors, capacitors, diodes, LEDs, 5V regulators)

<img width="1600" height="1204" alt="image" src="https://github.com/user-attachments/assets/c42d95fa-5c02-4593-9f3f-9756fa2c1efa" />

### Software Stack
* **Firmware:** Embedded C++ (Arduino Core) — state machine for detection, measurement routines, serial protocol to ESP32
* **Networking:** ESP32 Wi-Fi stack, local Port 80 web server
* **Frontend Web Dashboard:** HTML5, CSS3 (responsive, dark-themed dashboard), JavaScript (Fetch API for 1-second interval live updates without page reloads)

<img width="2718" height="1568" alt="Untitled - July 03, 2026 at 18 52 34" src="https://github.com/user-attachments/assets/4aaa799b-c47c-423b-a44a-c0cb9e31d652" />

---

## Getting Started

### Requirements
* Arduino IDE (or PlatformIO)
* ESP32 board support package installed in Arduino IDE
* Arduino Uno + ESP32 dev board
* Components listed above wired per the schematic

### Setup
1. Clone this repository.
2. Flash the Arduino Uno firmware (`/firmware/uno`) via the Arduino IDE.
3. Flash the ESP32 firmware (`/firmware/esp32`) with your local Wi-Fi credentials configured.
4. Power the system and note the IP address printed to Serial by the ESP32.
5. Connect any device on the same Wi-Fi network and navigate to that IP address in a browser to view the live dashboard.
6. Insert a component into the 2-pin (passive/diode) or 3-pin (regulator) test slot and watch the classification and reading appear in real time.

<img width="1280" height="963" alt="image" src="https://github.com/user-attachments/assets/88b584ea-e65e-4f3e-84d5-39ee0371d3a5" />


---

## Future Improvements
* Expand auto-detection to include inductors and transistors
* Add historical data export (CSV) from the session log
* Support multiple simultaneous test slots
* Mobile-optimized dashboard layout

---

