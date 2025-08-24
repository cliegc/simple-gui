#pragma once
#include "base_component.hpp"


namespace SimpleGui {
	class RootComponent final : public BaseComponent {
		friend class GuiManager;

	public:
		~RootComponent() = default;

		virtual bool HandleEvent(const SDL_Event& event) override;
		virtual void Update() override;
		virtual void Render(const Renderer& renderer) override;

	private:
		RootComponent();

		void SetSizeToFillWindow();
	};
}