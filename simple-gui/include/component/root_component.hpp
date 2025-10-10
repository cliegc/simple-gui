#pragma once
#include "base_component.hpp"


namespace SimpleGui {
	class RootComponent final : public BaseComponent {
		friend class Window;

	public:
		~RootComponent() override = default;

		bool HandleEvent(Event* event) override;
		void Update() override;
		void Render(Renderer& renderer) override;

	private:
		explicit RootComponent(Window* window);

		void SetSizeToFillWindow();
	};
}