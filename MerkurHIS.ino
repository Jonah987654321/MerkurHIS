#include <time.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <Fonts/FreeSansBold12pt7b.h>

#include "src/DisplayManager.h"

// TFT Pins
#define TFT_CS   D8
#define TFT_DC   D4
#define TFT_RST  D3

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Timing
int currentMin = -1;

void setup() {
  Serial.begin(115200);

  DisplayManager display(tft);
  display.init();
}

void loop() {
  display.update();
}
