#pragma once
#include "scrollbar.hpp"


namespace SimpleGui {
	class ScrollPanel final : public BaseComponent {
	public:
		ScrollPanel();
		~ScrollPanel() override = default;

		void SetHScrollBarVisible(bool visible) const { m_hScrollBar->SetVisible(visible); }
		void SetVScrollBarVisible(bool visible) const { m_vScrollBar->SetVisible(visible); }

		bool HandleEvent(Event* event) override;
		void Update() override;
		void Render(Renderer& renderer) override;

	protected:
		void EnteredComponentTree() override;
		inline Vec2 GetContentSize() const override;

	private:
		const int SCROLL_BAR_THICKNESS = 15;

		std::unique_ptr<ScrollBar> m_hScrollBar;
		std::unique_ptr<ScrollBar> m_vScrollBar;
		bool m_shiftPressed;
	};
}