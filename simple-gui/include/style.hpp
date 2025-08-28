#pragma once
#include <string>
#include <memory>
#include <vector>
#include <array>
#include <unordered_map>
#include <optional>
#include "math.hpp"
#include "component/common.hpp"



namespace SimpleGui {
	enum class ThemeColorFlags {
		Background,
		Foreground,
		Border,
		Disabled,

		LabelBackgound,
		LabelForeground,
		LabelBorder,

		ButtonNormal,
		ButtonHovered,
		ButtonPressed,
		ButtonForeground,
		ButtonBorder,

		TextureRectBackround,
		TextureRectBorder,

		DraggablePanelBackround,
		DraggablePanelForeground,
		DraggablePanelHandle,
		DraggablePanelSizeGrip,
		DraggablePanelBorder,

		ScrollbarSlot,
		ScrollbarSlider,
		ScrollbarSliderHovered,
		ScrollbarSliderPressed,
		ScrollbarBorder,

		FlagsTotal
	};


	struct ThemeColors final {
		std::array<Color, (size_t)ThemeColorFlags::FlagsTotal>  colors;

		Color& operator[](ThemeColorFlags index) {
			return colors[(size_t)index];
		}

		Color& operator[](size_t index) {
			return colors[index];
		}

		Color operator[](ThemeColorFlags index) const {
			return colors[(size_t)index];
		}

		Color operator[](size_t index) const {
			return colors[index];
		}
	};


	struct Style final {
		ComponentPadding componentPadding{ 5,5,5,5 };
		int itemSpacing = 5;
		// ......

		ThemeColors  colors;
	};


	class StyleManager final {
	public:
		static const std::string LightStyle;
		static const std::string DarkStyle;

	public:
		StyleManager();
		~StyleManager() = default;

		StyleManager(const StyleManager&) = delete;
		StyleManager& operator=(const StyleManager&) = delete;
		StyleManager(StyleManager&&) = delete;
		StyleManager& operator=(StyleManager&&) = delete;

	private:
		friend class GuiManager;

		bool RegisterStyle(const std::string& name, std::unique_ptr<Style> style);
		bool UnregisterStyle(const std::string& name);
		Style& GetCurrentStyle() const;
		std::optional<std::reference_wrapper<Style>> GetStyle(const std::string& name);
		void SwitchStyle(const std::string& name);
		void SetStyleFollowSystem();

		std::unique_ptr<Style> CreateLightStyle() const;
		std::unique_ptr<Style> CreateDarkStyle() const;

	private:
		std::unordered_map<std::string, std::unique_ptr<Style>> m_styles;
		Style* m_currStyle;
	};
}