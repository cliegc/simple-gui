#include "component/layout/anchor_point_layout.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	AnchorPointLayout::AnchorPointLayout() : Layout() {
		//m_padding = SG_GuiManager.GetCurrentStyle().componentPadding;
	}

	void AnchorPointLayout::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		PreparationOfUpdateChildren();
		CalcVisibleGlobalRect(m_parent, this);

		for (auto& child : m_children) {
			auto it = m_anchorPoints.find(child.get());
			if (it != m_anchorPoints.end()) {
				UpdateAnchorPointLocation(it->first, it->second);
			}
			UpdateChildSizeConfigs(child.get());
			child->Update();
		}
	}

	std::unique_ptr<BaseComponent> AnchorPointLayout::RemoveChild(BaseComponent* cmp) {
		m_anchorPoints.erase(cmp);
		return std::unique_ptr<BaseComponent>();
	}

	std::unique_ptr<BaseComponent> AnchorPointLayout::RemoveChildDeferred(BaseComponent* cmp) {
		m_anchorPoints.erase(cmp);
		return std::unique_ptr<BaseComponent>();
	}

	void AnchorPointLayout::SetAnchorPoint(BaseComponent* cmp, const AnchorPoint& point) {
		if (!HasChild(cmp)) return;
		m_anchorPoints.insert_or_assign(cmp, point);
	}

	void AnchorPointLayout::SetAnchorPoint(BaseComponent* cmp, AnchorPointType type, const Vec2& distance,
		AnchorPointLocation layoutLocation, AnchorPointLocation cmpLocation) {
		AnchorPoint point{ layoutLocation, cmpLocation, type, distance };
		SetAnchorPoint(cmp, point);
	}

	void AnchorPointLayout::UpdateAnchorPointLocation(BaseComponent* cmp, const AnchorPoint& point) {
		//Rect cmpRect = cmp->GetRect();
		Vec2 layoutAnchorPos;		// 局部坐标
		Vec2 distance;
		Vec2 contentSize = GetContentSize();

		if (point.type == AnchorPointType::Fixed) {
			distance = point.distance;
		}
		else {
			distance = m_size * point.distance;
		}

		switch (point.layoutLocation) {
		case AnchorPointLocation::TopLeft: {
			layoutAnchorPos.x = 0;
			layoutAnchorPos.y = 0;
			break;
		}
		case AnchorPointLocation::TopRight: {
			layoutAnchorPos.x = contentSize.w;
			layoutAnchorPos.y = 0;
			break;
		}
		case AnchorPointLocation::BottomLeft: {
			layoutAnchorPos.x = 0;
			layoutAnchorPos.y = contentSize.h;
			break;
		}
		case AnchorPointLocation::BottomRight: {
			layoutAnchorPos.x = contentSize.w;
			layoutAnchorPos.y = contentSize.h;
			break;
		}
		case AnchorPointLocation::Center: {
			layoutAnchorPos = contentSize / 2.f;
			break;
		}
		case AnchorPointLocation::LeftCenter: {
			layoutAnchorPos.x = 0;
			layoutAnchorPos.y = contentSize.h / 2.f;
			break;
		}
		case AnchorPointLocation::TopCenter: {
			layoutAnchorPos.x = contentSize.w / 2.f;
			layoutAnchorPos.y = 0;
			break;
		}
		case AnchorPointLocation::RightCenter: {
			layoutAnchorPos.x = contentSize.w;
			layoutAnchorPos.y = contentSize.h / 2.f;
			break;
		}
		case AnchorPointLocation::BottomCenter: {
			layoutAnchorPos.x = contentSize.w / 2.f;
			layoutAnchorPos.y = contentSize.h;
			break;
		}
		}

		UpdateChildPosition(cmp, point.cmpLocation, distance, layoutAnchorPos);
	}

	void AnchorPointLayout::UpdateChildPosition(BaseComponent* cmp, AnchorPointLocation cmpLocation, const Vec2& distance, const Vec2& layoutAnchorPos) const {
		switch (cmpLocation) {
		case AnchorPointLocation::TopLeft: {
			Vec2 pos = layoutAnchorPos + distance;
			cmp->SetPosition(pos);
			break;
		}
		case AnchorPointLocation::TopRight: {
			Vec2 pos = layoutAnchorPos + distance - Vec2(cmp->GetSize().w, 0);
			cmp->SetPosition(pos);
			break;
		}
		case AnchorPointLocation::BottomLeft: {
			Vec2 pos = layoutAnchorPos + distance - Vec2(0, cmp->GetSize().h);
			cmp->SetPosition(pos);
			break;
		}
		case AnchorPointLocation::BottomRight: {
			Vec2 pos = layoutAnchorPos + distance - cmp->GetSize();
			cmp->SetPosition(pos);
			break;
		}
		case AnchorPointLocation::Center: {
			Vec2 pos = layoutAnchorPos + distance - cmp->GetSize() / 2;
			cmp->SetPosition(pos);
			break;
		}
		case AnchorPointLocation::LeftCenter: {
			Vec2 pos = layoutAnchorPos + distance - Vec2(0, cmp->GetSize().h / 2);
			cmp->SetPosition(pos);
			break;
		}
		case AnchorPointLocation::TopCenter: {
			Vec2 pos = layoutAnchorPos + distance - Vec2(cmp->GetSize().w / 2, 0);
			cmp->SetPosition(pos);
			break;
		}
		case AnchorPointLocation::RightCenter: {
			Vec2 size = cmp->GetSize();
			Vec2 pos = layoutAnchorPos + distance - Vec2(size.w, size.h / 2);
			cmp->SetPosition(pos);
			break;
		}
		case AnchorPointLocation::BottomCenter: {
			Vec2 size = cmp->GetSize();
			Vec2 pos = layoutAnchorPos + distance - Vec2(size.w / 2, size.h);
			cmp->SetPosition(pos);
			break;
		}
		default:
			break;
		}
	}
}