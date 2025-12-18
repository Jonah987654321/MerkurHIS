#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <string>

#include <Adafruit_ST7735.h>
#include "../secrets.h"

class DisplayManager {
  public:
    DisplayManager(Adafruit_ST7735 &display);
    void init();
    void update();

    double getWidth() { return tft.width(); }
    double getHeight() {return tft.height(); }

  private:
    Adafruit_ST7735 &tft;
    int status = 0;

    int currentMin = -1;

    int startupCursorHeight = 10;
    void startupLog(std::string text);
};

#endif
