#include <time.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#include "src/DisplayManager.h"
#include "src/ScreenManager.h"

#include "src/screenComponents/layout/Column.h"
#include "src/screenComponents/content/Label.h"
#include "src/screenComponents/ComponentFactory.h"

// TFT Pins
#define TFT_CS   D8
#define TFT_DC   D4
#define TFT_RST  D3

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
DisplayManager display(tft);
ScreenManager screens = ScreenManager();

void setup() {
  Serial.begin(115200);
  display.init();

  // Inital screen
  std::unique_ptr<Screen> example = newComponent<Screen>(display);
  std::unique_ptr<Column> col = newComponent<Column>(100, 33);
  col->setColorBg(ST7735_BLUE);
  example->addChild(std::move(col));

  std::unique_ptr<Column> col2 = newComponent<Column>(100, 33);
  col2->setColorBg(ST7735_RED);
  example->addChild(std::move(col2));

  std::unique_ptr<Column> col3 = newComponent<Column>(100, 33);
  col3->setColorBg(ST7735_YELLOW);
  col3->addChild(newComponent<Label>(100, 100, "Test"));
  example->addChild(std::move(col3));

  screens.registerNewScreen(std::move(example));
}

void loop() {
  screens.update();

  /*
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
  */
}
