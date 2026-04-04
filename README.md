# Wireless-ESP32-Macro-Keyboard-with-Multi-Profile-Automation-
I recently developed a wireless macro controller using ESP32 and ESP-NOW, designed to improve productivity and automate daily tasks.

## Key Features


**Ultra-Fast Wireless:** Uses ESP-NOW protocol instead of Bluetooth/WiFi for instant communication between the remote and the PC receiver.

**Plug & Play:** The receiver acts as a standard USB Keyboard. No companion software needed on the PC.

**3 Smart Profiles (Layer System):** Multiply your physical buttons. 4 buttons give you up to 16 different actions.

**Profile 1:** Windows Navigation (Lock, Desktop, Copy, Paste)

**Profile 2:** Media Controller (Play/Pause, Mute, Vol Up, Vol Down)

**Profile 3:** The Automation Mode (One-click launch Workspace, AI Assistants, and CMD)

**Short Press & Long Press Logic:** Every button handles both short clicks and long holds for different shortcuts.

---

## Hardware Requirements

1. **The Master (Wireless Remote):** Standard `ESP32-S` (or ESP32 DevKit V1) + 4x Push Buttons.
2. **The Slave (PC Receiver):** `ESP32-S3` (Must support Native USB HID) + 4x LEDs.
3. Power source for the Master (Power bank or LiPo battery).
4. Breadboards and Jumper Wires.

---

## Wiring Guide

### Master (Sender / Remote)
Connect your 4 push buttons to the following pins. **Note:** We use `INPUT_PULLUP` in the code, so wire the other leg of every button directly to `GND`. No external resistors needed!
Button 1 to `GPIO 12`
Button 2 to `GPIO 13`
Button 3 to `GPIO 14`
Button 4 to `GPIO 27`

### Slave (Receiver / PC Dongle)
Connect your 4 indicator LEDs (with appropriate current-limiting resistors) to:
LED 1 to `GPIO 4`
LED 2 to `GPIO 5`
LED 3 to `GPIO 3`
LED 4 to `GPIO 7`

---

## Software & Setup

### 1. Find Your Slave's MAC Address
Before uploading, you need to know the MAC address of your ESP32-S3. Run a basic WiFi MAC address scanner sketch on the S3, note down the 6-byte address, and update this line in the **Master Code**:
```cpp
uint8_t broadcastAddress[] = {0xB8, 0xF8, 0x62, 0xF7, 0xEF, 0xC4}; // REPLACE THIS WITH YOUR SLAVE'S MAC
```

### 2. Flashing the Master (ESP32-S)
* Open the Master Code in Arduino IDE.
* Select Board: **DOIT ESP32 DEVKIT V1** (or your specific ESP32 board).
* Upload the code.

### 3. Flashing the Slave (ESP32-S3)
* **CRITICAL IDE SETTINGS:** For the ESP32-S3 to act as a keyboard, you *must* configure the Tools menu correctly before uploading:
  * **Board:** `ESP32S3 Dev Module`
  * **USB Mode:** `Hardware CDC and JTAG`
  * **USB Functions:** `HID` ⚠️ *(This is mandatory)*
* Connect the S3 via its **USB/OTG** port (not the UART port).
* Upload the code.

---

## How to Use

1. Leave the **Slave (ESP32-S3)** plugged into your computer's USB port.
2. Power on the **Master (ESP32-S)**.
3. The Slave will light up LED 1, indicating you are in Profile 1.
4. **Change Profiles:** Press and hold **Button 4** for 1 second. The LEDs will flash, and it will switch to Profile 2 (Media Mode).
5. **Global Shortcuts:** No matter what profile you are in, holding Buttons 1, 2, or 3 will instantly open Task Manager, File Explorer, or close the current window!
