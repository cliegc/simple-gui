#include "component/base_component.hpp"
#include <algorithm>
#include <memory>
#include "deleter.hpp"
#include "gui_manager.hpp"
#include "logger.hpp"


namespace SimpleGui {
    BaseComponent::BaseComponent() {
        m_padding = {0};
        m_extFunctionsManager = std::make_unique<ExtendedFunctionsManager>(this);
        m_toolTip = std::make_unique<ToolTip>(this);
    }

    void BaseComponent::PreparationOfUpdateChildren() {
        // add caches of children to m_children, and clear caches
        for (auto &child: m_childCaches) {
            m_children.push_back(std::move(child));
        }
        m_childCaches.clear();

        // remove NeedRemove children from m_children
        for (auto it = m_children.begin(); it != m_children.end();) {
            if (!(*it) || (*it)->m_needRemove) {
                if ((*it) && (*it)->m_needRemove) (*it)->ExitedComponentTree();
                it = m_children.erase(it);
            } else {
                ++it;
            }
        }
    }

    void BaseComponent::CalcVisibleGlobalRect(BaseComponent *parent, BaseComponent *target) const {
        if (!parent) {
            target->m_visibleGRect = target->GetGlobalRect();
            return;
        }

        Rect globalRect = target->GetGlobalRect();
        Rect parentVisibleGRect = parent->m_visibleGRect;
        Rect parentContentGRect = parent->GetContentGlobalRect();
        Rect parentVisibleContentGRect = parentContentGRect.GetIntersection(parentVisibleGRect);

        if (parentVisibleContentGRect.size.w < 0) parentVisibleContentGRect.size.w = 0;
        if (parentVisibleContentGRect.size.h < 0) parentVisibleContentGRect.size.h = 0;
        target->m_visibleGRect = parentVisibleContentGRect.GetIntersection(globalRect);
    }

    Rect BaseComponent::CalcVisibleGlobalRect(const Rect &parentVisibleGRect, const Rect &parentContentGRect,
                                              const Rect &targetGRect) const {
        Rect parentVisibleContentGRect = parentContentGRect.GetIntersection(parentVisibleGRect);
        if (parentVisibleContentGRect.size.w < 0) parentVisibleContentGRect.size.w = 0;
        if (parentVisibleContentGRect.size.h < 0) parentVisibleContentGRect.size.h = 0;
        return parentVisibleContentGRect.GetIntersection(targetGRect);
    }

    void BaseComponent::UpdateChildSizeConfigs(BaseComponent *cmp) const {
        if (cmp->m_sizeConfigs.first == ComponentSizeConfig::Expanding) {
            cmp->m_position.x = 0;
            cmp->m_size.w = GetContentSize().w;
        }

        if (cmp->m_sizeConfigs.second == ComponentSizeConfig::Expanding) {
            cmp->m_position.y = 0;
            cmp->m_size.h = GetContentSize().h;
        }
    }

    Rect BaseComponent::CalcChildrenBoundaryGlobalRect(BaseComponent *cmp) const {
        // 在Update中准备工作之后调用，不需要考虑m_childCaches
        if (cmp->GetChildrenCount() == 0) return {};
        Rect rect = cmp->GetChildAt(0)->GetGlobalRect();
        Vec2 bottomRight = rect.BottomRight();
        for (auto &child: cmp->m_children) {
            Rect globalRect = child->GetGlobalRect();
            rect.position = rect.position.Min(globalRect.position);
            bottomRight = bottomRight.Max(globalRect.BottomRight());
        }
        rect.size = bottomRight - rect.position;
        return rect;
    }

    inline Vec2 BaseComponent::GetLocalCoordinateOriginOffset() const {
        return Vec2(m_padding.left, m_padding.top);
    }

    inline Vec2 BaseComponent::GetContentSize() const {
        return Vec2(
            m_size.w - m_padding.left - m_padding.right,
            m_size.h - m_padding.top - m_padding.bottom
        );
    }


    bool BaseComponent::HandleEvent(Event *event) {
        SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

        m_toolTip->HandleEvent(event);

        // update extended functions
        m_extFunctionsManager->HandleEvent(event);

        // handle events of m_children
        for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
            if (!(*it) || (*it)->m_needRemove) continue;
            if ((*it)->HandleEvent(event)) return true;
        }

        // handle events of chaches [X]

        return false;
    }

    void BaseComponent::Update() {
        SG_CMP_UPDATE_CONDITIONS;

        PreparationOfUpdateChildren();

        // calc visible size
        CalcVisibleGlobalRect(m_parent, this);

        m_toolTip->Update();

        // update extended functions
        m_extFunctionsManager->Update();

        // update child, and size configs of child
        for (auto &child: m_children) {
            UpdateChildSizeConfigs(child.get());
            child->Update();
        }
    }

    void BaseComponent::Render(Renderer &renderer) {
        SG_CMP_RENDER_CONDITIONS;

        /*if (m_disabled) {
            renderer.FillRect(m_visibleRect, GetThemeColor(ThemeColorFlags::Disabled));
        }*/

        m_toolTip->Render(renderer);

        // render extended functions
        m_extFunctionsManager->Render(renderer);

        // render m_children
        for (auto &child: m_children) {
            child->Render(renderer);
        }
    }

    Rect BaseComponent::GetContentGlobalRect() const {
        Vec2 pos = GetGlobalPosition() + GetLocalCoordinateOriginOffset();
        return {pos, GetContentSize()};
    }

    Vec2 BaseComponent::GetGlobalPosition() const {
        if (m_parent) return m_position + m_parent->GetGlobalPosition() + m_parent->GetLocalCoordinateOriginOffset();
        return m_position;
    }

    void BaseComponent::SetGlobalPosition(const Vec2 &pos) {
        if (m_parent) {
            m_position = pos - m_parent->GetGlobalPosition() - m_parent->GetLocalCoordinateOriginOffset();
            return;
        }

        m_position = pos;
    }

    void BaseComponent::SetGlobalPosition(float x, float y) {
        if (m_parent) {
            Vec2 parentGlobalPos = m_parent->GetGlobalPosition();
            Vec2 parentOriginOfs = m_parent->GetLocalCoordinateOriginOffset();
            m_position.x = x - parentGlobalPos.x - parentOriginOfs.x;
            m_position.y = y - parentGlobalPos.y - parentOriginOfs.y;
            return;
        }

        m_position.x = x;
        m_position.y = y;
    }

    void BaseComponent::SetGlobalPositionX(float x) {
        if (m_parent) {
            m_position.x = x - m_parent->GetGlobalPosition().x - m_parent->GetLocalCoordinateOriginOffset().x;
            return;
        }

        m_position.x = x;
    }

    void BaseComponent::SetGlobalPositionY(float y) {
        if (m_parent) {
            m_position.y = y - m_parent->GetGlobalPosition().y - m_parent->GetLocalCoordinateOriginOffset().y;
            return;
        }

        m_position.y = y;
    }

    Vec2 BaseComponent::MapPositionToGlobal(const Vec2 &pos) const {
        if (m_parent) return pos + m_parent->GetGlobalPosition() + m_parent->GetLocalCoordinateOriginOffset();
        return pos;
    }

    Vec2 BaseComponent::MapGlobalPositionToLocal(const Vec2 &pos) const {
        if (m_parent) return pos - m_parent->GetGlobalPosition() - m_parent->GetLocalCoordinateOriginOffset();
        return pos;
    }

    void BaseComponent::SetSize(const Vec2 &size) {
        SetWidth(size.w);
        SetHeight(size.h);
    }

    void BaseComponent::SetSize(float w, float h) {
        SetWidth(w);
        SetHeight(h);
    }

    void BaseComponent::SetWidth(float w) {
        m_size.w = w < m_minSize.w ? m_minSize.w : w;
    }

    void BaseComponent::SetHeight(float h) {
        m_size.h = h < m_minSize.h ? m_minSize.h : h;
    }

    void BaseComponent::SetMinSize(const Vec2 &size) {
        SetMinWidth(size.w);
        SetMinHeight(size.h);
    }

    void BaseComponent::SetMinSize(float w, float h) {
        SetMinWidth(w);
        SetMinHeight(h);
    }

    void BaseComponent::SetMinWidth(float w) {
        m_minSize.w = w < 0 ? 0 : w;
        m_size.w = m_size.w < m_minSize.w ? m_minSize.w : m_size.w;
    }

    void BaseComponent::SetMinHeight(float h) {
        m_minSize.h = h < 0 ? 0 : h;
        m_size.h = m_size.h < m_minSize.h ? m_minSize.h : m_size.h;
    }

    //void BaseComponent::PresetChildrenCount(size_t count) {
    //	m_children.reserve(count);
    //}

    void BaseComponent::SetVisible(bool visible) {
        if (m_visible == visible) return;
        m_visible = visible;
        visibleChanged.Emit(visible);
    }

    void BaseComponent::AddChild(std::unique_ptr<BaseComponent> child) {
        if (!child || child->GetParent() == this) {
            SG_WARN("AddChild: this child is null or parent of child already is this.");
            return;
        }
        if (child->GetParent()) {
            SG_WARN("AddChild: this child already has a parent node.");
            return;
        }

        SetComponentOwner(child.get(), m_window, this);
        child->m_needRemove = false;
        auto temp = child.get();
        m_children.push_back(std::move(child));
        temp->EnteredComponentTree();
        SG_INFO("AddChild: child entered component tree");
    }

    void BaseComponent::AddChildDeferred(std::unique_ptr<BaseComponent> child) {
        if (!child || child->GetParent() == this) return;
        if (child->GetParent()) {
            SG_WARN("AddChild: this child already has a parent node.");
            return;
        }

        SetComponentOwner(child.get(), m_window, this);
        child->m_needRemove = false;
        auto temp = child.get();
        m_childCaches.push_back(std::move(child));
        temp->EnteredComponentTree();
    }

    BaseComponent *BaseComponent::GetChildAt(size_t idx) const {
        if (idx < m_children.size()) {
            return m_children[idx].get();
        }

        idx -= m_children.size();
        if (idx < m_childCaches.size()) {
            return m_childCaches[idx].get();
        }

        return nullptr;
    }

    std::unique_ptr<BaseComponent> BaseComponent::RemoveChild(BaseComponent *cmp) {
        auto it = std::ranges::find_if(m_children,
                                       [cmp](auto &child) {
                                           return child.get() == cmp;
                                       });

        if (it != m_children.end()) {
            std::unique_ptr<BaseComponent> child = std::move(*it);
            m_children.erase(it);
            child->m_parent = nullptr;
            child->m_window = nullptr;
            child->ExitedComponentTree();
            return child;
        }

        return nullptr;
    }

    std::unique_ptr<BaseComponent> BaseComponent::RemoveChildDeferred(BaseComponent *cmp) {
        auto it = std::ranges::find_if(m_children,
                                       [cmp](auto &child) {
                                           return child.get() == cmp;
                                       });

        if (it != m_children.end()) {
            std::unique_ptr<BaseComponent> child = std::move(*it);
            child->m_parent = nullptr;
            child->m_window = nullptr;
            //child->m_needRemove = true;			// 其实不需要使用need_remove标记，移动后，原位置上为nullptr
            child->ExitedComponentTree();
            return child;
        }

        return nullptr;
    }

    size_t BaseComponent::GetChildrenCount() const {
        auto count1 = std::ranges::count_if(m_children,
                                            [](auto &child) {
                                                return child != nullptr;
                                            });
        return count1 + m_childCaches.size();
    }

    bool BaseComponent::HasChild(BaseComponent *cmp) {
        auto it = std::ranges::find_if(m_children,
                                       [cmp](auto &child) {
                                           return child.get() == cmp;
                                       });

        auto cachesIt = std::ranges::find_if(m_childCaches,
                                             [cmp](auto &child) {
                                                 return child.get() == cmp;
                                             });

        return it != m_children.end() || cachesIt != m_childCaches.end();
    }

    void BaseComponent::ClearAllChildren() {
        m_children.clear();
        m_childCaches.clear();

        for (auto &child: m_children) {
            child->ExitedComponentTree();
        }
    }

    void BaseComponent::ClearAllChildrenDeferred() const {
        for (auto &child: m_children) {
            child->m_needRemove = true;
        }

        for (auto &child: m_childCaches) {
            child->m_needRemove = true;
        }
    }

    void BaseComponent::ForEachChild(const std::function<void(BaseComponent *)> &fn) {
        for (auto &child: m_children) {
            if (!child || child->m_needRemove) continue;
            fn(child.get());
        }

        for (auto &child: m_childCaches) {
            if (!child || child->m_needRemove) continue;
            fn(child.get());
        }
    }

    Font &BaseComponent::GetFont() {
        if (!m_font || m_font->IsNull()) {
            const Font &font = m_window ? m_window->GetFont() : SG_GuiManager.GetDefaultFont();
            m_font = std::make_unique<Font>(font.GetPath(), font.GetSize());
        }
        return *m_font;
    }

    void BaseComponent::SetFont(std::string_view path, int size) {
        m_font = std::make_unique<Font>(path, size);
    }

    Color BaseComponent::GetThemeColor(ThemeColorFlags flag) {
        if (m_themeColorCaches.contains(flag)) return m_themeColorCaches[flag];
        if (m_parent) return m_parent->GetThemeColor(flag);
        if (m_window) return m_window->GetCurrentStyle()->colors[flag];
        return SG_GuiManager.GetDefaultStyle().colors[flag];
    }

    void BaseComponent::CustomThemeColor(ThemeColorFlags flag, const Color &color) {
        if (m_themeColorCaches[flag] == color) return;
        m_themeColorCaches[flag] = color;
    }

    void BaseComponent::ClearCustomThemeColor(ThemeColorFlags flag) {
        m_themeColorCaches.erase(flag);
    }

    void BaseComponent::ClearCustomThemeColors() {
        m_themeColorCaches.clear();
    }

    BaseComponent::ToolTip::ToolTip(BaseComponent *target) {
        this->target = target;
        enabled = false;
        isFirstShown = true;
        m_timer = std::make_unique<Timer>(MOUSE_SATY_DURATION);
        m_timer->SetOneShot(true);
        m_timer->timeout.Connect("on_timeout",
                                 [this, target]() {
                                     // 显示component
                                     if (!enabled) return;

                                     if (isFirstShown) {
                                         target->SetComponentOwner(this->cmp.get(), target->m_window,
                                                                   &target->m_window->GetRootComponent());
                                         target->EnteredComponentTree(this->cmp.get());
                                         isFirstShown = false;
                                     }

                                     SetSafePositionForComponent();
                                     cmp->SetVisible(true);
                                 });
    }

    void BaseComponent::ToolTip::HandleEvent(Event *event) const {
        if (!enabled || !cmp) return;

        if (auto ev = event->Convert<MouseMotionEvent>()) {
            if (!target->GetVisibleGlobalRect().ContainPoint(ev->GetPosition())) {
                m_timer->SetPaused(true);
                if (cmp->IsVisible()) cmp->SetVisible(false);
                return;
            }

            if (cmp->IsVisible()) cmp->SetVisible(false);
            m_timer->Start();
        }
    }

    void BaseComponent::ToolTip::Update() const {
        if (!enabled || !cmp) return;

        m_timer->Update();
        cmp->Update();
    }

    void BaseComponent::ToolTip::Render(Renderer &renderer) const {
        if (!enabled || !cmp) return;

        renderer.SetTopRender(true);
        cmp->Render(renderer);
        renderer.SetTopRender(false);
    }

    void BaseComponent::ToolTip::SetSafePositionForComponent() const {
        const Vec2 winSize = SG_GuiManager.GetWindow().GetRootComponent().GetVisibleGlobalRect().size;
        Rect cmpRect = cmp->GetGlobalRect();

        cmpRect.position = SG_GuiManager.GetMousePosition();
        cmpRect.position.y += 15;

        if (cmpRect.Right() > winSize.w) cmpRect.position.x = winSize.w - cmpRect.size.w;
        if (cmpRect.Bottom() > winSize.h) cmpRect.position.y = winSize.h - cmpRect.size.h;

        cmp->SetGlobalPosition(cmpRect.position);
    }
}
