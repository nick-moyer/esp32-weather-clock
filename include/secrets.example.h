#pragma once

// Copy this file to "secrets.h" in the same directory and fill in your values.

// Wi-Fi Credentials
const char* const WIFI_SSID = "YOUR_WIFI_SSID";
const char* const WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// OpenWeatherMap API Configuration
// Sign up for a free API key at https://openweathermap.org/
const char* const OWM_API_KEY = "YOUR_OPENWEATHERMAP_API_KEY";
const char* const WEATHER_CITY = "YourCity,ST";                 // location (more info on this in https://openweathermap.org/current)
const char* const WEATHER_COUNTRY = "US";                       // country code (ISO 3166 alpha-2)

// Time Server Configuration
const char* const NTP_SERVER = "pool.ntp.org";                 // time server
const char* const TIMEZONE = "CST6CDT,M3.2.0,M11.1.0";         // US Central Time with auto-DST
const char* const TIME_FORMAT = "12";                          // "12" for 12-hour or "24" for 24-hour format

// Date Format Configuration ("MM-DD" or "DD-MM")
const char* const DATE_FORMAT = "MM-DD";

// Display and Update Timings
const unsigned long DISPLAY_STAGE_SEC = 15;           // Seconds per display mode (time, temp, date)
const unsigned long WEATHER_UPDATE_INTERVAL_MIN = 10; // Weather update interval in minutes
const int DISPLAY_BRIGHTNESS = 1;                     // LED matrix brightness (0-15)

// Temperature Unit Configuration ("F" for Fahrenheit, "C" for Celsius)
const char* const TEMP_UNIT = "F";
