#include "component/progress_bar.hpp"
#include <format>
#include "gui_manager.hpp"


namespace SimpleGui {
	ProgressBar::ProgressBar(float value, float minValue, float maxValue): Range(value, minValue, maxValue) {
		m_progressLbl = std::make_unique<Label>("");

		SetMinValue(minValue);
		SetMaxValue(maxValue);
		SetValue(value);

		m_indeterminateProgressData.delta = 0.f;
		m_indeterminateProgressData.speed = 200.f;
		m_indeterminateProgressData.minLength = 5.f;
		m_indeterminateProgressData.lengthScale = 0.25f;

		m_progressLbl->SetTextAlignments(TextAlignment::Center, TextAlignment::Center);
	}

	void ProgressBar::EnteredComponentTree() {
		SetComponentOwner(m_progressLbl.get(), m_window, this);
		BaseComponent::EnteredComponentTree(m_progressLbl.get());
	}

	void ProgressBar::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		BaseComponent::Update();

		//update progress global rect
		Rect contentGRect = GetContentGlobalRect();
		if (!m_indeterminate) UpdateProgressGlobalRect(contentGRect);
		else UpdateIndeterminateProgressGlobalRect(contentGRect);

		// update progress label
		if (m_showProgress) {
			m_progressLbl->Update();
			m_progressLbl->SetSize(m_size);
			m_progressLbl->SetPosition(
				(m_size.w - m_progressLbl->GetSize().w) / 2,
				(m_size.h - m_progressLbl->GetSize().h) / 2);
			SetComponentVisibleGlobalRect(m_progressLbl.get(), m_progressLbl->GetGlobalRect());
		}
	}

	void ProgressBar::Render(Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		renderer.RenderRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::ProgressBarSlot), true);
		
		renderer.SetRenderClipRect(m_visibleGRect);
		renderer.RenderRect(m_currProgressGRect, GetThemeColor(ThemeColorFlags::ProgressBarProgress), true);
		renderer.RenderRect(GetGlobalRect(), GetThemeColor(ThemeColorFlags::ProgressBarBorder), false);
		renderer.ClearRenderClipRect();

		// draw current progress 
		if (m_showProgress) {
			m_progressLbl->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::ProgressBarForeground));
			m_progressLbl->Render(renderer);
		}

		BaseComponent::Render(renderer);
	}

	void ProgressBar::UpdateProgressGlobalRect(const Rect& contentGRect) {
		float scale = GetValueToMinValueInterval() / GetInterval();
		
		// update rect follow fill mode
		switch (m_fillMode) {
		case SimpleGui::ProgressFillMode::LeftToRight: {
			m_currProgressGRect.size.w = scale * contentGRect.size.w;
			m_currProgressGRect.size.h = contentGRect.size.h;
			m_currProgressGRect.position = contentGRect.position;
			break;
		}
		case SimpleGui::ProgressFillMode::RightToLeft: {
			m_currProgressGRect.size.w = scale * contentGRect.size.w;
			m_currProgressGRect.size.h = contentGRect.size.h;
			m_currProgressGRect.position.x = contentGRect.Right() - m_currProgressGRect.size.w;
			m_currProgressGRect.position.y = contentGRect.position.y;
			break;
		}
		case SimpleGui::ProgressFillMode::BottomToTop: {
			m_currProgressGRect.size.w = contentGRect.size.w;
			m_currProgressGRect.size.h = scale * contentGRect.size.h;
			m_currProgressGRect.position.x = contentGRect.position.x;
			m_currProgressGRect.position.y = contentGRect.Bottom() - m_currProgressGRect.size.h;
			break;
		}
		case SimpleGui::ProgressFillMode::TopToBottom: {
			m_currProgressGRect.size.w = contentGRect.size.w;
			m_currProgressGRect.size.h = scale * contentGRect.size.h;
			m_currProgressGRect.position = contentGRect.position;
			break;
		}
		case SimpleGui::ProgressFillMode::CenterH: {
			m_currProgressGRect.size.w = scale * contentGRect.size.w;
			m_currProgressGRect.size.h = contentGRect.size.h;
			m_currProgressGRect.position.x = (contentGRect.size.w - m_currProgressGRect.size.w) / 2 + contentGRect.Left();
			m_currProgressGRect.position.y = contentGRect.position.y;
			break;
		}
		case SimpleGui::ProgressFillMode::CenterV: {
			m_currProgressGRect.size.w = contentGRect.size.w;
			m_currProgressGRect.size.h = scale * contentGRect.size.h;
			m_currProgressGRect.position.x = contentGRect.position.x;
			m_currProgressGRect.position.y = (contentGRect.size.h - m_currProgressGRect.size.h) / 2 + contentGRect.Top();
			break;
		}
		}
	}

	void ProgressBar::UpdateIndeterminateProgressGlobalRect(const Rect& contentGRect) {
		m_indeterminateProgressData.delta += SG_GuiManager.GetDelta() * m_indeterminateProgressData.speed;

		switch (m_fillMode) {
		case ProgressFillMode::LeftToRight:
		case ProgressFillMode::RightToLeft:
		case ProgressFillMode::CenterH: {
			float length = m_indeterminateProgressData.lengthScale * contentGRect.size.w;
			length = SDL_clamp(length, m_indeterminateProgressData.minLength, contentGRect.size.w);

			m_currProgressGRect.size.w = length;
			m_currProgressGRect.size.h = contentGRect.size.h;
			m_currProgressGRect.position.y = contentGRect.position.y;

			if (m_fillMode == ProgressFillMode::LeftToRight) {
				m_currProgressGRect.position.x = m_indeterminateProgressData.delta + contentGRect.Left() - length;
				if (m_currProgressGRect.Left() > contentGRect.Right()) {
					m_currProgressGRect.position.x = contentGRect.Left() - length;
					m_indeterminateProgressData.delta = 0.f;
				}
			}
			else {
				m_currProgressGRect.position.x = contentGRect.Right() - m_indeterminateProgressData.delta;
				if (m_currProgressGRect.Right() < contentGRect.Left()) {
					m_currProgressGRect.position.x = contentGRect.Right();
					m_indeterminateProgressData.delta = 0.f;
				}
			}

			break;
		}
		case ProgressFillMode::BottomToTop:
		case ProgressFillMode::TopToBottom:
		case ProgressFillMode::CenterV: {
			float length = m_indeterminateProgressData.lengthScale * contentGRect.size.h;
			length = SDL_clamp(length, m_indeterminateProgressData.minLength, contentGRect.size.h);

			m_currProgressGRect.size.h = length;
			m_currProgressGRect.size.w = contentGRect.size.w;
			m_currProgressGRect.position.x = contentGRect.position.x;

			if (m_fillMode == ProgressFillMode::BottomToTop) {
				m_currProgressGRect.position.y = contentGRect.Bottom() - m_indeterminateProgressData.delta;
				if (m_currProgressGRect.Bottom() < contentGRect.Top()) {
					m_currProgressGRect.position.y = contentGRect.Bottom();
					m_indeterminateProgressData.delta = 0.f;
				}
			}
			else {
				m_currProgressGRect.position.y = m_indeterminateProgressData.delta + contentGRect.Top() - length;
				if (m_currProgressGRect.Top() > contentGRect.Bottom()) {
					m_currProgressGRect.position.y = contentGRect.Top() - length;
					m_indeterminateProgressData.delta = 0.f;
				}
			}
		}

			break;
		}
	}

	void ProgressBar::SetValue(float value) {
		Range::SetValue(value);

		m_progressLbl->SetText(std::format("{}%", 
			static_cast<int>(GetValueToMinValueInterval() / GetInterval() * 100)));
	}
}