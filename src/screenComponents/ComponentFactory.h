#pragma once

#include <memory>
#include <type_traits>
#include "ComponentBase.h"

template<typename T, typename... Args>
std::unique_ptr<T> newComponent(Args&&... args) {
    static_assert(std::is_base_of<ComponentBase, T>::value,
                  "T must derive from ComponentBase");
    return std::make_unique<T>(std::forward<Args>(args)...);
}
