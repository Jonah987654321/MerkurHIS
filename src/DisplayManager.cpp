#include "DisplayManager.h"
#include <ESP8266WiFi.h>
#include <Fonts/FreeSansBold12pt7b.h>

DisplayManager::DisplayManager(Adafruit_ST7735 &display) : tft(display) {}

void DisplayManager::init() {
    //Initialize tft display:
    this->tft.initR(INITR_BLACKTAB);
    this->tft.fillScreen(ST77XX_BLACK);
    this->tft.setTextColor(ST77XX_WHITE);
    this->tft.setTextSize(1);
    this->tft.setCursor(10, 30);
    this->tft.setRotation(2);
    this->startupLog("Starting MerkurHIS");

    // Connect to WiFi
    this->startupLog("Connecting to WiFi...");
    this->startupLog(std::string("SSID: ") + WLAN_SSID);
    WiFi.begin(WLAN_SSID, WLAN_PWD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      yield(); // Feed WDT
    }
    this->startupLog("WiFi connected!");

    // COnfigure ntp as middle european with automatic summertime
    configTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org", "time.nist.gov");
    this->startupLog("Retrieving current time...");

    // Wait until time is set
    time_t now = time(nullptr);
    while (now < 100000) {
        delay(500);
        now = time(nullptr);
        yield();
    }
}

void DisplayManager::update() {
  time_t now = time(nullptr);
  struct tm *timeinfo = localtime(&now);

  if (timeinfo->tm_min != currentMin) {
    // HH:MM korrekt als C-String
    char buf[6];
    snprintf(buf, sizeof(buf), "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);

    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setFont(&FreeSansBold12pt7b);

    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);

    int16_t y = (tft.height() - h) / 2 - y1;
    int16_t x = (tft.width() - w) / 2 - x1;

    tft.setCursor(x, y);
    tft.print(buf);

    currentMin = timeinfo->tm_min;
  }
  yield();
}

void DisplayManager::startupLog(std::string text) {
  const char* toPrint = text.c_str();
  this->tft.setCursor(5, startupCursorHeight);

  // Calculate height for the next log
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(toPrint, 0, 0, &x1, &y1, &w, &h);
  
  this->startupCursorHeight += h+5;
  this->tft.print(toPrint);
}

void DisplayManager::fillRect(int x, int y, int w, int h, uint16_t color) {
  tft.fillRect(x, y, w, h, color);
}

void DisplayManager::drawText(
  int x,
  int y,
  const char* text,
  uint16_t color,
  uint8_t size
) {
  tft.setTextSize(size);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(text);
}