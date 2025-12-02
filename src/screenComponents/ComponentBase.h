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
            allowChildren(allowChildren),
            parent(nullptr) 
        {
            if (wPct < 0 || wPct > 100 || hPct < 0 || hPct > 100) {
                throw std::invalid_argument("Width and Height for component must be between 0 and 100");
            }

            this->widthPct = wPct;
            this->heightPct = hPct;
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
            this->children.push_back(child);
        }
        virtual void render() = 0;
    private:
        int widthPct;
        int heightPct;
        bool allowChildren;

        ComponentBase* parent;
        std::vector<std::unique_ptr<ComponentBase>> children;
};

#endif