# ESP32 Weather Clock

A connected dot-matrix weather clock built with an ESP32 and an FC-16 MAX7219 4-in-1 LED matrix module. Displays synchronized time via NTP, live local temperature via OpenWeatherMap API, and date with custom matrix typography and animations.

---

## Features

- **Accurate Timekeeping**: Synchronizes with NTP servers with automatic Daylight Saving Time (DST) support.
- **Configurable Time**: Switch between 12-hour and 24-hour formats.
- **Live Weather**: Fetches current temperature from the OpenWeatherMap API (refreshed every 10 minutes).
- **Configurable Units**: Display temperature in Fahrenheit (`°F`) or Celsius (`°C`).
- **Configurable Date**: Display date in `MM-DD` or `DD-MM`.
- **Configurable Brightness**: Control the LED matrix intensity (0-15).
- **Smooth Display Cycle**: Cycles every 15 seconds through three information views:
  1. **Time** (`HH:MM` or `12-hour` with blinking colon and spinning indicator icon)
  2. **Temperature** (current temperature in `°F` or `°C`)
  3. **Date** (`MM-DD` or `DD-MM`)
- **Centralized Secrets Management**: Wi-Fi, API credentials, display timings, and format settings are isolated in `include/secrets.h` (ignored by git).

---

## Hardware Requirements

- **Microcontroller**: ESP32 Development Board (e.g., NodeMCU-32S, ESP32 DevKit v1)
- **Display**: MAX7219 4-in-1 8x32 Dot Matrix LED Display Module (FC-16 type)
- **Power**: 5V Micro-USB or regulated 5V external power supply

### Pin Connections (ESP32 to MAX7219)

| MAX7219 Pin | ESP32 GPIO | Description |
| :--- | :--- | :--- |
| **VCC** | `5V` (VIN) | 5V Power Supply |
| **GND** | `GND` | Ground |
| **DIN** | `GPIO 23` | VSPI MOSI (Hardware SPI) |
| **CS** | `GPIO 5` | Chip Select (defined in code) |
| **CLK** | `GPIO 18` | VSPI SCK (Hardware SPI Clock) |

---

## Configuration

All configuration is managed in `include/secrets.h`.

1. **Create `secrets.h`**:
   Copy the example configuration file:
   ```bash
   cp include/secrets.example.h include/secrets.h
   ```

2. **Configure your settings in `include/secrets.h`**:
   ```cpp
   #pragma once

   // Wi-Fi Credentials
   const char* const WIFI_SSID = "YOUR_WIFI_SSID";
   const char* const WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

   // OpenWeatherMap API Configuration
   // Sign up for a free API key at https://openweathermap.org/
   const char* const OWM_API_KEY = "YOUR_OPENWEATHERMAP_API_KEY";
   const char* const WEATHER_CITY = "YourCity,ST";
   const char* const WEATHER_COUNTRY = "US";

   // Time Server & Format Configuration
   const char* const NTP_SERVER = "pool.ntp.org";
   const char* const TIMEZONE = "CST6CDT,M3.2.0,M11.1.0"; // POSIX timezone string with automatic DST rules
   const char* const TIME_FORMAT = "12";            // "12" for 12-hour or "24" for 24-hour format

   // Date Format Configuration ("MM-DD" or "DD-MM")
   const char* const DATE_FORMAT = "MM-DD";

   // Display and Update Timings
   const unsigned long DISPLAY_STAGE_SEC = 15;           // Seconds per display mode (time, temp, date)
   const unsigned long WEATHER_UPDATE_INTERVAL_MIN = 10; // Weather update interval in minutes (respects API limits)
   const int DISPLAY_BRIGHTNESS = 0;                     // LED matrix brightness (0-15)

   // Temperature Unit Configuration ("F" for Fahrenheit, "C" for Celsius)
   const char* const TEMP_UNIT = "F";
   ```

---

## Project Structure

```text
esp32-weather-clock/
├── include/
│   ├── displayFont.h       # Custom 8x8 font and animated glyph definitions
│   ├── secrets.example.h   # Template for credentials and configuration
│   └── secrets.h           # Local credentials (git-ignored)
├── src/
│   └── main.cpp            # Main application logic and display state machine
├── platformio.ini          # PlatformIO board and library configuration
└── README.md
```

---

## Build & Upload with PlatformIO

### Using VS Code / PlatformIO IDE:
1. Open this repository in VS Code (`File > Open Folder... > esp32-weather-clock`).
2. PlatformIO will automatically install required libraries (`MD_Parola`, `MD_MAX72XX`, `Arduino_JSON`).
3. Click the **Build** (✓) button on the bottom toolbar.
4. Connect your ESP32 via USB and click the **Upload** (→) button.
5. Open the **Serial Monitor** (plug icon) at 115200 baud to view status messages.

### Using PlatformIO CLI:
```bash
# Build firmware
pio run

# Upload to board
pio run --target upload

# Open Serial Monitor
pio device monitor -b 115200
```
