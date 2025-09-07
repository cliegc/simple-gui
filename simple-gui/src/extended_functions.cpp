#include "extended_functions.hpp"


namespace SimpleGui {
	int ExtendedFunctionsManager::s_counter = 0;

	ExtendedFunctionsManager::ExtendedFunctionsManager(BaseComponent* target): m_target(target) {
	}

	bool ExtendedFunctionsManager::HandleEvent(Event* event) {
		for (auto& item : m_functions) {
			if (item.second->HandleEvent(event)) return true;
		}
		return false;
	}

	void ExtendedFunctionsManager::Update() {
		for (auto& item : m_functions) {
			item.second->Update();
		}
	}

	void ExtendedFunctionsManager::Render(const Renderer& renderer) {
		for (auto& item : m_functions) {
			item.second->Render(renderer);
		}
	}

	void ExtendedFunctionsManager::Clear() {
		m_functions.clear();
	}
}