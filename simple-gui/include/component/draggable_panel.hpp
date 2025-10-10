#pragma once
#include "base_component.hpp"
#include "label.hpp"


namespace SimpleGui {
	class DraggablePanel final : public BaseComponent {
	public:
		explicit DraggablePanel(std::string_view title = "draggable panel");
		~DraggablePanel() override = default;

		bool HandleEvent(Event* event) override;
		void Update() override;
		void Render(Renderer& renderer) override;

		std::string GetTitle() const { return m_titleLbl->GetText(); }
		void SetTitle(std::string_view title) const { m_titleLbl->SetText(title); }

		bool IsHandleVisible() const { return m_handleVisible; }
		void SetHandleVisible(bool visible) { m_handleVisible = visible; }

		bool IsResizable() const { return m_resizable; }
		void SetResizable(bool resizable) { m_resizable = resizable; }

		bool IsGlobalDragEnable() const { return m_globalDragEnable; }
		void SetGlobalDragEnable(bool enable) { m_globalDragEnable = enable; }

		bool IsClampRangeFollowParentContent() const { return m_isClampRangeFollowParent; }
		void SetClampRangeFollowParentContent(bool enable) { m_isClampRangeFollowParent = enable; }

		void SetFont(std::unique_ptr<Font> font) override;
		void SetFont(std::string_view path, int size) override;

	protected:
		void EnteredComponentTree() override;
		inline Vec2 GetLocalCoordinateOriginOffset() const override;
		inline Vec2 GetContentSize() const override;

	private:
		struct DragData {
			Rect dragGRect;
			Vec2 startMousePos;
			Vec2 startData;
			bool dragging{};
		};

		struct FoldData {
			Rect toggleGRect;
			Vec2 unfoldSize;
			bool isFolded{};
		};

	private:
		std::unique_ptr<Label> m_titleLbl;
		UniqueCursorPtr m_resizeCursor;
		float m_handleThickness{};
		float m_resizeBlockWidth;
		bool m_handleVisible;
		bool m_resizable;
		bool m_globalDragEnable;
		bool m_isClampRangeFollowParent;

		DragData m_dragData{};
		DragData m_resizeData{};
		FoldData m_foldData{};

	private:
		void ClampPosition();
		bool HandleDragMotion(Event* event, const Vec2& mousePos, bool globalDragEnable = false);
		bool HandleDragResize(Event* event, const Vec2& mousePos);
		bool HandleToggleFold(Event* event, const Vec2& mousePos);
	};
}
