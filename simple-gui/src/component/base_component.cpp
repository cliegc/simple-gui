#include "component/base_component.hpp"
#include <algorithm>
#include "gui_manager.hpp"


namespace SimpleGui {
	BaseComponent::BaseComponent() {
		m_padding = { 0 };
	}

	void BaseComponent::PreparationOfUpdateChildren() {
		// add caches of children to m_children, and clear caches
		for (auto& child : m_childCaches) {
			m_children.push_back(std::move(child));
			child->EnteredComponentTree();
		}
		m_childCaches.clear();

		// remove NeedRemove children from m_children
		for (auto it = m_children.begin(); it != m_children.end();) {
			if (!(*it) || (*it)->m_needRemove) {
				it = m_children.erase(it);
			}
			if ((*it) && (*it)->m_needRemove) (*it)->ExitedComponentTree();
			else {
				++it;
			}
		}
	}

	void BaseComponent::CalcVisibleGlobalRect(BaseComponent* parent, BaseComponent* target) const {
		if (!parent) {
			target->m_visibleGRect = target->GetGlobalRect();
			return;
		}

		Rect globalRect = target->GetGlobalRect();
		Rect parentVisibleGRect = parent->m_visibleGRect;
		Rect parentContentGRect = parent->GetContentGlobalRect();
		Rect parentVisibleContentGRect = parentContentGRect.GetIntersection(parentVisibleGRect);

		target->m_visibleGRect = parentVisibleContentGRect.GetIntersection(globalRect);
	}

	Rect BaseComponent::CalcVisibleGlobalRect(const Rect& parentVisibleGRect, const Rect& parentContentGRect, const Rect& targetGRect) const {
		Rect parentVisibleContentGRect = parentContentGRect.GetIntersection(parentVisibleGRect);
		return parentVisibleContentGRect.GetIntersection(targetGRect);
	}

	void BaseComponent::UpdateChildSizeConfigs(BaseComponent* cmp) const {
		if (cmp->m_sizeConfigs.first == ComponentSizeConfig::Expanding) {
			cmp->m_position.x = 0;
			cmp->m_size.w = GetContentSize().w;
		}

		if (cmp->m_sizeConfigs.second == ComponentSizeConfig::Expanding) {
			cmp->m_position.y = 0;
			cmp->m_size.h = GetContentSize().h;
		}
	}

	Rect BaseComponent::CalcChildrenBoundaryGlobalRect(BaseComponent* cmp) const {
		// 在Update中准备工作之后调用，不需要考虑m_childCaches
		Rect rect;
		for (auto& child : cmp->m_children) {
			Rect globalRect = child->GetGlobalRect();
			rect.position = rect.position.Min(globalRect.position);
			rect.size = rect.size.Max(globalRect.BottomRight() - rect.position);
		}
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


	bool BaseComponent::HandleEvent(const SDL_Event& event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		// update extended functions
		for (auto& func : m_extFunctions) {
			func->HandleEvent(nullptr);
		}

		// handle events of m_children
		for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
			if (!(*it) || (*it)->m_needRemove) continue;
			if ((*it)->HandleEvent(event)) {
				// handle extended functions
				for (auto& func : (*it)->m_extFunctions) {
					func->HandleEvent(nullptr);
				}
				return true;
			}
		}

		// handle events of chaches [X]

		return false;
	}

	void BaseComponent::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		PreparationOfUpdateChildren();

		// calc visible size
		CalcVisibleGlobalRect(m_parent, this);

		// update extended functions
		for (auto& func : m_extFunctions) {
			func->Update();
		}

		// update child, and size configs of chid
		for (auto& child : m_children) {
			UpdateChildSizeConfigs(child.get());
			child->Update();

			// update child extended functions
			for (auto& func : child->m_extFunctions) {
				func->Update();
			}
		}
	}

	void BaseComponent::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		/*if (m_disabled) {
			renderer.FillRect(m_visibleRect, GetThemeColor(ThemeColorFlags::Disabled));
		}*/
		
		// render extended functions
		for (auto& func : m_extFunctions) {
			func->Render(renderer);
		}

		// render m_children
		for (auto& child : m_children) {
			child->Render(renderer);

			// render child extended functions
			for (auto& func : child->m_extFunctions) {
				func->Render(renderer);
			}
		}
	}

	Rect BaseComponent::GetContentGlobalRect() const {
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

	Vec2 BaseComponent::MapPositionToGlobal(const Vec2& pos) const {
		if (m_parent) return pos + m_parent->GetGlobalPosition() + m_parent->GetLocalCoordinateOriginOffset();
		return pos;
	}

	Vec2 BaseComponent::MapGlobalPositionToLocal(const Vec2& pos) const {
		if (m_parent) return pos - m_parent->GetGlobalPosition() - m_parent->GetLocalCoordinateOriginOffset();
		return pos;
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
		if (!child || child->GetParent() == this) {
			SDL_Log("AddChild: this child is null or parent of child already is this.\n");
			return;
		}
		if (child->GetParent()) {
			SDL_Log("AddChild: this child already has a parent node.\n");
			return;
		}

		child->m_parent = this;
		child->m_window = m_window;
		child->m_needRemove = false;
		auto temp = child.get();
		m_children.push_back(std::move(child));
		temp->EnteredComponentTree();
		SDL_Log("AddChild: child entered component tree.\n");
	}

	void BaseComponent::AddChildDeferred(std::unique_ptr<BaseComponent> child) {
		if (!child || child->GetParent() == this) return;
		if (child->GetParent()) {
			SDL_Log("AddChild: this child already has a parent node.");
			return;
		}

		child->m_parent = this;
		child->m_window = m_window;
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
			child->m_window = nullptr;
			child->ExitedComponentTree();
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
			child->m_window = nullptr;
			//child->m_needRemove = true;			// 其实不需要使用need_remove标记，移动后，原位置上为nullptr
			child->ExitedComponentTree();
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

	bool BaseComponent::HasChild(BaseComponent* cmp) {
		auto it = std::find_if(m_children.begin(), m_children.end(), 
			[cmp](auto& child) {
				return child.get() == cmp;
			});

		auto cachesIt = std::find_if(m_childCaches.begin(), m_childCaches.end(),
			[cmp](auto& child) {
				return child.get() == cmp;
			});

		return it != m_children.end() || cachesIt != m_childCaches.end();
	}

	void BaseComponent::ClearAllChildren() {
		m_children.clear();
		m_childCaches.clear();

		for (auto& child : m_children) {
			child->ExitedComponentTree();
		}
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

	Font& BaseComponent::GetFont() {
		if (!m_font || m_font->IsNull()) {
			Font& font = m_window ? m_window->GetFont() : SG_GuiManager.GetDefaultFont();
			m_font.reset(new Font(font.GetPath(), font.GetSize()));
		}
		return *m_font;
	}

	void BaseComponent::SetFont(std::string_view path, int size) {
		m_font.reset(new Font(path, size));
	}

	Color BaseComponent::GetThemeColor(ThemeColorFlags flag) {
		if (m_themeColorCaches.contains(flag)) return m_themeColorCaches[flag];
		else if (m_window) return m_window->GetCurrentStyle().colors[flag];
		else return SG_GuiManager.GetDefaultStyle().colors[flag];
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

	void BaseComponent::AddExtendedFunctions(std::unique_ptr<ExtendedFunctions> functions) {
		functions->m_target = this;
		m_extFunctions.push_back(std::move(functions));
	}

	void BaseComponent::RemoveExtendedFunctions(ExtendedFunctions* functions) {
		
	}

	void BaseComponent::ClearAllExtendedFunctions() {
	}
}
