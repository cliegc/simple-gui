#pragma once
#include <ranges>
#include <string>
#include <unordered_map>
#include <list>
#include <type_traits>


#define SG_TYPE_REGISTRY(T) SimpleGui::refl::Registry::GetRegistry<T>()
#define SG_TYPE_REGISTRY_TYPE_INFO(T) SimpleGui::refl::Registry::GetRegistry<T>().GetInfo()
#define SG_TYPE_REGISTRY_TYPE_INFO_STR(T_STR) SimpleGui::refl::Registry::GetTypeInfo(T_STR)
#define SG_NUMERIC_REGISTRY(T) SimpleGui::refl::NumericRegistry<T>::Instance()
#define SG_NUMERIC_REGISTRY_TYPE_INFO(T) SimpleGui::refl::NumericRegistry<T>::Instance().GetInfo()
#define SG_ENUM_REGISTRY(T) SimpleGui::refl::EnumRegistry<T>::Instance()
#define SG_ENUM_REGISTRY_TYPE_INFO(T) SimpleGui::refl::EnumRegistry<T>::Instance().GetInfo()
#define SG_CLASS_REGISTRY(T)


namespace SimpleGui::refl {
    class Type;

    enum class TypeKind {
        Type,
        Numeric,
        Enum,
        Class,
    };

    namespace internal {
        static std::list<const Type*> s_infos;
    }

#define SG_REFL_GET_TYPE_KIND(kind) public: constexpr virtual TypeKind GetTypeKind() const override { return (kind); } \
                                                        static constexpr TypeKind GetStaticTypeKind() { return kind; }

    class Type {
        template<typename T>
        friend class EnumRegistry;

    public:
        explicit Type(const std::string& typeName): m_typeName(typeName) {}
        virtual ~Type() = default;

        constexpr virtual TypeKind GetTypeKind() const { return TypeKind::Type; }
        static constexpr TypeKind GetStaticTypeKind() { return TypeKind::Type; }

        auto& GetName() const { return m_typeName; }

        template<typename T>
        const T* Convert() const {
            static_assert(std::is_base_of_v<Type, T>, "T 的基类必须是Type");
            if (GetTypeKind() == T::GetStaticTypeKind()) {
                return static_cast<const T*>(this);
            }
            return nullptr;
        }

    private:
        std::string m_typeName;
    };

    class Numeric final : public Type {
    public:
        SG_REFL_GET_TYPE_KIND(TypeKind::Numeric);

        enum class Kind {
            Char,
            Short,
            Int,
            Long,
            LongLong,
            Float,
            Double,
            Unknown,
        };

        explicit Numeric(Kind kind, bool isSigned): Type(GetTypeName(kind)), m_kind(kind), m_isSigned(isSigned) {}

        template<typename T>
        static Numeric Create() {
            return Numeric(DetectKind<T>(), std::is_signed_v<T>);
        }

        Kind GetKind() const { return m_kind; }
        bool IsSigned() const { return m_isSigned; }

    private:
        Kind m_kind;
        bool m_isSigned;

        static std::string GetTypeName(Kind kind) {
            switch (kind) {
                case Kind::Char: return "Char";
                case Kind::Short: return "Short";
                case Kind::Int: return "Int";
                case Kind::Long: return "Long";
                case Kind::LongLong: return "LongLong";
                case Kind::Float: return "Float";
                case Kind::Double: return "Double";
                default: return "Unknown";
            }
        }

        template<typename T>
        static Kind DetectKind() {
            if constexpr (std::is_same_v<T, char>) return Kind::Char;
            else if constexpr (std::is_same_v<T, short>) return Kind::Short;
            else if constexpr (std::is_same_v<T, int>) return Kind::Int;
            else if constexpr (std::is_same_v<T, long>) return Kind::Long;
            else if constexpr (std::is_same_v<T, long long>) return Kind::LongLong;
            else if constexpr (std::is_same_v<T, float>) return Kind::Float;
            else if constexpr (std::is_same_v<T, double>) return Kind::Double;
            else return Kind::Unknown;
        }
    };

    class Enum final : public Type {
    public:
        SG_REFL_GET_TYPE_KIND(TypeKind::Enum);

        using ValueType = long;

        Enum(): Type("Unknown-Enum") {}
        explicit Enum(const std::string& typeName): Type(typeName) {}

        template<typename VT>
        void Add(const std::string& name, VT value) {
            m_items.emplace(name, static_cast<ValueType>(value));
        }

        auto& GetItems() const { return m_items; }

    private:
        std::unordered_map<std::string, ValueType> m_items;
    };

    struct MemberVariable final {
        std::string name;
        const Type* type;
    };

    struct MemberFunction final {
        std::string name;
        const Type* returnType;
        std::vector<const Type*> parameterTypes;
    };

    class Class final : public Type {
    public:
        SG_REFL_GET_TYPE_KIND(TypeKind::Class);

        explicit Class(const std::string& typeName): Type(typeName) {}

        void AddMemberVar(MemberVariable&& var) {
            m_memberVariables.emplace_back(std::move(var));
        }

        void AddMemberFunction(MemberFunction&& func) {
            m_memberFunctions.emplace_back(std::move(func));
            // std::type_tr
        }

        auto& GetMemberVariables() const { return m_memberVariables; }
        auto& GetMemberFunctions() const { return m_memberFunctions; }

    private:
        std::vector<MemberVariable> m_memberVariables;
        std::vector<MemberFunction> m_memberFunctions;
    };

    template<typename T>
    class NumericRegistry final {
    public:
        static NumericRegistry& Instance() {
            static NumericRegistry reg(Numeric::Create<T>());
            static bool isSaved = false;
            if (!isSaved) {
                internal::s_infos.push_back(&reg.GetInfo());
                isSaved = true;
            }
            return reg;
        }

        auto& GetInfo() const { return m_info; }

    private:
        Numeric m_info;

        explicit NumericRegistry(Numeric&& info): m_info(std::move(info)) {}
    };

    template<typename T>
    class EnumRegistry final {
    public:
        static EnumRegistry& Instance() {
            static EnumRegistry reg;
            static bool isSaved = false;
            if (!isSaved) {
                internal::s_infos.push_back(&reg.GetInfo());
                isSaved = true;
            }
            return reg;
        }

        auto& GetInfo() const { return m_info; }

        EnumRegistry& Register(const std::string& name) {
            m_info.m_typeName = name;
            return *this;
        }

        template<typename VT>
        EnumRegistry& Add(const std::string& name, VT value) {
            m_info.Add(name, value);
            return *this;
        }

    private:
        Enum m_info;
    };

    class TrivialRegistry final {
    public:
        static TrivialRegistry& Instance() {
            static TrivialRegistry reg;
            return reg;
        }
    };

    class Registry final {
    public:
        template<typename T>
        static auto& GetRegistry() {
            if constexpr (std::is_fundamental_v<T>) return NumericRegistry<T>::Instance();
            else if constexpr (std::is_enum_v<T>) return EnumRegistry<T>::Instance();
            else return TrivialRegistry::Instance();
        }

        static const Type* GetTypeInfo(const std::string& typeName) {
            for (const auto& info : internal::s_infos) {
                if (info->GetName() == typeName) return info;
            }
            return nullptr;
        }
    };
}