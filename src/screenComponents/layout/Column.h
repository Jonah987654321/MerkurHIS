#ifndef COMPONENT_COLUMN_H
#define COMPONENT_COLUMN_H

#include "../ComponentBase.h"

class Column : public ComponentBase {
    public:
        Column(int wPct, int hPct, int spacing = 0)
            : ComponentBase(wPct, hPct, true), spacing(spacing) {}

    protected:
        void handleChildSize(double childW, double childH) override {
            nextOffsetY += childH + spacing;
        }

    private:
        int spacing;
};

#endif
