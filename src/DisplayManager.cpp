#include "DisplayManager.h"
#include <ESP8266WiFi.h>

DisplayManager::DisplayManager(Adafruit_ST7735 &display) : tft(display) {}

void DisplayManager::init() {
    //Initialize tft display:
    this->tft.initR(INITR_BLACKTAB);
    this->tft.setRotation(1);
    this->tft.fillScreen(ST77XX_BLACK);
    this->tft.setTextColor(ST77XX_WHITE);
    this->tft.setTextSize(1);
    this->tft.setCursor(10, 30);
    this->tft.print("Starting MerkurHIS...\n");

    // WLAN verbinden
    this->tft.print("Connecting to WiFi: ");
    this->tft.print(WLAN_SSID);
    this->tft.print(WLAN_PWD);
    WiFi.begin(WLAN_SSID, WLAN_PWD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      yield(); // WDT füttern
    }
    tft.print("WLAN verbunden!");

    // NTP konfigurieren: Mitteleuropäische Zeit, Sommerzeit automatisch
    configTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org", "time.nist.gov");
    tft.print("Hole Zeit...");

    // Warten bis Zeit gesetzt
    time_t now = time(nullptr);
    while (now < 100000) {
        delay(500);
        now = time(nullptr);
        yield();
    }
}

void DisplayManager::update() {
}

void DisplayManager::startupLog()