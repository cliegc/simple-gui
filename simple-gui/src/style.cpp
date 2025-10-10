#include "style.hpp"
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_log.h>


namespace SimpleGui {

	const std::string StyleManager::LightStyle = "light_style";
	const std::string StyleManager::DarkStyle = "dark_style";

	StyleManager::StyleManager() {
		auto lightStyle = CreateLightStyle();
		auto darkStyle = CreateDarkStyle();

		m_currStyle = lightStyle.get();
		m_styles.emplace(LightStyle, std::move(lightStyle));
		m_styles.emplace(DarkStyle, std::move(darkStyle));
	}

	bool StyleManager::RegisterStyle(const std::string& name, std::unique_ptr<Style> style) {
		auto r = m_styles.emplace(name, std::move(style));
		return r.second;
	}

	bool StyleManager::UnregisterStyle(const std::string& name) {
		if (name == LightStyle || name == DarkStyle) return false;

		auto it = m_styles.find(name);
		if (it != m_styles.end()) {
			m_styles.erase(name);
			return true;
		}
		return false;
	}

	std::optional<std::reference_wrapper<Style>> StyleManager::GetStyle(const std::string& name) {
		if (m_styles.contains(name)) {
			return *m_styles[name];
		}
		return std::nullopt;
	}

	void StyleManager::SwitchStyle(const std::string& name) {
		if (!m_styles.contains(name)) return;
		auto style = m_styles[name].get();
		if (style == m_currStyle) return;
		m_currStyle = style;
	}

	void StyleManager::SetStyleFollowSystem() {
		SDL_SystemTheme theme = SDL_GetSystemTheme();
		if (theme == SDL_SYSTEM_THEME_LIGHT) {
			SwitchStyle(LightStyle);
		}
		else {
			SwitchStyle(DarkStyle);
		}
	}

	Style& StyleManager::GetCurrentStyle() const {
		return *m_currStyle;
	}

	std::unique_ptr<Style> StyleManager::CreateLightStyle() {
		auto style = std::make_unique<Style>();
		style->colors[ThemeColorFlags::Background] = Color::WHITE;
		style->colors[ThemeColorFlags::Foreground] = Color::BLACK;
		style->colors[ThemeColorFlags::Border] = Color(0x7f8c8dff);
		style->colors[ThemeColorFlags::Disabled] = Color(0x95a5a664);

		style->colors[ThemeColorFlags::LabelBackgound] = Color::TRANSPARENT;
		style->colors[ThemeColorFlags::LabelForeground] = Color::BLACK;
		style->colors[ThemeColorFlags::LabelBorder] = Color::TRANSPARENT;

		style->colors[ThemeColorFlags::ButtonNormal] = Color(0x3498dbff);
		style->colors[ThemeColorFlags::ButtonHovered] = Color(0x3498db64);
		style->colors[ThemeColorFlags::ButtonPressed] = Color(0x3498db32);
		style->colors[ThemeColorFlags::ButtonForeground] = Color::WHITE;
		style->colors[ThemeColorFlags::ButtonBorder] = Color(0x7f8c8dff);

		style->colors[ThemeColorFlags::DraggablePanelBackround] = Color(0xffffff64);
		style->colors[ThemeColorFlags::DraggablePanelForeground] = Color::WHITE;
		style->colors[ThemeColorFlags::DraggablePanelBorder] = Color(0x7f8c8dff);
		style->colors[ThemeColorFlags::DraggablePanelHandle] = Color(0x3498dbff);
		style->colors[ThemeColorFlags::DraggablePanelSizeGrip] = Color(0x9badb7ff);

		style->colors[ThemeColorFlags::TextureRectBackround] = Color(0xffffff64);
		style->colors[ThemeColorFlags::TextureRectBorder] = Color(0x7f8c8dff);


		return style;
	}

	std::unique_ptr<Style> StyleManager::CreateDarkStyle() {
		auto style = std::make_unique<Style>();
		style->colors[ThemeColorFlags::Background] = Color(0x0d1117ff);
		style->colors[ThemeColorFlags::Foreground] = Color::WHITE;
		style->colors[ThemeColorFlags::Border] = Color(0x3d444dff);

		style->colors[ThemeColorFlags::LabelBackgound] = Color::TRANSPARENT;
		style->colors[ThemeColorFlags::LabelForeground] = Color::WHITE;
		style->colors[ThemeColorFlags::LabelBorder] = Color::TRANSPARENT;

		style->colors[ThemeColorFlags::ButtonNormal] = Color(0x8250dfff);
		style->colors[ThemeColorFlags::ButtonHovered] = Color(0x8250df64);
		style->colors[ThemeColorFlags::ButtonPressed] = Color(0x8250df32);
		style->colors[ThemeColorFlags::ButtonForeground] = Color::WHITE;
		style->colors[ThemeColorFlags::ButtonBorder] = Color(0x3d444dff);

		style->colors[ThemeColorFlags::DraggablePanelBackround] = Color(47, 47, 47, 100);
		style->colors[ThemeColorFlags::DraggablePanelForeground] = Color::WHITE;
		style->colors[ThemeColorFlags::DraggablePanelBorder] = Color(0x3d444dff);
		style->colors[ThemeColorFlags::DraggablePanelHandle] = Color(0x34495eff);
		style->colors[ThemeColorFlags::DraggablePanelSizeGrip] = Color(0x9badb7ff);

		style->colors[ThemeColorFlags::TextureRectBackround] = Color(47, 47, 47, 100);
		style->colors[ThemeColorFlags::TextureRectBorder] = Color(0x3d444dff);

		style->colors[ThemeColorFlags::ScrollbarSlot_H] = Color(47, 47, 47);
		style->colors[ThemeColorFlags::ScrollbarSlider_H] = Color(255, 0, 0);
		style->colors[ThemeColorFlags::ScrollbarBorder_H] = Color(0x3d444dff);
		style->colors[ThemeColorFlags::ScrollbarSliderHovered_H] = Color(255, 0, 0, 200);
		style->colors[ThemeColorFlags::ScrollbarSliderPressed_H] = Color(255,0 ,0, 150);

		style->colors[ThemeColorFlags::ScrollbarSlot_V] = Color(47, 47, 47);
		style->colors[ThemeColorFlags::ScrollbarSlider_V] = Color(255, 0, 0);
		style->colors[ThemeColorFlags::ScrollbarBorder_V] = Color(0x3d444dff);
		style->colors[ThemeColorFlags::ScrollbarSliderHovered_V] = Color(255, 0, 0, 200);
		style->colors[ThemeColorFlags::ScrollbarSliderPressed_V] = Color(255, 0, 0, 150);

		style->colors[ThemeColorFlags::ScrollPanelBackground] = Color::TRANSPARENT;
		style->colors[ThemeColorFlags::ScrollPanelBorder] = Color(0x3d444dff);

		style->colors[ThemeColorFlags::LineEditBackground] = Color(47, 47, 47);
		style->colors[ThemeColorFlags::LineEditForeground] = Color::WHITE;
		style->colors[ThemeColorFlags::LineEditPlaceholder] = Color(100, 100, 100);
		style->colors[ThemeColorFlags::LineEditBorder] = Color(0x3d444dff);
		style->colors[ThemeColorFlags::LineEditActivatedBorder] = Color(0x8250dfff);
		style->colors[ThemeColorFlags::LineEditCaret] = Color::WHITE;
		style->colors[ThemeColorFlags::LineEditSelectedBackground] = Color(0x8250dfff);
		style->colors[ThemeColorFlags::LineEditSelectedForeground] = Color::WHITE;

		style->colors[ThemeColorFlags::ProgressBarSlot] = Color(47, 47, 47);
		style->colors[ThemeColorFlags::ProgressBarProgress] = Color(0x8250dfff);
		style->colors[ThemeColorFlags::ProgressBarForeground] = Color::WHITE;
		style->colors[ThemeColorFlags::ProgressBarBorder] = Color(0x3d444dff);

		style->colors[ThemeColorFlags::SliderSlot] = Color(47, 47, 47);
		style->colors[ThemeColorFlags::SliderValue] = Color(0x8250df64);
		style->colors[ThemeColorFlags::SliderHandler] = Color(0x8250dfff);
		style->colors[ThemeColorFlags::SliderHandlerHovered] = Color(0x8266dfff);

		style->colors[ThemeColorFlags::CheckBoxBackground] = Color(47, 47, 47, 0);
		style->colors[ThemeColorFlags::CheckBoxForeground] = Color::WHITE;
		style->colors[ThemeColorFlags::CheckBoxCheckRectBackground] = Color(47, 47, 47, 200);
		style->colors[ThemeColorFlags::CheckBoxCheckedFlag] = Color(0x8250dfff);
		style->colors[ThemeColorFlags::CheckBoxCheckRectBorder] = Color(0x3d444dff);

		return style;
	}

}