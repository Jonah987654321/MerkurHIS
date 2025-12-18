#ifndef COMPONENT_BASE_H
#define COMPONENT_BASE_H

#include <vector>
#include <stdexcept>
#include <memory>

class ComponentBase {
    public:
        ComponentBase(int wPct, int hPct, bool allowChildren = true)
            : widthPct(wPct),
            heightPct(hPct),
            allowChildren(allowChildren)
        {
            if (wPct < 0 || wPct > 100 || hPct < 0 || hPct > 100) {
                throw std::invalid_argument("Width and Height for component must be between 0 and 100");
            }
        };

        virtual ~ComponentBase() = default;

        void setParent(ComponentBase* parent) {
            this->parent = parent;
        };

        void addChildren(std::unique_ptr<ComponentBase> child) {
            if (!this->allowChildren) {
                throw std::logic_error(
                    std::string("Adding children not supported on component type ")
                    + typeid(*this).name()
                );
            }
            child->setParent(this);

            double childXReturn = child->setStartX(this->startX+this->nextOffsetX);
            double childYReturn = child->setStartY(this->startY+this->nextOffsetY);
            this->handleChildSize(childXReturn, childYReturn);

            this->children.push_back(std::move(child));
        };

        virtual double getHeight() {
            if (!this->parent) return this->heightAbsolute; // Root
            if (this->heightCached) return this->heightAbsolute;
            this->heightAbsolute = (this->heightPct/100.0)*this->parent->getHeight();
            this->heightCached = true;
            return this->heightAbsolute;
        }

        virtual double getWidth() {
            if (!this->parent) return this->widthAbsolute;  // Root
            if (this->widthCached) return this->widthAbsolute;
            this->widthAbsolute = (this->widthPct/100.0)*this->parent->getWidth();
            this->widthCached = true;
            return this->widthAbsolute;
        }

        uint16_t getBaseColor() {
            return hasBaseColor?baseColor:parent->getBaseColor();
        }

        // Default render implementation: render self & then all children
        virtual void render(bool force = false) {
            bool childUnconditional = false;
            if (this->needRerender() || force) {
                childUnconditional = true;
                cleanBackground();
                renderSelf();
            }
            for (auto& child : children) {
                if (child->needRerender() || childUnconditional) {
                    child->render(true);
                }
            }
            lastHash = hash();
            wasRendered = true;
        }

        // Public hash interface
        virtual std::size_t hash() const {
            return createBaseHash();
        }

        bool needRerender() {
            return !wasRendered || lastHash!=hash();
        }

        double setStartX(double x) { 
            this->startX = x;
            return getWidth();
        }
        double setStartY(double y) {
            this->startY = y;
            return getHeight();
        }

        virtual void handleChildSize(double x, double y) {
            this->nextOffsetY += y;
        }

    protected:
        int widthPct;
        int heightPct;

        double startX;
        double startY;

        double nextOffsetX = 0;
        double nextOffsetY = 0;

        bool heightCached = false;
        bool widthCached = false;
        double heightAbsolute;
        double widthAbsolute;

        uint16_t baseColor;
        bool hasBaseColor;
        
        bool allowChildren;

        ComponentBase* parent = nullptr;
        std::vector<std::unique_ptr<ComponentBase>> children;

        // Base hash calculation for width, height, and children
        bool wasRendered = false;
        std::size_t lastHash = 0;
        std::size_t createBaseHash() const {
            std::size_t h = std::hash<int>{}(widthPct);
            h ^= std::hash<int>{}(heightPct) << 1;
            h ^= std::hash<double>{}(startX) << 1;
            h ^= std::hash<double>{}(startY) << 1;

            return h;
        }

        virtual void renderSelf() {};
        void cleanBackground() {
            
        };
};

#endif