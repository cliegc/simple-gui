#pragma once
#include <string>
#include <memory>
#include <array>
#include <unordered_map>
#include <optional>
#include "math.hpp"
#include "component/common/types.hpp"


namespace SimpleGui {
	enum class ThemeColorFlags {
		Background,
		Foreground,
		Border,
		Disabled,

		LabelBackground,
		LabelForeground,
		LabelBorder,

		ButtonNormal,
		ButtonHovered,
		ButtonPressed,
		ButtonForeground,
		ButtonBorder,

		TextureRectBackground,
		TextureRectBorder,

		DraggablePanelBackground,
		DraggablePanelForeground,
		DraggablePanelHandle,
		DraggablePanelSizeGrip,
		DraggablePanelBorder,

		ScrollbarSlot_H,
		ScrollbarSlider_H,
		ScrollbarSliderHovered_H,
		ScrollbarSliderPressed_H,
		ScrollbarBorder_H,
		
		ScrollbarSlot_V,
		ScrollbarSlider_V,
		ScrollbarSliderHovered_V,
		ScrollbarSliderPressed_V,
		ScrollbarBorder_V,
		
		ScrollPanelBackground,
		ScrollPanelBorder,

		LineEditBackground,
		LineEditForeground,
		LineEditPlaceholder,
		LineEditBorder,
		LineEditActivatedBorder,
		LineEditCaret,
		LineEditSelectedBackground,
		LineEditSelectedForeground,

		ProgressBarSlot,
		ProgressBarProgress,
		ProgressBarForeground,
		ProgressBarBorder,

		SliderSlot,
		SliderValue,
		SliderHandler,
		SliderHandlerHovered,

		CheckBoxBackground,
		CheckBoxForeground,
		CheckBoxCheckRectBackground,
		CheckBoxCheckedFlag,
		CheckBoxCheckRectBorder,

		ComboBoxBackground,
		ComboBoxForeground,
		ComboBoxBorder,
		ComboBoxItemSelected,
		ComboBoxItemHovered,

		FlagsTotal
	};


	struct ThemeColors final {
		std::array<Color, static_cast<size_t>(ThemeColorFlags::FlagsTotal)>  colors;

		Color& operator[](ThemeColorFlags index) {
			return colors[static_cast<size_t>(index)];
		}

		Color& operator[](size_t index) {
			return colors[index];
		}

		Color operator[](ThemeColorFlags index) const {
			return colors[static_cast<size_t>(index)];
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

		StyleManager();
		~StyleManager() = default;

		StyleManager(const StyleManager&) = delete;
		StyleManager& operator=(const StyleManager&) = delete;
		StyleManager(StyleManager&&) = delete;
		StyleManager& operator=(StyleManager&&) = delete;

	private:
		friend class GuiManager;
		friend class Window;

		bool RegisterStyle(const std::string& name, std::unique_ptr<Style> style);
		bool UnregisterStyle(const std::string& name);
		Style& GetCurrentStyle() const;
		std::optional<std::reference_wrapper<Style>> GetStyle(const std::string& name);
		void SwitchStyle(const std::string& name);
		void SetStyleFollowSystem();

		static std::unique_ptr<Style> CreateLightStyle();
		static std::unique_ptr<Style> CreateDarkStyle();

		std::unordered_map<std::string, std::unique_ptr<Style>> m_styles;
		Style* m_currStyle;
	};
}