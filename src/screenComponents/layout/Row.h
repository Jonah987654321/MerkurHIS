#ifndef COMPONENT_ROW_H
#define COMPONENT_ROW_H

#include "../ComponentBase.h"

class Row : public ComponentBase {
    public:
        Row(int wPct, int hPct, int spacing = 0)
            : ComponentBase(wPct, hPct, true), spacing(spacing) {}

    protected:
        void handleChildSize(double childW, double childH) override {
            nextOffsetX += childW + spacing;
        }

    private:
        int spacing;
};

#endif
