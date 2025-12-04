#include "component/layout/split_layout.hpp"
#include <array>

#include "event.hpp"


namespace SimpleGui {
    SplitLayout::SplitLayout(Direction direction) {
        m_direction = direction;
    }

    bool SplitLayout::HandleEvent(Event *event) {
        SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

        if (Layout::HandleEvent(event)) return true;

        return false;
    }

    void SplitLayout::Update() {
        SG_CMP_UPDATE_CONDITIONS;

        // 遍历更新子控件的准备工作，
        PreparationOfUpdateChildren();
        CalcVisibleGlobalRect(m_parent, this);

        if (m_children.empty()) return;
        if (m_direction == Direction::Horizontal) UpdateHorizontalDirection();
        else UpdateVerticalDirection();
    }

    void SplitLayout::Render(Renderer &renderer) {
        SG_CMP_RENDER_CONDITIONS;

        Layout::Render(renderer);

        // draw handle
        if (m_splitter.draw && m_splitter.visible) {
            renderer.RenderRect(m_splitter.rect.visibleGRect, GetThemeColor(ThemeColorFlags::SplitLayoutHandle), true);
        }
    }

    void SplitLayout::UpdateHorizontalDirection() {
        if (m_needUpdateWeights) {
            UpdateWeights(Direction::Horizontal);
            return;
        }

        const Rect contentGRect = GetContentGlobalRect();
    }


    void SplitLayout::UpdateVerticalDirection() {
        if (m_needUpdateWeights) {
            UpdateWeights(Direction::Vertical);
            return;
        }

        const Rect contentGRect = GetContentGlobalRect();
    }

    void SplitLayout::UpdateWeights(Direction direction) {
        const auto childrenCount = m_children.size();
        int totalWeight = 0;
        for (auto i = 0; i < childrenCount; ++i) {
            if (i >= m_weights.size()) {
                totalWeight++;
                continue;
            }
            totalWeight += m_weights[i];
        }

        const Rect contentGRect = GetContentGlobalRect();
        Vec2 pos = contentGRect.TopLeft();
        if (direction == Direction::Horizontal) {
            for (auto i = 0; i < childrenCount; ++i) {
                const auto child = m_children[i].get();
                const float width = (m_weights[i] / totalWeight * 1.f) * contentGRect.size.w;
                child->SetPosition(pos.x, contentGRect.Top());
                child->SetWidth(width);
                pos.x += width;
            }
        }else {
            for (auto i = 0; i < childrenCount; ++i) {
                const auto child = m_children[i].get();
                const float height = (m_weights[i] / totalWeight * 1.f) * contentGRect.size.h;
                child->SetPosition(contentGRect.Left(), pos.y);
                child->SetHeight(height);
                pos.y += height;
            }
        }

        m_needUpdateWeights = false;
    }

    void SplitLayout::HandleDragSplitter(Event *event, Direction direction) {
        if (auto ev = event->Convert<MouseButtonEvent>();
            ev && ev->IsPressed(MouseButton::Left)) {
            if (!m_splitter.rect.gRect.ContainPoint(ev->GetPosition())) return;
            m_dragSplitterData.startMousePos = ev->GetPosition();
            m_dragSplitterData.startSplitterPos = m_splitter.rect.gRect.TopLeft();
            m_dragSplitterData.canDrag = true;
        }

        if (m_dragSplitterData.canDrag) {
            if (auto ev = event->Convert<MouseButtonEvent>();
                ev && ev->IsReleased(MouseButton::Left)) {
                m_dragSplitterData.canDrag = false;
                m_dragSplitterData.dragging = false;
            }
            if (auto ev = event->Convert<MouseMotionEvent>()) {
                m_dragSplitterData.dragging = true;
            }
        }
    }

    void SplitLayout::SetWeights(const std::vector<int> &weights) {
        m_weights.clear();

        for (const int i : weights) {
            int weight = i < 0 ? 1 : i;
            m_weights.push_back(weight);
        }

        m_needUpdateWeights = true;
    }
}
