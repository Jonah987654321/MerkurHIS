#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <string>

#include <Adafruit_ST7735.h>
#include "../secrets.h"

class AssignedSpace {
    public:
        AssignedSpace(): x(0), y(0), width(0), height(0) {};
        AssignedSpace(double x, double y, double width, double height)
            : x(x), y(y), width(width), height(height) {};
        double x,y,width,height;
};

enum class VerticalAlign {
  Top,
  Middle,
  Bottom
};

enum class HorizontalAlign {
  Left,
  Center,
  Right
};


class Align {
  public:
    Align(): vertical(VerticalAlign::Top), horizontal(HorizontalAlign::Left) {};
    Align(VerticalAlign v, HorizontalAlign h): vertical(v), horizontal(h) {};
    VerticalAlign vertical;
    HorizontalAlign horizontal;

    bool operator==(const Align& other) const {
        return vertical == other.vertical && horizontal == other.horizontal;
    }
    bool operator!=(const Align& other) const {
      return !(*this == other);
    }
};
struct AlignHash {
    std::size_t operator()(const Align& a) const {
        std::size_t h = 0;
        auto hash_combine = [&h](std::size_t v) {
            h ^= v + 0x9e3779b9 + (h << 6) + (h >> 2);
        };

        hash_combine(std::hash<int>{}(static_cast<int>(a.vertical)));
        hash_combine(std::hash<int>{}(static_cast<int>(a.horizontal)));

        return h;
    }
};

class DisplayManager {
  public:
    DisplayManager(Adafruit_ST7735 &display);
    void init();

    double getWidth() { return tft.width(); }
    double getHeight() {return tft.height(); }

    void fillRect(AssignedSpace space, uint16_t color);
    void drawText(AssignedSpace space, Align align, const char* text, uint16_t color, uint8_t size = 1);


  private:
    Adafruit_ST7735 &tft;
    int status = 0;

    int currentMin = -1;

    int startupCursorHeight = 10;
    void startupLog(std::string text);
};

#endif
