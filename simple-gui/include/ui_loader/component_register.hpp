#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <any>
#include <map>

#include "component/base_component.hpp"


namespace SimpleGui {
    class BaseComponent;

    class ComponentRegister final {
    public:
        ComponentRegister();
        ~ComponentRegister() = default;

        ComponentRegister(const ComponentRegister&) = delete;
        ComponentRegister& operator=(const ComponentRegister&) = delete;
        ComponentRegister(ComponentRegister&&) = delete;
        ComponentRegister& operator=(ComponentRegister&&) = delete;

        using FunctionType = std::function<std::any(const std::vector<std::any>&)>;
        std::map<std::string, FunctionType> functions;
    };
}

