#include "component/draggable_panel.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	DraggablePanel::DraggablePanel(std::string_view title): BaseComponent() {
		m_titleLbl = std::make_unique<Label>(title);
		m_handleVisible = true;
		m_resizable = true;
		m_globalDragEnable = false;
		m_isClampRangeFollowParent = false;
		m_resizeBlockWidth = 10;
		m_resizeData.dragGRect.size.w = m_resizeBlockWidth;
		m_resizeData.dragGRect.size.h = m_resizeBlockWidth;
		m_foldData.toggleGRect.size.w = 10;
		m_foldData.toggleGRect.size.h = 10;
		//m_padding = SG_GuiManager.GetCurrentStyle().componentPadding;
		m_handleThickness = m_titleLbl->GetSize().h;
		m_resizeCursor = UniqueCursorPtr(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NWSE_RESIZE));
		m_titleLbl->SetTextAlignments(TextAlignment::Left, TextAlignment::Center);
		m_titleLbl->CustomThemeColor(ThemeColorFlags::LabelBackgound, Color::TRANSPARENT);
		SetMinSize(m_titleLbl->GetSize().w + m_foldData.toggleGRect.size.w + 5, 
			m_titleLbl->GetSize().h + m_dragData.dragGRect.size.h);
	}

	bool DraggablePanel::HandleEvent(Event* event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		//Vec2 renderPos = SG_GuiManager.GetRenderer().GetRenderPositionFromMouse();
		Vec2 renderPos;

		//if (HandleToggleFold(event, renderPos)) return true;
		//if (HandleDragResize(event, renderPos)) return true;
		//if (HandleDragMotion(event, renderPos)) return true;

		//if (BaseComponent::HandleEvent(event)) return true;

		//// 启用全局拖拽必须放在子组件的事件处理之后，否则不会处理子组件的事件
		//if (HandleDragMotion(event, renderPos, m_globalDragEnable)) return true;
		//if (GetGlobalRect().ContainPoint(renderPos)) return true;

		return false;
	}

	void DraggablePanel::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		if (m_isClampRangeFollowParent) ClampPosition();
		BaseComponent::Update();

		// update handle_rect
		Rect globalRect = GetGlobalRect();
		// 必须放在计算可见矩形之后，否则更新的handle_rect的位置是上一帧可见矩形的位置
		m_handleThickness = m_titleLbl->GetSize().h;
		m_dragData.dragGRect.position = m_visibleGRect.position;
		m_dragData.dragGRect.size.w = m_visibleGRect.size.w;
		m_dragData.dragGRect.size.h = m_handleThickness;
		if (m_position.y < 0) {
			m_dragData.dragGRect.size.h = m_handleThickness + m_position.y;
			if (m_dragData.dragGRect.size.h < 0) m_dragData.dragGRect.size.h = 0;
		}

		// update size_grip_rect
		Vec2 bottomRight = globalRect.BottomRight();
		m_resizeData.dragGRect.position = bottomRight - Vec2(m_resizeBlockWidth, m_resizeBlockWidth);
		Vec2 size = m_visibleGRect.BottomRight() - m_resizeData.dragGRect.position;
		m_resizeData.dragGRect.size.w = size.w < 0 ? 0 : size.w;
		m_resizeData.dragGRect.size.h = size.h < 0 ? 0 : size.h;

		// update fold btn rect
		Vec2 globalePos = globalRect.position;
		m_foldData.toggleGRect.position.x = globalePos.x + 5;
		m_foldData.toggleGRect.position.y = globalePos.y + (m_handleThickness - m_foldData.toggleGRect.size.h) / 2;

		// update title label
		m_titleLbl->SetGlobalPositionX(m_foldData.toggleGRect.Right());
		m_titleLbl->SetGlobalPositionY(globalePos.y);
		m_titleLbl->Update();
		Rect visibleRect = CalcVisibleGlobalRect(m_dragData.dragGRect, m_dragData.dragGRect, m_titleLbl->GetGlobalRect());
		SetComponentVisibleGlobalRect(m_titleLbl.get(), visibleRect);
	}

	void DraggablePanel::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		renderer.FillRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::DraggablePanelBackround));

		BaseComponent::Render(renderer);
		
		if (m_handleVisible) {
			// draw handle
			renderer.FillRect(m_dragData.dragGRect, GetThemeColor(ThemeColorFlags::DraggablePanelHandle));
			// draw title
			m_titleLbl->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::DraggablePanelForeground));
			m_titleLbl->Render(renderer);

			renderer.SetClipRect(m_visibleGRect);
			// draw fold btn
			if (m_foldData.isFolded) {
				Vec2 bottomCenter(
					m_foldData.toggleGRect.position.x + m_foldData.toggleGRect.size.x / 2,
					m_foldData.toggleGRect.position.y + m_foldData.toggleGRect.size.y
				);
				renderer.FillTriangle(
					m_foldData.toggleGRect.TopLeft(),
					m_foldData.toggleGRect.TopRight(),
					bottomCenter,
					GetThemeColor(ThemeColorFlags::DraggablePanelForeground)
				);
			}
			else {
				Vec2 topCenter(
					m_foldData.toggleGRect.position.x + m_foldData.toggleGRect.size.x / 2,
					m_foldData.toggleGRect.position.y
				);
				renderer.FillTriangle(
					topCenter,
					m_foldData.toggleGRect.BottomRight(),
					m_foldData.toggleGRect.BottomLeft(),
					GetThemeColor(ThemeColorFlags::DraggablePanelForeground)
				);
			}
			renderer.ClearClipRect();
		}

		renderer.SetClipRect(m_visibleGRect);
		// draw size grip handle
		if (m_resizable && !m_foldData.isFolded) {
			Rect rect(m_resizeData.dragGRect.position, Vec2(m_resizeBlockWidth, m_resizeBlockWidth));
			renderer.FillTriangle(
				rect.BottomLeft(),
				rect.TopRight(),
				rect.BottomRight(),
				GetThemeColor(ThemeColorFlags::DraggablePanelSizeGrip)
			);
		}
		// draw border
		renderer.DrawRect(GetGlobalRect(), GetThemeColor(ThemeColorFlags::DraggablePanelBorder));
		renderer.ClearClipRect();

		// TODO 重写绘制disabled
	}

	void DraggablePanel::SetFont(std::unique_ptr<Font> font) {
		m_titleLbl->SetFont(std::move(font));
	}

	void DraggablePanel::SetFont(std::string_view path, int size) {
		m_titleLbl->SetFont(path, size);
	}

	inline Vec2 DraggablePanel::GetLocalCoordinateOriginOffset() const {
		float t = m_handleThickness;
		if (!m_handleVisible) t = 0;
		return Vec2(m_padding.left, m_padding.top + t);
	}

	inline Vec2 DraggablePanel::GetContentSize() const {
		float t = m_handleThickness;
		if (!m_handleVisible) t = 0;
		return Vec2(
			m_size.w - m_padding.left - m_padding.right,
			m_size.h - m_padding.top - m_padding.bottom - t
		);
	}

	void DraggablePanel::ClampPosition() {
		int w = 0, h = 0;
		//SDL_GetWindowSizeInPixels(&SG_GuiManager.GetWindow(), &w, &h);
		
		float t = m_handleThickness;
		if (!m_handleVisible) t = 0;

		Vec2 min{ m_handleThickness - m_size.w , 0};
		Vec2 max{ w - m_handleThickness, h - t };
		if (m_parent) {
			Vec2 contentSize = m_parent->GetContentGlobalRect().size;
			max.x = contentSize.w - m_handleThickness;
			max.y = contentSize.h - t;
		}

		if (max.x < 0) max.x = min.x;
		if (max.y < 0) max.y = min.y;

		m_position.Clamp(min, max);
	}

	bool DraggablePanel::HandleDragMotion(const SDL_Event& event, const Vec2& mousePos, bool globalDragEnable) {
		if ((m_handleVisible && !globalDragEnable && m_dragData.dragGRect.ContainPoint(mousePos)) ||
			(globalDragEnable && m_visibleGRect.ContainPoint(mousePos))) {
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
				m_dragData.startMousePos = mousePos;
				m_dragData.startData = m_position;
				m_dragData.dragging = true;
				return true;
			}
		}

		if (m_dragData.dragging) {
			if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT) {
				m_dragData.dragging = false;
			}
			else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				m_position = m_dragData.startData + mousePos - m_dragData.startMousePos;
			}
			return true;
		}
		return false;
	}

	bool DraggablePanel::HandleDragResize(const SDL_Event& event, const Vec2& mousePos) {
		if (m_resizable && m_resizeData.dragGRect.ContainPoint(mousePos)) {
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
				m_resizeData.startMousePos = mousePos;
				m_resizeData.startData = m_size;
				m_resizeData.dragging = true;
				SDL_SetCursor(m_resizeCursor.get());
				return true;
			}
		}

		if (m_resizeData.dragging) {
			if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT) {
				m_resizeData.dragging = false;
				SDL_SetCursor(SDL_GetDefaultCursor());
			}
			else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				Vec2 size = m_resizeData.startData + mousePos - m_resizeData.startMousePos;
				SetSize(size);
			}
			return true;
		}
		return false;
	}

	bool DraggablePanel::HandleToggleFold(const SDL_Event& event, const Vec2& mousePos) {
		if (!m_handleVisible) return false;
		if (m_dragData.dragGRect.ContainPoint(mousePos) && m_foldData.toggleGRect.ContainPoint(mousePos)) {
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (!m_foldData.isFolded) {
					m_foldData.unfoldSize = m_size;
					m_foldData.isFolded = true;
					m_size.h = m_handleThickness;
					return true;
				}
				else {
					m_foldData.isFolded = false;
					m_size.h = m_foldData.unfoldSize.h;
					return true;
				}
			}
		}
		return false;
	}
}