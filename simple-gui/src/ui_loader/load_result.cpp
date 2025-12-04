#include "ui_loader/load_result.hpp"


namespace SimpleGui::UILoader {
    std::shared_ptr<CheckBoxGroup> LoadResult::GetCheckBoxGroup(const std::string &name) const {
        if (!m_cbGroups.contains(name)) return nullptr;
        return m_cbGroups.at(name);
    }

    void LoadResult::EnterComponentTree(BaseComponent *parent) const {
        if (!parent || !m_root) return;
        if (m_root->GetParent() == parent) return;
        parent->AddChildDeferred(std::unique_ptr<BaseComponent>(m_root));
    }

    bool LoadResult::IsValid() const {
        return m_root != nullptr;
    }
}
