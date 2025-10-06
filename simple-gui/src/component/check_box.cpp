#include "component/check_box.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	void CheckBoxGroup::SetUniqueCheck(bool value) {
		m_uniqueCheck = value;
		if (!m_uniqueCheck) return;

		for (auto box : m_boxes) {
			if (!box->IsChecked()) continue;
			box->SetChecked(false);
			checkStateChanged.Emit(box);
		}
	}


	CheckBox::CheckBox(std::string_view text) {
		m_checked = false;
		m_textLbl = std::make_unique<Label>(text);

		m_checkRect.gRect.size.w = 15;
		m_checkRect.gRect.size.h = 15;

		m_textLbl->SetTextAlignments(TextAlignment::Left, TextAlignment::Center);
	}

	void CheckBox::EnteredComponentTree() {
		SetComponentOwner(m_textLbl.get(), m_window, this);
		BaseComponent::EnteredComponentTree(m_textLbl.get());
		SetSize(
			m_checkRect.gRect.size.w + m_textLbl->GetSize().w,
			m_textLbl->GetSize().h
		);
	}

	bool CheckBox::HandleEvent(Event* event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		if (BaseComponent::HandleEvent(event)) return true;
		if (HandleMouseClicked(event)) return true;

		return false;
	}

	void CheckBox::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		BaseComponent::Update();

		Rect contentGRect = GetContentGlobalRect();
		//update check rect
		m_checkRect.gRect.position.x = contentGRect.Left();
		m_checkRect.gRect.position.y = (contentGRect.size.h - m_checkRect.gRect.size.h) / 2 + contentGRect.Top();
		m_checkedFlagRect.gRect = m_checkRect.gRect.Grow(-3, -3, -3, -3);

		m_checkRect.visibleGRect = CalcVisibleGlobalRect(m_visibleGRect, contentGRect, m_checkRect.gRect);
		m_checkedFlagRect.visibleGRect = CalcVisibleGlobalRect(m_visibleGRect, contentGRect, m_checkedFlagRect.gRect);

		//update text label
		m_textLbl->SetPositionX(m_checkRect.gRect.size.w);
		m_textLbl->SetPositionY((contentGRect.size.h - m_textLbl->GetSize().h) / 2);
		m_textLbl->Update();
	}

	void CheckBox::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		renderer.FillRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::CheckBoxBackground));

		//draw check rect
		renderer.FillRect(m_checkRect.visibleGRect, GetThemeColor(ThemeColorFlags::CheckBoxCheckRectBackground));
		if (m_checked) {
			renderer.FillRect(m_checkedFlagRect.visibleGRect, GetThemeColor(ThemeColorFlags::CheckBoxCheckedFlag));
		}
		renderer.SetClipRect(m_checkRect.visibleGRect);
		renderer.DrawRect(m_checkRect.gRect, GetThemeColor(ThemeColorFlags::CheckBoxCheckRectBorder));
		renderer.ClearClipRect();

		//draw text label
		m_textLbl->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::CheckBoxForeground));
		m_textLbl->Render(renderer);

		BaseComponent::Render(renderer);

		//debuf
		//renderer.DrawRect(GetGlobalRect(), Color::GREEN);
	}

	bool CheckBox::HandleMouseClicked(Event* event) {
		if (!m_visibleGRect.ContainPoint(SG_GuiManager.GetMousePosition())) return false;
		if (auto ev = event->Convert<MouseButtonEvent>();
			ev && ev->IsPressed(MouseButton::Left)) {
			m_checked = !m_checked;
			checkStateChanged.Emit(m_checked);
			if (m_group) {
				m_group->checkStateChanged.Emit(this);

				if (m_group->m_uniqueCheck && m_checked) {
					for (auto box : m_group->m_boxes) {
						if (box == this) continue;
						if (box->IsChecked()) {
							box->SetChecked(false);
							m_group->checkStateChanged.Emit(box);
						}
					}
				}
			}
			return true;
		}
		return false;
	}

	inline void CheckBox::SetChecked(bool value) {
		if (m_checked == value) return;
		m_checked = value;
		checkStateChanged.Emit(m_checked);
	}

	void CheckBox::SetGoup(std::shared_ptr<CheckBoxGroup> group) {
		if (group == m_group) return;

		if (m_group) {
			auto it = std::find(m_group->m_boxes.begin(), m_group->m_boxes.end(), this);
			if (it != m_group->m_boxes.end()) {
				m_group->m_boxes.erase(it);
			}
		}
		m_group = group;
		m_group->m_boxes.push_back(this);
	}


}