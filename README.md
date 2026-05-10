# WolfSense

An Arduino library for ESP32 to connect Flydigi Dire Wolf 3 gamepad over Bluetooth Low Energy (BLE). Full joystick and button support with simultaneous multi-button input, dead zone adjustment, and auto-reconnect.

> Made by Tony Huynh Tran — May 2026

---

## Requirements

- **Hardware:** ESP32 (any board with BLE support)
- **Arduino IDE:** 1.8.0 or later
- **Dependency:** `BLEDevice.h` — included in the ESP32 Arduino core, no extra install needed

---

## Installation

**Option 1 — Arduino IDE (recommended)**

1. Download this repo as a ZIP file
2. In Arduino IDE: **Sketch → Include Library → Add .ZIP Library...**
3. Select the downloaded ZIP → Done ✅

**Option 2 — Manual**

1. Download and extract the repo
2. Copy the folder into `Documents/Arduino/libraries/WolfSense/`
3. Restart Arduino IDE

---

## Switch Dire Wolf 3 to Bluetooth Mode

The Dire Wolf 3 supports both 2.4GHz dongle and Bluetooth modes. To use with ESP32, you must switch it to **Bluetooth mode**:

1. Turn on the controller
2. Hold **FN + A** until the indicator light **flashes blue**
3. The controller is now in Bluetooth pairing mode

> If the light does not flash blue, make sure the controller is fully powered on first, then try again.

---

## Finding the MAC Address

The MAC address tells the ESP32 which device to connect to. You only need to do this once.

1. Put the Dire Wolf 3 into Bluetooth mode (hold **FN + A** until blue light flashes)
2. On your phone, go to **Settings → Bluetooth** and scan for devices
3. Connect to the controller — it will appear as **"Dire Wolf"** or similar
4. Once connected, tap the device name to view its details — the **MAC address** is listed there (format: `XX:XX:XX:XX:XX:XX`)
5. Note it down and paste it into `SetMAC()` in your sketch

---

## Quick Start

```cpp
//  MAKE BY TONY HUYNH TRAN
//  IN 5/2026
//  HAVE A GOOD DAY MY FRIENDS

#include <WolfSense.h>

void setup()
{   
    Serial.begin(2000000);                  // High-speed baud rate for real-time raw data streaming, high-quality USB cable recommended
    WolfSense.SetMAC("A4:C1:38:37:60:E0");   // Replace with your gamepad's MAC address
    WolfSense.SetDeathZone(20);
    WolfSense.begin();
}

void loop()
{
    WolfSense.loop();
    if (WolfSense.Connected()) 
    {
        // READ ANALOG
        Serial.print("LX: ");
        Serial.print(WolfSense.JoyL_X);

        Serial.print(" | LY: ");
        Serial.print(WolfSense.JoyL_Y);

        Serial.print(" | RX: ");
        Serial.print(WolfSense.JoyR_X);

        Serial.print(" | RY: ");
        Serial.print(WolfSense.JoyR_Y);


        // READ BUTTONS 
        Serial.print(" | BTN: ");
        bool hasButton = false;

            // DPAD
            if (WolfSense.UP) {
                Serial.print("UP ");
                hasButton = true;
            }

            if (WolfSense.DOWN) {
                Serial.print("DOWN ");
                hasButton = true;
            }

            if (WolfSense.LEFT) {
                Serial.print("LEFT ");
                hasButton = true;
            }

            if (WolfSense.RIGHT) {
                Serial.print("RIGHT ");
                hasButton = true;
            }


            // ABXY
            if (WolfSense.A) {
                Serial.print("A ");
                hasButton = true;
            }

            if (WolfSense.B) {
                Serial.print("B ");
                hasButton = true;
            }

            if (WolfSense.X) {
                Serial.print("X ");
                hasButton = true;
            }

            if (WolfSense.Y) {
                Serial.print("Y ");
                hasButton = true;
            }


            // SHOULDER BUTTONS
            if (WolfSense.LB) {
                Serial.print("LB ");
                hasButton = true;
            }

            if (WolfSense.RB) {
                Serial.print("RB ");
                hasButton = true;
            }

            if (WolfSense.LT) {
                Serial.print("LT ");
                hasButton = true;
            }

            if (WolfSense.RT) {
                Serial.print("RT ");
                hasButton = true;
            }


            // SYSTEM BUTTONS
            if (WolfSense.START) {
                Serial.print("START ");
                hasButton = true;
            }

            if (WolfSense.SELECT) {
                Serial.print("SELECT ");
                hasButton = true;
            }

            if (WolfSense.L3) {
                Serial.print("L3 ");
                hasButton = true;
            }

            if (WolfSense.R3) {
                Serial.print("R3 ");
                hasButton = true;
            }


            // MACRO BUTTONS
            if (WolfSense.M1) {
                Serial.print("M1 ");
                hasButton = true;
            }

            if (WolfSense.M2) {
                Serial.print("M2 ");
                hasButton = true;
            }

            if (WolfSense.FN) {
                Serial.print("FN ");
                hasButton = true;
            }

            if (WolfSense.POWER) {
                Serial.print("POWER ");
                hasButton = true;
            }


            // NONE BUTTONS ACTIVATE
            if (!hasButton)
            {
                Serial.print("NONE");
            }
    }
    else
    {
        Serial.println();
        Serial.print("DISCONNECT");
    }
        Serial.println();
        delay(10);
}

```

---

## API Reference

### Methods

| Method | Description |
|--------|-------------|
| `WolfSense.SetMAC(mac)` | Set the controller's MAC address |
| `WolfSense.SetDeathZone(dz)` | Set joystick dead zone (0–127) |
| `WolfSense.begin()` | Initialize BLE and connect |
| `WolfSense.loop()` | Maintain connection — call every loop |
| `WolfSense.Connected()` | Returns `true` if controller is connected |
| `WolfSense.PrintRawData()` | Print raw BLE packet to Serial Monitor for debugging |

### Joystick Values

```
JoyL_X   -127 (left)   → +127 (right)
JoyL_Y   -127 (down)   → +127 (up)
JoyR_X   -127 (left)   → +127 (right)
JoyR_Y   -127 (down)   → +127 (up)
```

Returns `0` at center or within the dead zone.

### Buttons

```
D-Pad     UP, DOWN, LEFT, RIGHT
Face      A, B, X, Y
Shoulder  LB, RB, LT, RT
Stick     L3, R3
System    START, SELECT, FN, POWER
Macro     M1, M2
```

All buttons are `bool` — `true` while held, `false` when released.

---

## Example Sketch

The included `Example.ino` reads all joystick axes and buttons and prints them to Serial Monitor:

```
LX: +45 | LY: +0 | RX: -20 | RY: +10 | BTN: A LB
```

Use this to verify the controller is working correctly before integrating into your project. Recommended baud rate for real-time streaming: `2000000` (requires a quality USB cable).

---

## Troubleshooting

**Controller not connecting**
- Make sure the controller is in Bluetooth mode — hold **FN + A** until the blue light flashes
- Double-check the MAC address format: `XX:XX:XX:XX:XX:XX`
- Keep the controller close to the ESP32 on first connection

**Joystick drifting when not touched**
- Increase the dead zone: `WolfSense.SetDeathZone(30)`

**Connection keeps dropping**
- The library auto-reconnects after 2 seconds
- Check that your ESP32 has a stable power supply
- Reduce distance between controller and ESP32

**High latency**
- Keep `delay()` at 10ms or less in your `loop()`
- Use `Serial.begin(2000000)` for faster debug output if needed

---

## License

MIT License — Free to use in personal and commercial projects.
