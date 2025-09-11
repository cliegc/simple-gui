#pragma once
#include "base_component.hpp"
#include "label.hpp"
#include "signal.hpp"



namespace SimpleGui {
	class Button final : public BaseComponent {
	public:
		explicit Button(std::string_view text);
		virtual ~Button() = default;

		std::string GetText() const;
		void SetText(std::string_view text);

		virtual void SetFont(std::unique_ptr<Font> font) override;
		virtual void SetFont(std::string_view path, int size) override;

		virtual bool HandleEvent(Event*) override;
		virtual void Update() override;
		virtual void Render(const Renderer&) override;

	public:
		Signal<> clicked;

	private:
		MouseState m_mouseState = MouseState::Normal;
		std::unique_ptr<Label> m_lbl;
	};
}