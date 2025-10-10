#pragma once
#include "base_component.hpp"
#include "label.hpp"
#include "signal.hpp"



namespace SimpleGui {
	class Button  : public BaseComponent {
	public:
		explicit Button(std::string_view text);
		~Button() override = default;

		std::string GetText() const;
		void SetText(std::string_view text);

		void SetFont(std::unique_ptr<Font> font) override;
		void SetFont(std::string_view path, int size) override;

		bool HandleEvent(Event*) override;
		void Update() override;
		void Render(Renderer&) override;

	public:
		Signal<> clicked;

	protected:
		void EnteredComponentTree() override;

	private:
		MouseState m_mouseState = MouseState::Normal;
		std::unique_ptr<Label> m_lbl;
	};
}