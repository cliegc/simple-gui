#pragma once
#include "../base_component.hpp"
//#define SG_CMP_DEBUG_LAYOUT_BG

namespace SimpleGui {
	class Layout : public BaseComponent {
	public:
		Layout() = default;
		virtual ~Layout() = default;

#ifdef SG_CMP_DEBUG_LAYOUT_BG
		virtual void Render(Renderer& renderer) override {
			SG_CMP_RENDER_CONDITIONS;
			renderer.FillRect(GetGlobalRect(), GetThemeColor(ThemeColorFlags::Background));
			BaseComponent::Render(renderer);
		}
#endif // SG_CMP_LAYOUT_BG
	};
}