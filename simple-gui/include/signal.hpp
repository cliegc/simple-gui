#pragma once
#include <string>
#include <unordered_map>
#include <functional>


namespace SimpleGui {
    template<typename... Args>
    class Signal final {
    public:
        using SlotType = std::function<void(Args...)>;

        bool IsConnect(const std::string& name) {
            return m_slots.find(name) != m_slots.end();
        }

        bool Connect(const std::string& name, const SlotType& slot) {
            if (IsConnect(name)) return false;

            m_slots[name] = slot;
            return true;
        }

        void Disconnect(const std::string& name) {
            m_slots.erase(name);
        }

        void Emit(Args... args) {
            for (auto& [_, slot] : m_slots) {
                slot(args...);
            }
        }

        void operator()(Args... args) {
            Emit(args...);
        }

    private:
        std::unordered_map<std::string, SlotType> m_slots;
    };
}