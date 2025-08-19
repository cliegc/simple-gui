#include "style.hpp"
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

	Style* StyleManager::GetStyle(const std::string& name) {
		if (m_styles.contains(name)) {
			return m_styles[name].get();
		}
		return nullptr;
	}

	void StyleManager::SwitchStyle(const std::string& name) {
		if (!m_styles.contains(name)) return;
		auto style = m_styles[name].get();
		if (style == m_currStyle) return;
		m_currStyle = style;
	}

	Style* StyleManager::GetCurrentStyle() const {
		return m_currStyle;
	}

	std::unique_ptr<Style> StyleManager::CreateLightStyle() const {
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

		return std::move(style);
	}

	std::unique_ptr<Style> StyleManager::CreateDarkStyle() const {
		auto style = std::make_unique<Style>();
		style->colors[ThemeColorFlags::Background] = Color(47, 47, 47);
		style->colors[ThemeColorFlags::Foreground] = Color::WHITE;
		style->colors[ThemeColorFlags::Border] = Color(0x2c3e50ff);

		style->colors[ThemeColorFlags::LabelBackgound] = Color::TRANSPARENT;
		style->colors[ThemeColorFlags::LabelForeground] = Color::WHITE;
		style->colors[ThemeColorFlags::LabelBorder] = Color::TRANSPARENT;

		style->colors[ThemeColorFlags::ButtonNormal] = Color(0x34495eff);
		style->colors[ThemeColorFlags::ButtonHovered] = Color(0x34495e64);
		style->colors[ThemeColorFlags::ButtonPressed] = Color(0x34495e32);
		style->colors[ThemeColorFlags::ButtonForeground] = Color::WHITE;

		return std::move(style);
	}

}