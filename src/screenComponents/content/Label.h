#ifndef COMPONENT_LABEL_H
#define COMPONENT_LABEL_H

#include "../ComponentBase.h"
#include <string>

class Label : public ComponentBase {
    public:
        Label(int wPct, int hPct,
              std::string text,
              Align align = Align())
            : ComponentBase(wPct, hPct, false),
              text(std::move(text)),
              align(align) {}

        void setText(const std::string& t) {
            if (t != text) {
                text = t;
            }
        }

        void setAlign(Align a) {
            if (a != align) {
                align = a;
            }
        }

    protected:
        void renderSelf() override {
            getDisplay().drawText(space, align, text.c_str(), ST7735_WHITE);
        }

        std::size_t hash() const override {
            std::size_t h = ComponentBase::hash();
            h ^= std::hash<std::string>{}(text) << 1;
            h ^= AlignHash{}(align) << 1;
            return h;
        }

    private:
        std::string text;
        Align align;
};

#endif
