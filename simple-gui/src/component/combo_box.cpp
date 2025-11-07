#include "component/combo_box.hpp"

#include "event.hpp"
#include "logger.hpp"


namespace SimpleGui {
	ComboBox::ComboBox(const std::vector<std::string>& items) {
		m_maxItemsListHeight = 120.f;

		m_masterItemLbl = std::make_unique<Label>("");
		m_masterItemLbl->CustomThemeColor(ThemeColorFlags::LabelBackground, Color::TRANSPARENT);
		m_masterItemLbl->SetSizeConfigs(ComponentSizeConfig::Expanding, ComponentSizeConfig::Expanding);

		m_itemsPanel = std::make_unique<ScrollPanel>();
		m_itemsPanel->SetVisible(false);
		m_itemsPanel->SetHeight(m_maxItemsListHeight);
		m_itemsPanel->CustomThemeColor(ThemeColorFlags::ScrollPanelBackground, GetThemeColor(ThemeColorFlags::ComboBoxBackground));
		m_itemsPanel->CustomThemeColor(ThemeColorFlags::ScrollPanelBorder, GetThemeColor(ThemeColorFlags::ComboBoxBorder));
		m_itemsPanel->CustomThemeColor(ThemeColorFlags::ScrollbarBorder_V, Color::TRANSPARENT);

		m_toggleRect.gRect.size.x = 15;
		SetSize(85, 25);
		SetMinWidth(25);

		m_items = items;
		if (!m_items.empty()) {
			m_masterItemLbl->SetText(m_items[0]);
			m_currIndex = 0;
		}
	}

	void ComboBox::EnteredComponentTree() {
		BaseComponent::EnteredComponentTree();
		SetComponentOwner(m_masterItemLbl.get(), m_window, this);
		BaseComponent::EnteredComponentTree(m_masterItemLbl.get());

		SetComponentOwner(m_itemsPanel.get(), m_window, &m_window->GetRootComponent());
		BaseComponent::EnteredComponentTree(m_itemsPanel.get());

		for (const auto& item : m_items) {
			AddItem(item, false);
		}
	}

	bool ComboBox::HandleEvent(Event* event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		if (BaseComponent::HandleEvent(event)) return true;
		if (HandleToggleItemsList(event)) return true;
		if (m_itemsPanel->HandleEvent(event)) return true;
		if (HandleSelectItem(event)) return true;

		return false;
	}

	void ComboBox::Update() {
		SG_CMP_UPDATE_CONDITIONS;
		
		BaseComponent::Update();

		m_masterItemLbl->Update();

		auto gRect = GetGlobalRect();
		m_toggleRect.gRect.position.x = gRect.Right() - m_toggleRect.gRect.size.w;
		m_toggleRect.gRect.position.y = gRect.Top();
		m_toggleRect.gRect.size.h = m_size.h;
		m_toggleRect.visibleGRect = CalcVisibleGlobalRect(m_visibleGRect, m_visibleGRect, m_toggleRect.gRect);

		m_itemsPanel->Update();
	}

	void ComboBox::Render(Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		renderer.RenderRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::ComboBoxBackground), true);

		m_masterItemLbl->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::ComboBoxForeground));
		m_masterItemLbl->Render(renderer);

		//draw toggle
		renderer.RenderRect(m_toggleRect.visibleGRect, GetThemeColor(ThemeColorFlags::ComboBoxToggleBackground), true);
		RenderToggleRect(renderer);

		//draw items list
		RenderItemsList(renderer);

		renderer.SetRenderClipRect(m_visibleGRect);
		renderer.RenderRect(GetGlobalRect(), GetThemeColor(ThemeColorFlags::ComboBoxBorder), false);
		renderer.ClearRenderClipRect();

		BaseComponent::Render(renderer);
	}

	void ComboBox::AddItem(const std::string &item, bool addToItems) {
		if (addToItems) m_items.push_back(item);

		const float y = m_itemLbls.empty() ? 0 : m_itemLbls.back()->GetRect().Bottom();
		const auto lbl = m_itemsPanel->AddChildDeferred<Label>(item);
		lbl->SetSizeConfigW(ComponentSizeConfig::Expanding);
		lbl->SetPosition(0, y);
		m_itemLbls.push_back(lbl);

		const float w = lbl->GetSize().w;
		const float width = m_maxItemsListWidth < w ? w + 20 : m_maxItemsListWidth;
		m_itemsPanel->SetWidth(width);

		auto height = y + lbl->GetSize().h;
		if (height <= m_maxItemsListHeight) {
			m_itemsPanel->SetHeight(height);
		}
	}

	bool ComboBox::HandleToggleItemsList(Event* event) {
		if (auto ev = event->Convert<MouseButtonEvent>();
			ev && ev->IsPressed(MouseButton::Left) &&
			(m_toggleRect.visibleGRect.ContainPoint(ev->GetPosition()) ||
			m_visibleGRect.ContainPoint(ev->GetPosition()))) {
			m_itemsPanel->SetVisible(!m_itemsPanel->IsVisible());
			if (m_itemsPanel->IsVisible()) {
				SetSafePositionForItemList();
				m_hoveringLbl = nullptr;
				m_lastHoveredLbl = nullptr;
			}
			return true;
		}
		return false;
	}

	bool ComboBox::HandleSelectItem(Event* event) {
		if (!m_itemsPanel->IsVisible()) return false;

		if (auto ev = event->Convert<MouseButtonEvent>();
			ev && ev->IsPressed(MouseButton::Left)) {
			if (!m_itemsPanel->GetGlobalRect().ContainPoint(ev->GetPosition())) {
				m_itemsPanel->SetVisible(false);
				return false;
			}

			for (int i = 0; i < m_itemLbls.size(); ++i) {
				if (!m_itemLbls[i]->GetGlobalRect().ContainPoint(ev->GetPosition())) continue;
				SetCurrentItem(i);
				if (m_autoHideItemsList) {
					m_itemsPanel->SetVisible(false);
				}
				break;
			}

			return true;
		}

		if (auto ev = event->Convert<MouseMotionEvent>()) {
			if (!m_itemsPanel->GetGlobalRect().ContainPoint(ev->GetPosition())) {
				m_lastHoveredLbl = m_hoveringLbl;
				m_hoveringLbl = nullptr;
				return false;
			}

			if (m_hoveringLbl && m_hoveringLbl->GetGlobalRect().ContainPoint(ev->GetPosition())) return false;

			int index = -1;
			for (const auto& lbl : m_itemLbls) {
				index++;
				if (!lbl->GetGlobalRect().ContainPoint(ev->GetPosition())) continue;
				if (lbl != m_hoveringLbl) m_lastHoveredLbl = m_hoveringLbl;
				m_hoveringLbl = lbl;
				return true;
			}
		}

		return false;
	}

	void ComboBox::RenderToggleRect(Renderer &renderer) {
		renderer.SetRenderClipRect(m_visibleGRect);
		Rect centerRect(
			m_toggleRect.gRect.position.x + (m_toggleRect.gRect.size.w - 10) / 2,
			m_toggleRect.gRect.position.y + (m_toggleRect.gRect.size.h - 10) / 2,
			10, 10
		);
		if (!m_itemsPanel->IsVisible()) {
			Vec2 bottomCenter(
					centerRect.position.x + centerRect.size.x / 2,
					centerRect.position.y + centerRect.size.y
				);
			renderer.RenderTriangle(
				centerRect.TopLeft(),
				centerRect.TopRight(),
				bottomCenter,
				GetThemeColor(ThemeColorFlags::ComboBoxForeground),
				true
			);
		}
		else {
			Vec2 topCenter(
				centerRect.position.x + centerRect.size.x / 2,
				centerRect.position.y
			);
			renderer.RenderTriangle(
				topCenter,
				centerRect.BottomRight(),
				centerRect.BottomLeft(),
				GetThemeColor(ThemeColorFlags::ComboBoxForeground),
				true
			);
		}
		renderer.ClearRenderClipRect();
	}

	void ComboBox::RenderItemsList(Renderer& renderer) {
		if (!m_itemsPanel->IsVisible()) return;

		renderer.SetTopRender(true);
		if (m_lastSelectedLbl) {
			m_lastSelectedLbl->CustomThemeColor(ThemeColorFlags::LabelBackground, Color::TRANSPARENT);
		}

		//draw hovered
		if (m_hoveringLbl) {
			m_hoveringLbl->CustomThemeColor(ThemeColorFlags::LabelBackground, GetThemeColor(ThemeColorFlags::ComboBoxItemHovered));
		}
		if (m_lastHoveredLbl) {
			m_lastHoveredLbl->CustomThemeColor(ThemeColorFlags::LabelBackground, Color::TRANSPARENT);
		}

		//draw selected
		if (m_selectedLbl) {
			m_selectedLbl->CustomThemeColor(ThemeColorFlags::LabelBackground, GetThemeColor(ThemeColorFlags::ComboBoxItemSelected));
		}

		m_itemsPanel->CustomThemeColor(ThemeColorFlags::ScrollPanelBackground, GetThemeColor(ThemeColorFlags::ComboBoxBackground));
		m_itemsPanel->CustomThemeColor(ThemeColorFlags::ScrollPanelBorder, GetThemeColor(ThemeColorFlags::ComboBoxBorder));
		m_itemsPanel->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::ComboBoxForeground));
		m_itemsPanel->Render(renderer);

		renderer.SetTopRender(false);
	}

	void ComboBox::SetSafePositionForItemList() const {
		const Rect rootContentGRect = m_window->GetRootComponent().GetContentGlobalRect();
		const Rect gRect = GetGlobalRect();
		Rect panelGRect = m_itemsPanel->GetGlobalRect();
		panelGRect.position.x = gRect.position.x;
		panelGRect.position.y = gRect.Bottom() + 3;

		if (panelGRect.Left() < rootContentGRect.Left()) panelGRect.position.x = rootContentGRect.Left();
		if (panelGRect.Right() > rootContentGRect.Right()) panelGRect.position.x = rootContentGRect.Right() - panelGRect.size.w;
		if (panelGRect.Bottom() > rootContentGRect.Bottom()) panelGRect.position.y = gRect.Top() - panelGRect.size.h - 3;

		m_itemsPanel->SetGlobalPosition(panelGRect.position);
	}

	std::string ComboBox::GetItem(size_t index) const {
		index = SDL_clamp(index, 0, m_items.size() - 1);
		return m_items[index];
	}

	std::vector<std::string> ComboBox::GetItems() const {
		return m_items;
	}

	size_t ComboBox::GetItemCount() const {
		return m_items.size();
	}

	void ComboBox::SetCurrentItem(size_t index) {
		index = SDL_clamp(index, 0, m_items.size() - 1);
		auto lbl = m_itemLbls[index];

		if (lbl == m_selectedLbl) return;
		m_lastSelectedLbl = m_selectedLbl;
		m_selectedLbl = lbl;
		m_masterItemLbl->SetText(lbl->GetText());
		m_currIndex = index;

		currentItemChanged.Emit(m_currIndex, m_masterItemLbl->GetText());
	}

	void ComboBox::SetCurrentItem(const std::string& item) {
		const auto it =  std::ranges::find(m_items, item);
		if (it == m_items.end()) return;
		SetCurrentItem(it - m_items.begin());
	}

	void ComboBox::AddItem(const std::string& item) {
		AddItem(item, true);
	}

	void ComboBox::AddItems(const std::vector<std::string> &items) {
		for (const auto& item : items) {
			AddItem(item);
		}
	}

	// TODO: fix bug
	void ComboBox::RemoveItem(size_t index) {
		if (index >= m_items.size()) return;
		if (m_items.empty() || m_itemLbls.empty()) return;

		const auto lbl = m_itemLbls[index];

		if (lbl == m_lastSelectedLbl) m_lastSelectedLbl = nullptr;

		m_items.erase(m_items.begin() + index);
		m_itemLbls.erase(m_itemLbls.begin() + index);
		m_itemsPanel->RemoveChildDeferred(lbl);


		if (m_itemLbls.empty()) {
			m_masterItemLbl->SetText("");
			m_selectedLbl = nullptr;
			m_lastSelectedLbl = nullptr;
			m_currIndex = 0;
		}
		else {
			if (index == m_currIndex) {
				if (m_currIndex == m_items.size()) {
					SetCurrentItem(m_currIndex - 1);
				}
				else {
					SetCurrentItem(m_currIndex);
				}
				m_lastSelectedLbl = nullptr;
			}
			else if (index < m_currIndex) {
				m_currIndex--;
			}
		}

		// update item lbl position
		float startY = 0;
		for (const auto& label : m_itemLbls) {
			label->SetPositionY(startY);
			startY += label->GetSize().h;
		}
		if (startY < m_maxItemsListHeight) {
			m_itemLbls.empty() ? m_itemsPanel->SetHeight(m_maxItemsListHeight) : m_itemsPanel->SetHeight(startY);
		}
	}

	void ComboBox::RemoveItem(const std::string &item) {
		const auto it =  std::ranges::find(m_items, item);
		if (it == m_items.end()) return;
		RemoveItem(it - m_items.begin());
	}

	void ComboBox::ClearItems() {
		m_items.clear();
		m_itemLbls.clear();
		m_itemsPanel->ClearAllChildrenDeferred();

		m_lastHoveredLbl = nullptr;
		m_hoveringLbl = nullptr;
		m_lastSelectedLbl = nullptr;
		m_selectedLbl = nullptr;
		m_currIndex = 0;
		m_masterItemLbl->SetText("");
		m_itemsPanel->SetHeight(m_maxItemsListHeight);
	}
}
