#pragma once
#include "base_component.hpp"


namespace SimpleGui {
	class CheckBox final : public BaseComponent {
	public:
		CheckBox();
		~CheckBox() = default;

		virtual bool HandleEvent(Event* event) override;
		virtual void Update() override;
		virtual void Render(const Renderer& renderer) override;

	private:

	};
}