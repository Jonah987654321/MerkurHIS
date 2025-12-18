#include <time.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#include "src/DisplayManager.h"

// TFT Pins
#define TFT_CS   D8
#define TFT_DC   D4
#define TFT_RST  D3

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
DisplayManager display(tft);

void setup() {
  Serial.begin(115200);
  display.init();
}

void loop() {
  display.update();
}
