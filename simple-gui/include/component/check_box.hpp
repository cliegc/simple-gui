#pragma once
#include "base_component.hpp"
#include "label.hpp"


namespace SimpleGui {
	class CheckButtonGroup final {

	};

	class CheckBox final : public BaseComponent {
	public:
		CheckBox(std::string_view text);
		~CheckBox() = default;

		virtual bool HandleEvent(Event* event) override;
		virtual void Update() override;
		virtual void Render(const Renderer& renderer) override;

	private:
		std::string m_text;

	};

	class ButtonAction final {

	};
}