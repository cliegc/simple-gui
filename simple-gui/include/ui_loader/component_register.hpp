#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <any>
#include "component/base_component.hpp"


#define SG_CMP_REG SimpleGui::ComponentRegister::GetInstance()
#define SG_CMP_REG_PROPERTY_SETTER_ARGS BaseComponent* cmp, const std::vector<std::string>& args
#define SG_CMP_REG_REG_CLASS(clazz) SimpleGui::ComponentRegister::GetInstance().RegisterComponent(#clazz, []{ return std::make_unique<clazz>(); })
#define SG_CMP_REG_REG_PROPERTY(clazz, property, func) SimpleGui::ComponentRegister::GetInstance().RegisterComponentProperty(#clazz, property, func)
#define SG_CMP_REG_CREATE(clazz) SimpleGui::ComponentRegister::GetInstance().CreateComponent(#clazz)
#define SG_CMP_REG_SET_PROPERTY(clazz, property, cmp, ...) SimpleGui::ComponentRegister::GetInstance().SetComponentProperty(#clazz, property, cmp, __VA_ARGS__)
#define SG_CMP_REG_COPY_PROPERTY(clazz, target_clazz, property) SimpleGui::ComponentRegister::GetInstance().CopyComponentProperty(#clazz, #target_clazz, property)
#define SG_CMP_REG_COPY_PROPERTIES(clazz, target_clazz) SimpleGui::ComponentRegister::GetInstance().CopyComponentAllProperties(#clazz, #target_clazz)


namespace SimpleGui {
    class ComponentRegister final {
    public:
        using ComponentConstructor = std::function<std::unique_ptr<BaseComponent>()>;
        using ComponentPropertySetter = std::function<void(BaseComponent *, const std::vector<std::string> &)>;
        using ComponentPropertyInfo = std::unordered_map<std::string, ComponentPropertySetter>;

        ~ComponentRegister() = default;
        ComponentRegister(const ComponentRegister &) = delete;
        ComponentRegister &operator=(const ComponentRegister &) = delete;
        ComponentRegister(ComponentRegister &&) = delete;
        ComponentRegister &operator=(ComponentRegister &&) = delete;

        static ComponentRegister &GetInstance() {
            static ComponentRegister instance;
            return instance;
        }

        static void Init();

        void RegisterComponent(const std::string &className, const ComponentConstructor &constructor);
        void RegisterComponentProperty(const std::string &className, const std::string &property,
                                       const ComponentPropertySetter &func);
        void CopyComponentProperty(const std::string &name, const std::string &target, const std::string &property);
        void CopyComponentAllProperties(const std::string &name, const std::string &target);
        std::unique_ptr<BaseComponent> CreateComponent(const std::string &className);
        void SetComponentProperty(const std::string &className, const std::string &property, BaseComponent *cmp,
                                  const std::vector<std::string> &args);

    private:
        struct ComponentInfo final {
            ComponentConstructor constructor;
            ComponentPropertyInfo properties;
        };

        static bool s_initialized;
        std::unordered_map<std::string, ComponentInfo> m_infos{};

        ComponentRegister() = default;
    };
}
