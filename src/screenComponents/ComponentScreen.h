#ifndef COMPONENT_SCREEN_H
#define COMPONENT_SCREEN_H

#include "ComponentBase.h"
#include "../DisplayManager.h"

class Screen: public ComponentBase {
    public:
        Screen(DisplayManager& display): ComponentBase(100, 100), display(display) {
            startX = 0;
            startY = 0;
            widthAbsolute  = display.getWidth();
            heightAbsolute = display.getHeight();
        };

        DisplayManager& getDisplay() override { return display; }

    private:
        DisplayManager& display;
};


#endif