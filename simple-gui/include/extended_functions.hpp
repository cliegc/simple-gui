#pragma once
#include "component/base_component.hpp"


namespace SimpleGui {
	class Event;

	class ExtendedFunctions {
	protected:
		friend class BaseComponent;

		virtual void HandleEvent(Event* event) {};
		virtual void Update() {};
		virtual void Render(const Renderer& renderer) {};

		BaseComponent* m_target;
	};
}