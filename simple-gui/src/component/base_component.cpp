#include "component/base_component.hpp"
#include <algorithm>
#include "gui_manager.hpp"


namespace SimpleGui {
	BaseComponent::BaseComponent() {
		m_parent = nullptr;
		m_needRemove = false;
		m_padding = { 0 };

		auto font = TTF_CopyFont(&SG_GuiManager.GetDefaultFont());
		if (!font) {
			SDL_Log("%s\n", SDL_GetError());
		}
		m_font = UniqueFontPtr(font);

	}

	void BaseComponent::PreparationOfUpdateChildren() {
		// add caches of children to m_children, and clear caches
		for (auto& child : m_childCaches) {
			m_children.push_back(std::move(child));
		}
		m_childCaches.clear();

		// remove NeedRemove children from m_children
		for (auto it = m_children.begin(); it != m_children.end();) {
			if (!(*it) || (*it)->m_needRemove) {
				it = m_children.erase(it);
			}
			else {
				++it;
			}
		}
	}

	void BaseComponent::CalcVisibleGlobalRect(BaseComponent* parent, BaseComponent* target) {
		if (!parent) {
			target->m_visibleGRect = target->GetGlobalRect();
			return;
		}

		if (parent->m_visibleGRect.Area() < parent->m_size.w * parent->m_size.h) {
			Rect globalRect = target->GetGlobalRect();
			Rect parentVisibaleRect = parent->m_visibleGRect;
			if (parentVisibaleRect.ContainRect(globalRect)) target->m_visibleGRect = globalRect;
			else target->m_visibleGRect = parentVisibaleRect.GetIntersection(globalRect);
			return;
		}

		Rect globalRect = target->GetGlobalRect();
		Rect parentContentRect = m_parent->GetContentGlobalRect();
		if (parentContentRect.ContainRect(globalRect)) target->m_visibleGRect = globalRect;
		else target->m_visibleGRect = parentContentRect.GetIntersection(globalRect);
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


	bool BaseComponent::HandleEvent(const SDL_Event& event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

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

		// update child, and size configs of chid
		for (auto& child : m_children) {
			if (child->m_sizeConfigs.first == ComponentSizeConfig::Expanding) {
				child->m_position.x = 0;
				child->m_size.w = GetContentSize().w;
			}

			if (child->m_sizeConfigs.second == ComponentSizeConfig::Expanding) {
				child->m_position.y = 0;
				child->m_size.h = GetContentSize().h;
			}

			child->Update();
		}
	}

	void BaseComponent::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		/*if (m_disabled) {
			renderer.FillRect(m_visibleRect, GetThemeColor(ThemeColorFlags::Disabled));
		}*/
		

		// render m_children
		for (auto& child : m_children) {
			child->Render(renderer);
		}
	}

	inline Rect BaseComponent::GetContentGlobalRect() const {
		Vec2 pos = GetGlobalPosition() + GetLocalCoordinateOriginOffset();
		return Rect(pos, GetContentSize());
	}

	Vec2 BaseComponent::GetGlobalPosition() const {
		if (m_parent) return m_position + m_parent->GetGlobalPosition() + m_parent->GetLocalCoordinateOriginOffset();
		return m_position;
	}

	void BaseComponent::SetGlobalPosition(const Vec2& pos) {
		if (m_parent) {
			m_position = pos - m_parent->GetGlobalPosition();
			return;
		}

		m_position = pos;
	}

	void BaseComponent::SetGlobalPosition(float x, float y) {
		if (m_parent) {
			Vec2 parentGlobalPos = m_parent->GetGlobalPosition();
			m_position.x = x - parentGlobalPos.x;
			m_position.y = y - parentGlobalPos.y;
			return;
		}

		m_position.x = x;
		m_position.y = y;
	}

	void BaseComponent::SetGlobalPositionX(float x) {
		if (m_parent) {
			m_position.x = x - m_parent->GetGlobalPosition().x;
			return;
		}

		m_position.x = x;
	}

	void BaseComponent::SetGlobalPositionY(float y) {
		if (m_parent) {
			m_position.y = y - m_parent->GetGlobalPosition().y;
			return;
		}

		m_position.y = y;
	}

	void BaseComponent::SetSize(const Vec2& size) {
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

	void BaseComponent::SetMinSize(const Vec2& size) {
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
	
	void BaseComponent::AddChild(std::unique_ptr<BaseComponent> child) {
		if (!child || child->GetParent() == this) return;
		if (child->GetParent()) {
			SDL_Log("AddChild: this child already has a parent node.");
			return;
		}

		child->m_parent = this;
		child->m_needRemove = false;
		m_children.push_back(std::move(child));
	}

	void BaseComponent::AddChildDeferred(std::unique_ptr<BaseComponent> child) {
		if (!child || child->GetParent() == this) return;
		if (child->GetParent()) {
			SDL_Log("AddChild: this child already has a parent node.");
			return;
		}

		child->m_parent = this;
		child->m_needRemove = false;
		m_childCaches.push_back(std::move(child));
	}

	BaseComponent* BaseComponent::GetChildAt(size_t idx) const {
		if (idx < m_children.size()) {
			return m_children[idx].get();
		}

		idx -= m_children.size();
		if (idx < m_childCaches.size()) {
			return m_childCaches[idx].get();
		}

		return nullptr;
	}

	std::unique_ptr<BaseComponent> BaseComponent::RemoveChild(BaseComponent* cmp) {
		auto it = std::find_if(m_children.begin(), m_children.end(),
			[cmp](auto& child) {
				return child.get() == cmp;
			});

		if (it != m_children.end()) {
			std::unique_ptr<BaseComponent> child = std::move(*it);
			m_children.erase(it);
			child->m_parent = nullptr;
			return child;
		}

		return nullptr;
	}

	std::unique_ptr<BaseComponent> BaseComponent::RemoveChildDeferred(BaseComponent* cmp) {
		auto it = std::find_if(m_children.begin(), m_children.end(),
			[cmp](auto& child) {
				return child.get() == cmp;
			});
		 
		if (it != m_children.end()) {
			std::unique_ptr<BaseComponent> child = std::move(*it);
			child->m_parent = nullptr;
			//child->m_needRemove = true;			// 其实不需要使用need_remove标记，移动后，原位置上为nullptr
			return child;
		}

		return nullptr;
	}

	size_t BaseComponent::GetChildrenCount() const {
		return std::count_if(m_children.begin(), m_children.end(),
			[](auto& child) {
				return child != nullptr;
			});
	}

	void BaseComponent::ClearAllChildren() {
		m_children.clear();
		m_childCaches.clear();
	}

	void BaseComponent::ClearAllChildrenDeferred() {
		for (auto& child : m_children) {
			child->m_needRemove = true;
		}

		for (auto& child : m_childCaches) {
			child->m_needRemove = true;
		}
	}

	void BaseComponent::ForEachChild(std::function<void(BaseComponent*)> fn) {
		for (auto& child : m_children) {
			if (!child || child->m_needRemove) continue;
			fn(child.get());
		}

		for (auto& child : m_childCaches) {
			if (!child || child->m_needRemove) continue;
			fn(child.get());
		}
	}

	TTF_Font& BaseComponent::GetFont() const {
		if (m_font) return *m_font;
		return SG_GuiManager.GetDefaultFont();
	}

	void BaseComponent::SetFont(std::string_view path, int size) {
		auto font = TTF_OpenFont(path.data(), size);
		if (!font) {
			SDL_Log("%s\n", SDL_GetError());
			return;
		}
		m_font.reset(font);
	}

	void BaseComponent::SetFontSize(int size) {
		if (!TTF_SetFontSize(m_font.get(), size)) {
			SDL_Log("%s\n", SDL_GetError());
		}
	}

	Color BaseComponent::GetThemeColor(ThemeColorFlags flag) {
		if (m_themeColorCaches.contains(flag)) return m_themeColorCaches[flag];
		else return SG_GuiManager.GetCurrentStyle().colors[flag];
	}

	void BaseComponent::CustomThemeColor(ThemeColorFlags flag, const Color& color) {
		if (m_themeColorCaches[flag] == color) return;
		m_themeColorCaches[flag] = color;
	}

	void BaseComponent::ClearCustomThemeColor(ThemeColorFlags flag) {
		m_themeColorCaches.erase(flag);
	}

	void BaseComponent::ClearCustomThemeColors() {
		m_themeColorCaches.clear();
	}

}
