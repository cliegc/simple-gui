#pragma once
#include <utility>
#include <SDL3/SDL_time.h>
#include "math.hpp"


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

	class Renderer;
	class Timer;

	class Caret {
	public:
		Caret();
		virtual ~Caret();

		void Render(const Renderer& renderer, Rect& rect) const;

		inline float GetWidth() const { return m_width; }
		inline void SetWidth(float width) { m_width = width; }

		inline Color GetColor() const { return m_color; }
		inline void SetColor(const Color& color) { m_color = color; }

		inline bool IsBlink() const { return m_blink; }
		inline void SetBlink(bool blink) { m_blink = blink; }

		inline float GetBlinkInterval() const { return m_timer->GetInterval(); }
		inline void SetBlinkInterval(float interval) { m_timer->SetInterval(interval); }

	private:
		float m_width;
		Color m_color;
		bool m_blink;
		bool m_visible;
		Timer* m_timer;
	};
}