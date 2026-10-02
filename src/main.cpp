#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <time.h>
#include "displayFont.h"

#if __has_include("secrets.h")
#include "secrets.h"
#else
#include "secrets.example.h"
#endif

// Hardware configuration
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW  // Screen type FC16_HW
#define CS_PIN 5                           // Chip Select pin
#define MAX_DEVICES 4                      // Number of linked 8x8 matrices

// Derived timings (DISPLAY_STAGE_SEC and WEATHER_UPDATE_INTERVAL_MIN defined in secrets.h)
const unsigned long WEATHER_UPDATE_INTERVAL_MS = WEATHER_UPDATE_INTERVAL_MIN * 60 * 1000UL;

MD_Parola ledMatrix = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

unsigned long lastWeatherCheck = 0;
double curTemp = 0.0;

// Forward declarations
String httpGETRequest(const char* serverName);
void updateWeather();

void setup() {
  Serial.begin(115200);

  ledMatrix.begin();
  ledMatrix.setIntensity(DISPLAY_BRIGHTNESS);
  ledMatrix.setFont(DISPLAYFONT);
  ledMatrix.setTextAlignment(PA_CENTER);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    ledMatrix.print("WIFI.  ");
    delay(500);
    ledMatrix.print("WIFI.. ");
    delay(500);
    ledMatrix.print("WIFI...");
    delay(500);
  }
  ledMatrix.print("WIFI OK");
  delay(500);

  configTime(0, 0, NTP_SERVER);
  setenv("TZ", TIMEZONE, 1);
  tzset();
}

void loop() {
  // Periodically fetch weather data
  if (lastWeatherCheck == 0 || (millis() - lastWeatherCheck) >= WEATHER_UPDATE_INTERVAL_MS) {
    updateWeather();
    lastWeatherCheck = millis();
  }

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    ledMatrix.print("--:--");
    delay(500);
    return;
  }

  // Calculate current stage in the 3-stage display cycle (Time -> Temp -> Date)
  const unsigned long stageDurationMs = DISPLAY_STAGE_SEC * 1000UL;
  const unsigned long totalCycleMs = stageDurationMs * 3;
  unsigned long cycleProgress = millis() % totalCycleMs;

  if (cycleProgress < stageDurationMs) {
    // Stage 1: Current Time with blinking colon and animated indicator glyph (12h or 24h)
    bool is12Hour = String(TIME_FORMAT).startsWith("12");
    switch (timeinfo.tm_sec % 4) {
      case 0: ledMatrix.print(&timeinfo, is12Hour ? "%I:%M A" : "%H:%M A"); break;
      case 1: ledMatrix.print(&timeinfo, is12Hour ? "%I %M B" : "%H %M B"); break;
      case 2: ledMatrix.print(&timeinfo, is12Hour ? "%I:%M C" : "%H:%M C"); break;
      case 3: ledMatrix.print(&timeinfo, is12Hour ? "%I %M D" : "%H %M D"); break;
    }
  } else if (cycleProgress < stageDurationMs * 2) {
    // Stage 2: Temperature (configurable: °F or °C)
    bool isCelsius = String(TEMP_UNIT).equalsIgnoreCase("C") || String(TEMP_UNIT).equalsIgnoreCase("metric");
    String tempText = String(static_cast<int>(round(curTemp))) + (isCelsius ? "°C" : "°F");
    ledMatrix.print(tempText);
  } else {
    // Stage 3: Date (configurable: MM-DD or DD-MM)
    if (String(DATE_FORMAT).equalsIgnoreCase("MM-DD") || String(DATE_FORMAT) == "%m-%d") {
      ledMatrix.print(&timeinfo, "%m-%d");
    } else {
      ledMatrix.print(&timeinfo, "%d-%m");
    }
  }
}

void updateWeather() {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  bool isCelsius = String(TEMP_UNIT).equalsIgnoreCase("C") || String(TEMP_UNIT).equalsIgnoreCase("metric");
  String unitsParam = isCelsius ? "metric" : "imperial";
  String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" +
                      String(WEATHER_CITY) + "," + String(WEATHER_COUNTRY) +
                      "&APPID=" + String(OWM_API_KEY) + "&units=" + unitsParam;

  String jsonBuffer = httpGETRequest(serverPath.c_str());
  JSONVar myObject = JSON.parse(jsonBuffer);

  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Parsing input failed!");
  } else if (myObject.hasOwnProperty("main") && myObject["main"].hasOwnProperty("temp")) {
    curTemp = double(myObject["main"]["temp"]);
  } else {
    Serial.print("Weather error response: ");
    Serial.println(jsonBuffer);
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
  int httpResponseCode = http.GET();
  String payload = "{}";

  if (httpResponseCode > 0) {
    payload = http.getString();
  }
  http.end();
  return payload;
}
