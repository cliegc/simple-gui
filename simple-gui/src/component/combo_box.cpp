#include "component/combo_box.hpp"


namespace SimpleGui {
	ComboBox::ComboBox(const std::vector<std::string>& items) {
		m_currItemLbl = std::make_unique<Label>("");
		m_currItemLbl->CustomThemeColor(ThemeColorFlags::LabelBackground, Color::TRANSPARENT);
	}

	void ComboBox::EnteredComponentTree() {
	}

	bool ComboBox::HandleEvent(Event* event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		if (BaseComponent::HandleEvent(event)) return true;
		if (HandleToggleItemsList(event)) return true;
		if (HandleSelectItem(event)) return true;

		return false;
	}

	void ComboBox::Update() {
		SG_CMP_UPDATE_CONDITIONS;
		
		BaseComponent::Update();
		UpdateItemsList();
	}

	void ComboBox::Render(Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		// renderer.FillRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::ComboBoxBackground));
		renderer.RenderRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::ComboBoxBackground), true);

		m_currItemLbl->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::ComboBoxForeground));
		m_currItemLbl->Render(renderer);

		//draw toggle

		//draw items list
		RenderItemsList(renderer);

		renderer.SetRenderClipRect(m_visibleGRect);
		renderer.RenderRect(GetGlobalRect(), GetThemeColor(ThemeColorFlags::ComboBoxBorder), false);
		renderer.ClearRenderClipRect();

		BaseComponent::Render(renderer);
	}

	bool ComboBox::HandleToggleItemsList(Event* event) {
		return false;
	}

	bool ComboBox::HandleSelectItem(Event* event) {
		return false;
	}

	void ComboBox::UpdateItemsList() {
	}

	void ComboBox::RenderItemsList(Renderer& renderer) {
		if (!m_showItemsList) return;

		renderer.SetTopRender(true);
		renderer.RenderRect(m_itemsRect.visibleGRect, GetThemeColor(ThemeColorFlags::ComboBoxBackground), true);
		for (const auto& lbl : m_lbls) {
			lbl->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::ComboBoxForeground));
			lbl->Render(renderer);
		}

		//draw hovered

		//draw selected
		
		renderer.SetRenderClipRect(m_itemsRect.visibleGRect);
		renderer.RenderRect(m_itemsRect.gRect, GetThemeColor(ThemeColorFlags::ComboBoxBorder), false);
		renderer.ClearRenderClipRect();
		renderer.SetTopRender(false);
	}
}