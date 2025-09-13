#include "component/label.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	Label::Label(std::string_view text) : BaseComponent() {
		//Init(text);
		m_text = text;
	}

	void Label::EnteredComponentTree() {
		//Init()
		auto ttf_text = TTF_CreateText(&m_window->GetTTFTextEngine(), &GetFont().GetTTFFont(), m_text.c_str(), m_text.size());
		m_ttfText = UniqueTextPtr(ttf_text);

		m_padding = m_window->GetCurrentStyle().componentPadding;

		AdjustSize(m_ttfText.get());

		m_textAlignments.first = TextAlignment::Left;
		m_textAlignments.second = TextAlignment::Top;
	}

	void Label::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		UpdateTextAlignments();

		BaseComponent::Update();
	}

	void Label::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		renderer.SetClipRect(m_visibleGRect);
		renderer.FillRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::LabelBackgound));
		renderer.DrawText(m_ttfText.get(), m_textRect.position, GetThemeColor(ThemeColorFlags::LabelForeground));
		renderer.DrawRect(GetGlobalRect(), GetThemeColor(ThemeColorFlags::LabelBorder));
		renderer.ClearClipRect();
		BaseComponent::Render(renderer);
	}

	void Label::Init(std::string_view text) {
		/*auto ttf_text = TTF_CreateText(&SG_GuiManager.GetTextEngine(), &GetFont().GetTTFFont(), text.data(), text.size());
		m_ttfText = UniqueTextPtr(ttf_text);

		m_padding = SG_GuiManager.GetCurrentStyle().componentPadding;

		AdjustSize(m_ttfText.get());

		m_textAlignments.first = TextAlignment::Left;
		m_textAlignments.second = TextAlignment::Top;*/
	}

	void Label::AdjustSize(TTF_Text* ttfText) {
		if (!ttfText) return;

		int w, h;
		TTF_GetTextSize(ttfText, &w, &h);
		w += m_padding.left + m_padding.right;
		h += m_padding.top + m_padding.bottom;

		if (m_size.w < w) m_size.w = w;
		if (m_size.h < h) m_size.h = h;

		SetMinSize(m_size);
	}

	void Label::UpdateTextAlignments() {
		int w, h;
		TTF_GetTextSize(m_ttfText.get(), &w, &h);
		m_textRect.size.w = w;
		m_textRect.size.h = h;

		SetMinSize(w + m_padding.left + m_padding.right, h + m_padding.top + m_padding.bottom);

		Rect globalRect = GetGlobalRect();

		if (m_textAlignments.first == TextAlignment::Left) {
			m_textRect.position.x = globalRect.position.x + m_padding.left;
		}
		else if (m_textAlignments.first == TextAlignment::Right) {
			m_textRect.position.x = globalRect.size.w - m_padding.right - m_textRect.size.w + globalRect.position.x;
		}
		else if (m_textAlignments.first == TextAlignment::Center) {
			m_textRect.position.x = (globalRect.size.w - m_textRect.size.w) / 2 + globalRect.position.x;
		}

		if (m_textAlignments.second == TextAlignment::Top) {
			m_textRect.position.y = globalRect.position.y + m_padding.top;
		}
		else if (m_textAlignments.second == TextAlignment::Bottom) {
			m_textRect.position.y = globalRect.size.h - m_padding.bottom - m_textRect.size.h + globalRect.position.y;
		}
		else if (m_textAlignments.second == TextAlignment::Center) {
			m_textRect.position.y = (globalRect.size.h - m_textRect.size.h) / 2 + globalRect.position.y;
		}

		TTF_SetTextWrapWidth(m_ttfText.get(), globalRect.size.w - m_padding.left - m_padding.right);
	}

	std::string Label::GetText() const {
		return m_text;
	}

	void Label::SetText(std::string_view text) {
		m_text = text;
		TTF_SetTextString(m_ttfText.get(), text.data(), text.size());
		AdjustSize(m_ttfText.get());
	}

	TextAlignments Label::GetTextAlignments() const {
		return m_textAlignments;
	}

	void Label::SetTextAlignments(const TextAlignments& alignments) {
		m_textAlignments = alignments;
	}

	void Label::SetTextAlignments(TextAlignment alignmentH, TextAlignment alignmentV) {
		m_textAlignments.first = alignmentH;
		m_textAlignments.second = alignmentV;
	}

	void Label::SetTextAlignmentH(TextAlignment alignmentH) {
		m_textAlignments.first = alignmentH;
	}

	void Label::SetTextAlignmentV(TextAlignment alignmentV) {
		m_textAlignments.second = alignmentV;
	}

	TTF_Direction Label::GetTextDirection() const {
		return TTF_GetTextDirection(m_ttfText.get());
	}

	void Label::SetTextDirection(TTF_Direction direction) {
		TTF_SetTextDirection(m_ttfText.get(), direction);
	}
}