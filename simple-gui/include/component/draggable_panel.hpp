#pragma once
#include "base_component.hpp"
#include "label.hpp"


namespace SimpleGui {
	class DraggablePanel final : public BaseComponent {
	public:
		DraggablePanel(std::string_view title);
		~DraggablePanel() = default;

		virtual bool HandleEvent(Event* event) override;
		virtual void Update() override;
		virtual void Render(Renderer& renderer) override;

		inline std::string GetTitle() const { return m_titleLbl->GetText(); }
		inline void SetTitle(std::string_view title) { m_titleLbl->SetText(title); }

		inline bool IsHandleVisible() const { return m_handleVisible; }
		inline void SetHandleVisible(bool visible) { m_handleVisible = visible; }

		inline bool IsResizable() const { return m_resizable; }
		inline void SetResizable(bool resizable) { m_resizable = resizable; }

		inline bool IsGlobalDragEnable() const { return m_globalDragEnable; }
		inline void SetGlobalDragEnable(bool enable) { m_globalDragEnable = enable; }

		inline bool IsClampRanageFollowParentContent() const { return m_isClampRangeFollowParent; }
		inline void SetClampRangeFollowParentContent(bool enable) { m_isClampRangeFollowParent = enable; }

		virtual void SetFont(std::unique_ptr<Font> font) override;
		virtual void SetFont(std::string_view path, int size) override;

	protected:
		virtual void EnteredComponentTree() override;
		inline virtual Vec2 GetLocalCoordinateOriginOffset() const override;
		inline virtual Vec2 GetContentSize() const override;

	private:
		struct DragData {
			Rect dragGRect;
			Vec2 startMousePos;
			Vec2 startData;
			bool dragging;
		};

		struct FoldData {
			Rect toggleGRect;
			Vec2 unfoldSize;
			bool isFolded;
		};

	private:
		std::unique_ptr<Label> m_titleLbl;
		UniqueCursorPtr m_resizeCursor;
		float m_handleThickness;
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
