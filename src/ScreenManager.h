#pragma once

#include <vector>
#include <memory>
#include "screenComponents/ComponentScreen.h"

class ScreenManager {
    public:

        void registerNewScreen(std::unique_ptr<Screen> s) {
            this->screens.push_back(std::move(s));
        }

        void update() {
            this->screens.at(currentScreenIndex)->render();
        }

    private:
        std::vector<std::unique_ptr<Screen>> screens;
        int currentScreenIndex = 0;
};