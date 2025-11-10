#pragma once
#include <string>
#include <memory>
#include <tinyxml2.h>
#include "component_register.hpp"


#define SG_UILoader


namespace SimpleGui {
    class BaseComponent;

    class UiFileParser final {
    public:
        UiFileParser();
        ~UiFileParser() = default;

        void Parse(const std::string& filePath);
        std::unique_ptr<BaseComponent> GetComponent();

    private:
        std::string GetAttribute(tinyxml2::XMLElement* element, const std::string& name, const std::string& defaultValue = "") {
            const char* value = element->Attribute(name.c_str());
            return value ? std::string(value) : defaultValue;
        }
    };
}