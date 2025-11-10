#include "ui_loader/component_register.hpp"
#include "component/component.hpp"
#include "ui_loader/type_convert.hpp"


#define ANY_STR_TO_FLOAT(x) std::stof(std::any_cast<const char*>(x))
#define ANY_STR_TO_INT(x) std::stoi(std::any_cast<const char*>(x))


namespace SimpleGui {
    bool ComponentRegister::s_initialized = false;

    void ComponentRegister::RegisterComponent(const std::string &className, const ComponentConstructor &constructor) {
        if (m_infos.contains(className)) return;
        m_infos.emplace(className, std::move(ComponentInfo{}));
        m_infos[className].constructor = constructor;
    }

    void ComponentRegister::RegisterComponentProperty(const std::string &className, const std::string &property,
        const ComponentPropertySetter &func) {
        if (!m_infos.contains(className)) return;
        if (m_infos[className].properties.contains(property)) return;
        m_infos[className].properties.emplace(property, func);
    }

    void ComponentRegister::CopyComponentProperty(const std::string &name, const std::string &target,
        const std::string &property) {
        if (!m_infos.contains(name) || !m_infos.contains(target)) return;
        if (m_infos[name].properties.contains(property)) return;

        auto& targetInfo = m_infos[target];
        if (!targetInfo.properties.contains(property)) return;

        auto& info = m_infos[name];
        info.properties.emplace(property, targetInfo.properties[property]);
    }

    void ComponentRegister::CopyComponentAllProperties(const std::string &name, const std::string &target) {
        if (!m_infos.contains(name) || !m_infos.contains(target)) return;
        for (auto& [property, setter] : m_infos[target].properties) {
            if (m_infos[name].properties.contains(property)) continue;
            m_infos[name].properties.emplace(property, setter);
        }
    }

    std::unique_ptr<BaseComponent> ComponentRegister::CreateComponent(const std::string& className) {
        const auto it = m_infos.find(className);
        if (it == m_infos.end()) return nullptr;
        return it->second.constructor();
    }

    void ComponentRegister::SetComponentProperty(const std::string &className, const std::string &property,
        BaseComponent *cmp, const std::vector<std::string> &args) {
        const auto it = m_infos.find(className);
        if (it == m_infos.end()) return;

        if (auto &[_, properties] = it->second; properties.contains(property)) {
            properties[property](cmp, args);
        }
    }

    void ComponentRegister::Init() {
        if (s_initialized) return;

        // BaseComponent
        SG_CMP_REG_REG_CLASS(BaseComponent);
        SG_CMP_REG_REG_PROPERTY(BaseComponent, "position", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            cmp->SetPosition(std::stof(args[0]), std::stof(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(BaseComponent, "position-x", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            cmp->SetPositionX(std::stof(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(BaseComponent, "position-y", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            cmp->SetPositionY(std::stof(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(BaseComponent, "size", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            cmp->SetSize(std::stof(args[0]), std::stof(args[1]));
        });
        SG_CMP_REG_REG_PROPERTY(BaseComponent, "width", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            cmp->SetWidth(std::stof(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(BaseComponent, "height", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            cmp->SetHeight(std::stof(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(BaseComponent, "min-size", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            cmp->SetMinSize(std::stof(args[0]), std::stof(args[1]));
        });
        SG_CMP_REG_REG_PROPERTY(BaseComponent, "theme-color", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            cmp->CustomThemeColor(std::any_cast<ThemeColorFlags>(args[0]), std::any_cast<Color>(args[1]));
        });

        // Label
        SG_CMP_REG_REG_CLASS(Label);
        SG_CMP_REG_COPY_PROPERTIES(Label, BaseComponent);
        SG_CMP_REG_REG_PROPERTY(Label, "text", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<Label*>(cmp)->SetText(args[0]);
        });
        // TODO ...

        // Button
        SG_CMP_REG_REG_CLASS(Button);
        SG_CMP_REG_COPY_PROPERTIES(Button, BaseComponent);
        SG_CMP_REG_REG_PROPERTY(Button, "text", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<Button*>(cmp)->SetText(args[0]);
        });

        // CheckBox
        SG_CMP_REG_REG_CLASS(CheckBox);
        SG_CMP_REG_COPY_PROPERTIES(CheckBox, BaseComponent);
        SG_CMP_REG_REG_PROPERTY(CheckBox, "text", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<CheckBox*>(cmp)->SetText(args[0]);
        });
        SG_CMP_REG_REG_PROPERTY(CheckBox, "checked", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<CheckBox*>(cmp)->SetChecked(TypeConverter::ToBool(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(CheckBox, "group", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<CheckBox*>(cmp)->SetGroup(std::any_cast<std::shared_ptr<CheckBoxGroup>>(args[0]));
        });

        // Combobox
        SG_CMP_REG_REG_CLASS(ComboBox);
        SG_CMP_REG_COPY_PROPERTIES(ComboBox, BaseComponent);
        SG_CMP_REG_REG_PROPERTY(Combobox, "item", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ComboBox*>(cmp)->AddItem(std::any_cast<const char*>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(Combobox, "current-index", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ComboBox*>(cmp)->SetCurrentItem(std::any_cast<int>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(Combobox, "auto-hide-list", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ComboBox*>(cmp)->SetAutoHideItemsList(std::any_cast<bool>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(Combobox, "list-max-height", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ComboBox*>(cmp)->SetMaxItemsListHeight(std::any_cast<float>(args[0]));
        });

        // DraggablePanel
        SG_CMP_REG_REG_CLASS(DraggablePanel);
        SG_CMP_REG_COPY_PROPERTIES(DraggablePanel, BaseComponent);
        SG_CMP_REG_REG_PROPERTY(DraggablePanel, "title", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<DraggablePanel*>(cmp)->SetTitle(std::any_cast<const char*>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(DraggablePanel, "resizable", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<DraggablePanel*>(cmp)->SetResizable(std::any_cast<bool>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(DraggablePanel, "global-draggable", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<DraggablePanel*>(cmp)->SetGlobalDragEnable(std::any_cast<bool>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(DraggablePanel, "clamp-position", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<DraggablePanel*>(cmp)->SetClampRangeFollowParentContent(std::any_cast<bool>(args[0]));
        });

        // ProgressBar
        SG_CMP_REG_REG_CLASS(ProgressBar);
        SG_CMP_REG_COPY_PROPERTIES(ProgressBar, BaseComponent);
        SG_CMP_REG_REG_PROPERTY(ProgressBar, "min-value", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ProgressBar*>(cmp)->SetMinValue(std::any_cast<float>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(ProgressBar, "max-value", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ProgressBar*>(cmp)->SetMaxValue(std::any_cast<float>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(ProgressBar, "value", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ProgressBar*>(cmp)->SetValue(std::any_cast<float>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(ProgressBar, "fill-mode", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ProgressBar*>(cmp)->SetProgressFillMode(std::any_cast<ProgressFillMode>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(ProgressBar, "show-text", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ProgressBar*>(cmp)->ShowProgressText(std::any_cast<bool>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(ProgressBar, "indeterminate", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ProgressBar*>(cmp)->SetIndeterminate(std::any_cast<bool>(args[0]));
        });

        // ScrollPanel
        SG_CMP_REG_REG_CLASS(ScrollPanel);
        SG_CMP_REG_COPY_PROPERTIES(ScrollPanel, BaseComponent);
        SG_CMP_REG_REG_PROPERTY(ScrollPanel, "vscroll-visible", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ScrollPanel*>(cmp)->SetVScrollBarVisible(std::any_cast<bool>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(ScrollPanel, "hscroll-visible", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ScrollPanel*>(cmp)->SetHScrollBarVisible(std::any_cast<bool>(args[0]));
        });

        // ScrollBar
        SG_CMP_REG_REG_CLASS(ScrollBar);
        SG_CMP_REG_COPY_PROPERTIES(ScrollBar, BaseComponent);
        SG_CMP_REG_REG_PROPERTY(ScrollBar, "direction", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ScrollBar*>(cmp)->SetDirection(std::any_cast<Direction>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(ScrollBar, "target", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ScrollBar*>(cmp)->SetTarget(std::any_cast<BaseComponent*>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(ScrollBar, "wheel-delta", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<ScrollBar*>(cmp)->SetMouseWheelDelta(std::any_cast<float>(args[0]));
        });

        // Slider
        SG_CMP_REG_REG_CLASS(Slider);
        SG_CMP_REG_COPY_PROPERTIES(Slider, BaseComponent);
        SG_CMP_REG_REG_PROPERTY(Slider, "min-value", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<Slider*>(cmp)->SetMinValue(std::any_cast<float>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(Slider, "max-value", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<Slider*>(cmp)->SetMaxValue(std::any_cast<float>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(Slider, "value", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<Slider*>(cmp)->SetValue(std::any_cast<float>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(Slider, "direction", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<Slider*>(cmp)->SetDirection(std::any_cast<Direction>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(Slider, "editable", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<Slider*>(cmp)->SetEditable(std::any_cast<bool>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(Slider, "scrollable", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<Slider*>(cmp)->SetScrollable(std::any_cast<bool>(args[0]));
        });

        // TextureRect
        SG_CMP_REG_REG_CLASS(TextureRect);
        SG_CMP_REG_COPY_PROPERTIES(TextureRect, BaseComponent);
        SG_CMP_REG_REG_PROPERTY(TextureRect, "texture", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<TextureRect*>(cmp)->SetTexture(std::any_cast<const char*>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(TextureRect, "stretch-mode", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<TextureRect*>(cmp)->SetTextureStretchMode(std::any_cast<TextureStretchMode>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(TextureRect, "scale-mode", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<TextureRect*>(cmp)->SetScaleMode(std::any_cast<TextureScaleMode>(args[0]));
        });
        SG_CMP_REG_REG_PROPERTY(TextureRect, "flip-h", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<TextureRect*>(cmp)->SetFlipH(std::any_cast<bool>(args[0]));
        });
                SG_CMP_REG_REG_PROPERTY(TextureRect, "scale-mode", [](SG_CMP_REG_PROPERTY_SETTER_ARGS) {
            static_cast<TextureRect*>(cmp)->SetFlipV(std::any_cast<bool>(args[0]));
        });

        // ...

        s_initialized = true;
    }
}
