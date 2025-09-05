#pragma once
#include "base_component.hpp"


namespace SimpleGui {
	class RootComponent final : public BaseComponent {
		friend class Window;

	public:
		~RootComponent() = default;

		virtual bool HandleEvent(const SDL_Event& event) override;
		virtual void Update() override;
		virtual void Render(const Renderer& renderer) override;

	protected:
		virtual void EnteredComponentTree() override;

	private:
		RootComponent(Window* window);

		void SetSizeToFillWindow();
	};
}