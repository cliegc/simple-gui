#pragma once
#include <type_traits>


namespace SimpleGui::refl {
    namespace detail {
        template<typename T>
        struct variable_type {
            using type = T;
        };

        template<typename Class, typename T>
        struct variable_type<T Class::*> {
            using type = T;
        };
    }


    template<typename T>
    using variable_type_t = typename detail::variable_type<T>::type;

    namespace detail {
        template<typename T>
        auto variable_pointer_to_type(long, T *) -> T;

        template<typename Class, typename T>
        auto variable_pointer_to_type(long, T Class::*) -> T;
    }

    template<auto T>
    using variable_pointer_to_type_t = decltype(detail::variable_pointer_to_type(0, T));

    namespace internal {
        template<typename T>
        struct basic_variable_traits {
            using type = variable_type_t<T>;
            static constexpr bool is_member = std::is_member_pointer_v<T>;
        };
    }

    template<typename T>
    struct variable_traits;

    template<typename T>
    struct variable_traits<T*>: internal::basic_variable_traits<variable_type_t<T>> {
        using pointer = T*;
    };

    template<typename Class, typename T>
    struct variable_traits<T Class::*>: internal::basic_variable_traits<T Class::*> {
        using pointer = T Class::*;
        using clazz = Class;
    };

    namespace detail {
        template<auto T>
        struct variable_pointer_traits: variable_traits<decltype(T)> {};
    }

    template<auto T>
    using variable_pointer_traits = detail::variable_pointer_traits<T>;
}
