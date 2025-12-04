#include "ui_loader/ui_node_registry.hpp"

#include "ui_loader/type_convert.hpp"


namespace SimpleGui::UILoader {
    bool UINodeRegistry::s_initialized = false;

    Node::Node(std::string name, Kind kind, bool selfClosing) {
        m_name = name;
        m_kind = kind;
        m_selfClosing = selfClosing;
    }

    void UINodeRegistry::RegisterPropertyGetter(const std::string &name, const std::string &property,
        const Node::ObjectPropertyGetter &getter) {
        if (!m_nodes.contains(name)) return;

        auto& node = m_nodes.at(name);
        if (!node.m_properties.contains(property)) {
            node.m_properties.emplace(property, Node::PropertyFunction{});
        }

        auto& funcs = node.m_properties.at(property);
        if (funcs.getter != nullptr) return;
       funcs.getter = getter;
    }

    void UINodeRegistry::RegisterPropertySetter(const std::string &name, const std::string &property,
        const Node::ObjectPropertySetter &setter) {
        if (!m_nodes.contains(name)) return;

        auto& node = m_nodes.at(name);
        if (!node.m_properties.contains(property)) {
            node.m_properties.emplace(property, Node::PropertyFunction{});
        }

        auto& funcs = node.m_properties.at(property);
        if (funcs.setter != nullptr) return;
        funcs.setter = setter;
    }

    void UINodeRegistry::CopyRegisteredProperty(const std::string &name, const std::string &target,
        const std::string &property) {
        if (!m_nodes.contains(name) || !m_nodes.contains(target)) return;
        if (m_nodes.at(name).m_properties.contains(property) || !m_nodes.at(target).m_properties.contains(property)) return;

        auto& node = m_nodes.at(name);
        auto& targetNode = m_nodes.at(target);
        node.m_properties.emplace(property, targetNode.m_properties.at(property));
    }

    void UINodeRegistry::CopyAllRegisteredProperty(const std::string &name, const std::string &target) {
        if (!m_nodes.contains(name) || !m_nodes.contains(target)) return;
        for (const auto& [property, func] : m_nodes.at(target).m_properties) {
            if (m_nodes.at(name).m_properties.contains(property)) continue;
            m_nodes.at(name).m_properties.emplace(property, func);
        }
    }

    const Node* UINodeRegistry::GetNode(const std::string &name) const {
        if (!m_nodes.contains(name)) return nullptr;
        return &m_nodes.at(name);
    }

    void UINodeRegistry::Init() {
        if (s_initialized) return;

        SG_UILOADER_REGISTER_CMP_NODE(BaseComponent);
        SG_UILOADER_REGISTER_NODE_PROPERTY_SETTER(BaseComponent, "position", [](SG_UILOADER_NODE_PROPERTY_SETTER_ARGS) {
            static_cast<BaseComponent*>(cmp)->SetPosition(TypeConverter::ToVec2(args[0]));
        });
        SG_UILOADER_REGISTER_NODE_PROPERTY_SETTER(BaseComponent, "position-x", [](SG_UILOADER_NODE_PROPERTY_SETTER_ARGS) {
            static_cast<BaseComponent*>(cmp)->SetPositionX(TypeConverter::ToFloat(args[0]));
        });
        SG_UILOADER_REGISTER_NODE_PROPERTY_SETTER(BaseComponent, "position-x", [](SG_UILOADER_NODE_PROPERTY_SETTER_ARGS) {
            static_cast<BaseComponent*>(cmp)->SetPositionY(TypeConverter::ToFloat(args[0]));
        });
        SG_UILOADER_REGISTER_NODE_PROPERTY_SETTER(BaseComponent, "size", [](SG_UILOADER_NODE_PROPERTY_SETTER_ARGS) {
            static_cast<BaseComponent*>(cmp)->SetSize(TypeConverter::ToVec2(args[0]));
        });
        SG_UILOADER_REGISTER_NODE_PROPERTY_SETTER(BaseComponent, "width", [](SG_UILOADER_NODE_PROPERTY_SETTER_ARGS) {
            static_cast<BaseComponent*>(cmp)->SetWidth(TypeConverter::ToFloat(args[0]));
        });
        SG_UILOADER_REGISTER_NODE_PROPERTY_SETTER(BaseComponent, "height", [](SG_UILOADER_NODE_PROPERTY_SETTER_ARGS) {
            static_cast<BaseComponent*>(cmp)->SetHeight(TypeConverter::ToFloat(args[0]));
        });
        SG_UILOADER_REGISTER_NODE_PROPERTY_SETTER(BaseComponent, "min-size", [](SG_UILOADER_NODE_PROPERTY_SETTER_ARGS) {
            static_cast<BaseComponent*>(cmp)->SetMinSize(TypeConverter::ToVec2(args[0]));
        });
        SG_UILOADER_REGISTER_NODE_PROPERTY_SETTER(BaseComponent, "theme-color", [](SG_UILOADER_NODE_PROPERTY_SETTER_ARGS) {
            static_cast<BaseComponent*>(cmp)->CustomThemeColor(TypeConverter::ToThemeColorFlags(args[0]), TypeConverter::ToColor(args[1]));
        });

        SG_UILOADER_REGISTER_CMP_NODE(Button);
        SG_UILOADER_COPY_REGISTERED_PROPERTIES(Button, BaseComponent);
        SG_UILOADER_REGISTER_NODE_PROPERTY_SETTER(Button, "text", [](SG_UILOADER_NODE_PROPERTY_SETTER_ARGS) {
            static_cast<Button*>(cmp)->SetText(args[0]);
        });

        SG_UILOADER_REGISTER_CMP_NODE(CheckBox);
        SG_UILOADER_COPY_REGISTERED_PROPERTIES(CheckBox, BaseComponent);
        SG_UILOADER_REGISTER_NODE_PROPERTY_SETTER(CheckBox, "text", [](SG_UILOADER_NODE_PROPERTY_SETTER_ARGS) {
            static_cast<CheckBox*>(cmp)->SetText(args[0]);
        });
        SG_UILOADER_REGISTER_NODE_PROPERTY_SETTER(CheckBox, "checked", [](SG_UILOADER_NODE_PROPERTY_SETTER_ARGS) {
            static_cast<CheckBox*>(cmp)->SetChecked(TypeConverter::ToBool(args[0]));
        });
        SG_UILOADER_REGISTER_NODE_PROPERTY_SETTER(CheckBox, "group", [](SG_UILOADER_NODE_PROPERTY_SETTER_ARGS) {
            static_cast<CheckBox*>(cmp)->SetGroup(std::any_cast<std::shared_ptr<CheckBoxGroup>>(args[0]));
        });

        s_initialized = true;
    }
}
