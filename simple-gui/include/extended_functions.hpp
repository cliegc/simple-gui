#pragma once
#include <memory>
#include <unordered_map>


namespace SimpleGui {
	class Event;
	class Renderer;
	class BaseComponent;

	class ExtendedFunctions {
	protected:
		friend class ExtendedFunctionsManager;

		virtual bool HandleEvent(Event* event) { return false; };
		virtual void Update() {};
		virtual void Render(const Renderer& renderer) {};

		BaseComponent* m_target = nullptr;
	};

	class ExtendedFunctionsManager final {
		friend class BaseComponent;
	public:
		ExtendedFunctionsManager(BaseComponent* target);
		~ExtendedFunctionsManager() = default;

		ExtendedFunctionsManager(const ExtendedFunctionsManager&) = delete;
		ExtendedFunctionsManager& operator=(const ExtendedFunctionsManager&) = delete;
		ExtendedFunctionsManager(ExtendedFunctionsManager&&) = delete;
		ExtendedFunctionsManager& operator=(ExtendedFunctionsManager&&) = delete;

	private:
		static int s_counter;
		BaseComponent* m_target;
		std::unordered_map<int, std::unique_ptr<ExtendedFunctions>> m_functions;

	private:
		bool HandleEvent(Event* event);
		void Update();
		void Render(const Renderer& renderer);

		void Clear();
		// 未实现
		void ClearDeferred();

		template<typename T>
		static int GetID() {
			static int id = s_counter++;
			return id;
		}

		template<typename T, typename ...Args>
		T* AddExtendedFunctions(Args&& ...args) {
			static_assert(std::is_base_of<ExtendedFunctions, T>::value, "T 必须继承自 ExtendedFunctions");
			if (m_functions.contains(GetID<T>())) return (T*)m_functions[GetID<T>()].get();
			auto functions = std::make_unique<T>(std::forward<Args>(args)...);
			auto ptr = functions.get();
			functions->m_target = m_target;
			m_functions.emplace(GetID<T>(), std::move(functions));
			return ptr;
		}

		// 未实现
		template<typename T, typename ...Args>
		T* AddExtendedFunctionsDeferred(Args&& ...args) {

		}

		template<typename T>
		std::unique_ptr<ExtendedFunctions> RemoveExtendedFunctions() {
			static_assert(std::is_base_of<ExtendedFunctions, T>::value, "T 必须继承自 ExtendedFunctions");
			auto it = m_functions.find(GetID<T>());
			if (it != m_functions.end()) {
				auto functions = std::move((*it).second);
				functions->m_target = nullptr;
				m_functions.erase(it);
				return std::move(functions);
			}

			return nullptr;
		}

		// 未实现
		template<typename T>
		std::unique_ptr<ExtendedFunctions> RemoveExtendedFunctionsDeferred() {

		}
	};
}