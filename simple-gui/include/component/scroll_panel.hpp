#pragma once
#include "scrollbar.hpp"


namespace SimpleGui {
	class ScrollPanel final : public BaseComponent {
	public:
		ScrollPanel();
		~ScrollPanel() = default;

		inline void SetHScrollBarVisible(bool visible) const { m_hScrollBar->SetVisible(visible); }
		inline void SetVScrollBarVisible(bool visible) const { m_vScrollBar->SetVisible(visible); }

		virtual bool HandleEvent(Event* event) override;
		virtual void Update() override;
		virtual void Render(const Renderer& renderer) override;

	protected:
		virtual void EnteredComponentTree() override;
		inline virtual Vec2 GetContentSize() const override;

	private:
		const int SCROLL_BAR_THICKNESS = 15;

		std::unique_ptr<ScrollBar> m_hScrollBar;
		std::unique_ptr<ScrollBar> m_vScrollBar;
		bool m_shiftPressed;
	};
}