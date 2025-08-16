#pragma once
#include <utility>


namespace SimpleGui {
	struct ComponentPadding {
		int left;
		int top;
		int right;
		int bottom;
	};

	enum class ComponentSizeConfig {
		Fixed,
		Expanding,
	};

	using ComponentSizeConfigs = std::pair<ComponentSizeConfig, ComponentSizeConfig>;

	enum class Direction {
		Horizontal,
		Vertical,
	};

	enum class Alignment {
		Begin,
		Center,
		End,
	};

	enum class TextAlignment {
		Left,
		Right,
		Top,
		Bottom,
		Center,
	};

	using TextAlignments = std::pair<TextAlignment, TextAlignment>;

	enum class MouseState {
		Normal,
		Hovering,
		Pressed,
		Release,
	};

	enum class TextureStretchMode {		// 控制在调整组件大小时图片显示方式 
		Scale,							// 图片大小跟随组件大小（不超过组件大小），不保证图片的宽高比
		Tile,							// 图片平铺整个控件
		Keep,							// 保持图片的大小，并在组件的左上角显示
		KeepCentered,					// 保持图片的大小，并在组件中心位置显示
		KeepAspect,						// 图片大小跟随组件大小（不超过组件大小），保证图片的宽高比，并在组件的左上角显示
		KeepAspectCentered,				// 图片大小跟随组件大小（不超过组件大小），保证图片的宽高比，并在组件中心位置显示
		KeepAspectCovered,				// 图片大小跟随组件大小（超过组件大小），保证图片的宽高比，并在组件中心位置显示
	};
}