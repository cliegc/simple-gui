#pragma once
#include "base_component.hpp"


namespace SimpleGui {
	class RootComponent final : public BaseComponent {
		friend class Window;

	public:
		~RootComponent() override = default;

		RootComponent(const RootComponent&) = delete;
		RootComponent& operator=(const RootComponent&) = delete;
		RootComponent(RootComponent&&) = delete;
		RootComponent& operator=(RootComponent&&) = delete;

		bool HandleEvent(Event* event) override;
		void Update() override;
		void Render(Renderer& renderer) override;

		BaseComponent* GetHandlingComponent() const { return m_handlingCmp; }
		void SetHandlingComponent(BaseComponent* cmp) { m_handlingCmp = cmp; }

	private:
		BaseComponent* m_handlingCmp;

		explicit RootComponent(Window* window);
		void SetSizeToFillWindow();
	};
}