#ifndef COMPONENT_BASE_H
#define COMPONENT_BASE_H

#include <vector>
#include <stdexcept>
#include <memory>

#include "../DisplayManager.h"

class ComponentBase {
    public:
        ComponentBase(int wPct, int hPct, bool allowChildren = true, double initialOffsetX = 0, double initialOffsetY = 0)
            : widthPct(wPct),
            heightPct(hPct),
            allowChildren(allowChildren),
            initialOffsetX(initialOffsetX),
            initialOffsetY(initialOffsetY)
        {
            if (wPct < 0 || wPct > 100 || hPct < 0 || hPct > 100) {
                // Illegal values so we hide the element
                widthPct = 0;
                heightPct = 0;
            }
        };

        virtual ~ComponentBase() = default;

        void setParent(ComponentBase* parent) {
            this->parent = parent;
        };

        template<typename T>
        void addChild(std::unique_ptr<T> child) {
            static_assert(std::is_base_of<ComponentBase, T>::value, "Child must derive from ComponentBase");
            if (!this->allowChildren) {
                return;
            }
            child->setParent(this);
            this->children.push_back(std::move(child));
        };

        void setColorBg(uint16_t color) { 
            this->hasColorBg = true;
            this->colorBg = color;
        }
        uint16_t getColorBg() { 
            if (hasColorBg) return colorBg;
            if (parent) return parent->getColorBg();
            return ST77XX_BLACK; // Default black
         }

        // Default render implementation: render self & then all children
        virtual void render() {
            if (!parent) {
                this->space = AssignedSpace(0, 0, getDisplay().getWidth(), getDisplay().getHeight());
            } else {
                this->space = this->parent->getAndReserveSpacing(widthPct, heightPct);
            }

            std::size_t currentHash = hash();
            if (!wasRendered || lastHash!=currentHash) {
                // Element dirty -> rerender & invalidate children
                cleanBackground();
                renderSelf();

                invalidate();

                lastHash = currentHash;
                wasRendered = true;
            }

            // Reset offsets
            this->nextOffsetX = this->initialOffsetX;
            this->nextOffsetY = this->initialOffsetY;

            // Render children
            for (auto& child : children) {
                child->render();
            }
        }

        void invalidate() {
            this->wasRendered = false;
            for (auto& child : children) {
                child->invalidate();
            }
        }

        // Public hash interface
        virtual std::size_t hash() const {
            return createBaseHash();
        }
        
        virtual AssignedSpace getAndReserveSpacing(int widthPct, int heightPct) { 
            double calculatedWidth = (widthPct/100.0)*this->space.width;
            double calculatedHeight = (heightPct/100.0)*this->space.height;
            AssignedSpace forChild(this->space.x+nextOffsetX, this->space.y+nextOffsetY, calculatedWidth, calculatedHeight);
            handleChildSize(calculatedWidth, calculatedHeight);
            return forChild;
        }

        virtual void handleChildSize(double x, double y) {
            this->nextOffsetY += y;
        }

        virtual DisplayManager& getDisplay() {
            // Any root component (screen) MUST override this!!
            return this->parent->getDisplay();
        }

    protected:
        int widthPct;
        int heightPct;

        AssignedSpace space;

        double initialOffsetX;
        double initialOffsetY;

        double nextOffsetX = 0;
        double nextOffsetY = 0;

        uint16_t colorBg;
        bool hasColorBg = false;
        
        bool allowChildren;

        ComponentBase* parent = nullptr;
        std::vector<std::unique_ptr<ComponentBase>> children;

        // Base hash calculation for width, height, and children
        bool wasRendered = false;
        std::size_t lastHash = 0;
        std::size_t createBaseHash() const {
            std::size_t h = 0;

            auto hash_combine = [&h](std::size_t v) {
                h ^= v + 0x9e3779b9 + (h << 6) + (h >> 2);
            };

            auto hash_double = [&](double v) {
                return static_cast<std::size_t>(v * 64.0); // fixed-point
            };

        
            hash_combine(std::hash<int>{}(widthPct));
            hash_combine(std::hash<int>{}(heightPct));
        
            hash_combine(hash_double(space.x));
            hash_combine(hash_double(space.y));
            hash_combine(hash_double(space.width));
            hash_combine(hash_double(space.height));

            hash_combine(std::hash<bool>{}(hasColorBg));
            if (hasColorBg) {
                hash_combine(std::hash<uint16_t>{}(colorBg));
            }
        
            return h;
        }


        virtual void renderSelf() {};
        void cleanBackground() {
            getDisplay().fillRect(space, getColorBg());
        };
};

#endif