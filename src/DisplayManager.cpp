#include "DisplayManager.h"
#include <ESP8266WiFi.h>

DisplayManager::DisplayManager(Adafruit_ST7735 &display) : tft(display) {}

void DisplayManager::init() {
    //Initialize tft display:
    this->tft.initR(INITR_BLACKTAB);
    this->tft.fillScreen(ST77XX_BLACK);
    this->tft.setTextColor(ST77XX_WHITE);
    this->tft.setTextSize(1);
    this->tft.setCursor(10, 30);
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