#include "component/layout/box_layout.hpp"
#include "gui_manager.hpp"
#include <algorithm>


namespace SimpleGui {
	BoxLayout::BoxLayout(Direction direction) : Layout() {
		m_direction = direction;
		m_alignment = Alignment::Begin;
		//m_padding = SG_GuiManager.GetCurrentStyle().componentPadding;
		//m_spacing = SG_GuiManager.GetCurrentStyle().itemSpacing;
	}

	int BoxLayout::GetSpacing() const {
		return m_spacing;
	}

	void BoxLayout::SetSpacing(int spacing) {
		m_spacing = spacing;
	}

	Direction BoxLayout::GetDirection() const {
		return m_direction;
	}

	void BoxLayout::SetDirection(Direction direction) {
		m_direction = direction;
	}

	Alignment BoxLayout::GetAlignment() const {
		return m_alignment;
	}

	void BoxLayout::SetAlignment(Alignment alignment) {
		m_alignment = alignment;
	}

	const std::vector<int>& BoxLayout::GetWeights() const {
		return m_weigths;
	}

	void BoxLayout::SetWeights(const std::vector<int>& weights) {
		m_weigths.clear();

		for (int i = 0; i < weights.size(); ++i) {
			int weight = weights[i] < 0 ? 1 : weights[i];
			m_weigths.push_back(weight);
		}
	}

	void BoxLayout::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		// 遍历更新子控件的准备工作，
		PreparationOfUpdateChildren();

		//SetMinSize(CalcAllChildrenMinSize());
		//CalcVisibleRect();
		CalcVisibleGlobalRect(m_parent, this);
		
		if (m_direction == Direction::Horizontal) UpdateHorizontalDirection();
		else UpdateVerticalDirection();
	}

	void BoxLayout::UpdateHorizontalDirection() {
		int count = GetVisibleChildrenCount();
		if (!count) return;

		Rect contentGRect = GetContentGlobalRect();

		Vec2 startPos;
		Vec2 maxChildSize((contentGRect.size.w - (count - 1) * m_spacing) / count, contentGRect.size.h);

		std::vector<BaseComponent*> fixedSizeComponets;
		std::vector<BaseComponent*> expandingSizeComponents;
		float totalFixedWidth = 0.0f;
		int totalWeight = 0;

		for (int i = 0; i < m_children.size(); ++i) {
			if (m_children[i]->IsVisible()) {
				if (m_children[i]->GetSizeConfigs().first == ComponentSizeConfig::Fixed) {
					fixedSizeComponets.push_back(m_children[i].get());
					totalFixedWidth += m_children[i]->GetSize().w;
				}
				else if (m_children[i]->GetSizeConfigs().first == ComponentSizeConfig::Expanding) {
					expandingSizeComponents.push_back(m_children[i].get());
				}
			}

			// 若权重数组中的值的数量少于子组件的数量时，缺少指定权重值的组件的权重值设为1
			totalWeight += (i < m_weigths.size() ? (!m_children[i]->IsVisible() ? 0 : m_weigths[i]) : 1);
		}

		// 更新子组件的位置和大小
		// 所有子组件x轴方向上的sizeconfig皆为expanding时，权重数组生效
		if (fixedSizeComponets.empty()) {
			// 权重数组为空时，默认所有组件的权重都为1
			if (m_weigths.empty() || (m_weigths.size() == 1 && m_weigths[0] == 1)) {
				for (int i = 0; i < expandingSizeComponents.size(); ++i) {
					auto child = expandingSizeComponents[i];
					float x = i * (maxChildSize.w + m_spacing);
					child->SetPosition(x, 0);
					child->SetSize(maxChildSize);

					child->Update();
				}
			}
			else {
				float offsetX = 0.0;
				float totalChildrenWidth = contentGRect.size.w - (count - 1) * m_spacing;
				for (int i = 0; i < expandingSizeComponents.size(); ++i) {
					auto child = expandingSizeComponents[i];
					float w = static_cast<float>(m_weigths[i]) / totalWeight * totalChildrenWidth;
					child->SetPosition(offsetX, 0);
					child->SetSize(w, maxChildSize.h);
					offsetX += w + m_spacing;

					child->Update();
				}
			}
		}

		// 权重无效
		else {
			// 没有Expanding，都是Fixed,实现Alignment, 默认为Alignment::Begin
			if (expandingSizeComponents.empty()) {
				if (m_alignment == Alignment::Center) {
					startPos.x = (contentGRect.size.w - totalFixedWidth - (count - 1) * m_spacing) / 2;
				}
				else if (m_alignment == Alignment::End) {
					startPos.x = contentGRect.size.w - totalFixedWidth - (count - 1) * m_spacing;
				}
			}

			float expandingWidth = (contentGRect.size.w - totalFixedWidth - (count - 1) * m_spacing) / expandingSizeComponents.size();

			// 更新子组件的矩形数据
			float offsetX = 0.0;
			for (int i = 0; i < m_children.size(); ++i) {
				auto child = m_children[i].get();
				float x = startPos.x + offsetX;

				if (!child->IsVisible()) continue;

				if (child->GetSizeConfigs().first == ComponentSizeConfig::Fixed) {
					offsetX += child->GetSize().w + m_spacing;
				}
				else if (child->GetSizeConfigs().first == ComponentSizeConfig::Expanding) {
					offsetX += expandingWidth + m_spacing;
					child->SetWidth(expandingWidth);
				}

				child->SetPosition(x, startPos.y);
				child->SetHeight(maxChildSize.h);

				child->Update();
			}
		}
	}

	void BoxLayout::UpdateVerticalDirection() {
		int count = GetVisibleChildrenCount();
		if (!count) return;

		Rect contentGRect = GetContentGlobalRect();

		Vec2 startPos;
		Vec2 maxChildSize(contentGRect.size.w, (contentGRect.size.h - (count - 1) * m_spacing) / count);

		std::vector<BaseComponent*> fixedSizeComponets;
		std::vector<BaseComponent*> expandingSizeComponents;
		float totalFixedHeight = 0.0f;
		int totalWeight = 0;

		for (int i = 0; i < m_children.size(); ++i) {

			if (m_children[i]->IsVisible()) {
				if (m_children[i]->GetSizeConfigs().second == ComponentSizeConfig::Fixed) {
					fixedSizeComponets.push_back(m_children[i].get());
					totalFixedHeight += m_children[i]->GetSize().h;
				}
				else if (m_children[i]->GetSizeConfigs().second == ComponentSizeConfig::Expanding) {
					expandingSizeComponents.push_back(m_children[i].get());
				}
			}

			// 若权重数组中的值的数量少于子组件的数量时，缺少指定权重值的组件的权重值设为1
			totalWeight += (i < m_weigths.size() ? (!m_children[i]->IsVisible() ? 0 : m_weigths[i]) : 1);
		}

		// 更新子组件的位置和大小
		// 所有子组件y轴方向上的sizeconfig皆为expanding时，权重数组生效
		if (fixedSizeComponets.empty()) {
			// 权重数组为空时，默认所有组件的权重都为1
			if (m_weigths.empty() || (m_weigths.size() == 1 && m_weigths[0] == 1)) {
				for (int i = 0; i < expandingSizeComponents.size(); ++i) {
					auto child = expandingSizeComponents[i];
					child->SetPosition(0, i * (maxChildSize.h + m_spacing));
					child->SetSize(maxChildSize);

					child->Update();
				}
			}
			else {
				float offsetY = 0.0;
				float totalChildrenHeight = contentGRect.size.h - (count - 1) * m_spacing;
				for (int i = 0; i < expandingSizeComponents.size(); ++i) {
					auto child = expandingSizeComponents[i];
					float h = static_cast<float>(m_weigths[i]) / totalWeight * totalChildrenHeight;
					float y = offsetY;
					child->SetPosition(0, y);
					child->SetSize(maxChildSize.w, h);
					offsetY += h + m_spacing;

					child->Update();
				}
			}
		}

		// 权重无效
		else {
			// 没有Expanding，都是Fixed,实现Alignment
			if (expandingSizeComponents.empty()) {
				if (m_alignment == Alignment::Center) {
					startPos.y = (contentGRect.size.h - totalFixedHeight - (count - 1) * m_spacing) / 2;
				}
				else if (m_alignment == Alignment::End) {
					startPos.y = contentGRect.size.h - totalFixedHeight - (count - 1) * m_spacing;
				}
			}

			float expandingHeight = (contentGRect.size.h - totalFixedHeight - (count - 1) * m_spacing) / expandingSizeComponents.size();

			// 更新子组件的矩形数据
			float offsetY = 0.0;
			for (int i = 0; i < m_children.size(); ++i) {
				auto child = m_children[i].get();
				float y = startPos.y + offsetY;

				if (!child->IsVisible()) continue;


				if (child->GetSizeConfigs().second == ComponentSizeConfig::Fixed) {
					offsetY += child->GetSize().h + m_spacing;
				}
				else if (child->GetSizeConfigs().second == ComponentSizeConfig::Expanding) {
					offsetY += expandingHeight + m_spacing;
					child->SetHeight(expandingHeight);
				}

				child->SetPosition(startPos.x, y);
				child->SetWidth(maxChildSize.w);

				child->Update();
			}
		}
	}

	int BoxLayout::GetVisibleChildrenCount() const {
		return std::count_if(m_children.begin(), m_children.end(),
			[](const auto& cmp) {
				return cmp->IsVisible();
			});
	}
}