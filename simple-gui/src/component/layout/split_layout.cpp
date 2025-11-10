#include "component/layout/split_layout.hpp"


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

        if (m_direction == Direction::Horizontal) UpdateHorizontalDirection();
        else UpdateVerticalDirection();
    }

    void SplitLayout::Render(Renderer &renderer) {
        SG_CMP_RENDER_CONDITIONS;

        Layout::Render(renderer);

        // draw handle
        if (m_handle.draw && m_handle.visible) {
            renderer.RenderRect(m_handle.rect.visibleGRect, GetThemeColor(ThemeColorFlags::SplitLayoutHandle), true);
        }
    }

    void SplitLayout::UpdateHorizontalDirection() {

    }

    void SplitLayout::UpdateVerticalDirection() {
    }

    void SplitLayout::SetWeights(const std::vector<int> &weights) {
        m_weights.clear();

        for (const int i : weights) {
            int weight = i < 0 ? 1 : i;
            m_weights.push_back(weight);
        }
    }
}
