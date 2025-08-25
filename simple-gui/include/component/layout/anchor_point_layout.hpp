#pragma once
#include "layout.hpp"


namespace SimpleGui {
	enum class AnchorPointLocation {
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		Center,
		LeftCenter,
		TopCenter,
		RightCenter,
		BottomCenter,
	};

	enum class AnchorPointType {
		Fixed,
		Percentage,
	};

	struct AnchorPoint final {
		AnchorPointLocation layoutLocation;
		AnchorPointLocation cmpLocation;
		AnchorPointType type;
		Vec2 distance;							// cmpLocation与layoutLocation之间的距离。AnchorPointType为Fixed时，该值表示像素距离；若为Percentage时，该值为布局宽高的缩放比例
	};

	class AnchorPointLayout : public Layout {
	public:
		AnchorPointLayout();
		virtual ~AnchorPointLayout() = default;

		void SetAnchorPoint(BaseComponent* cmp, const AnchorPoint& point);
		void SetAnchorPoint(BaseComponent* cmp, AnchorPointType type, const Vec2& distance,
			AnchorPointLocation layoutLocation = AnchorPointLocation::TopLeft,
			AnchorPointLocation cmpLocation = AnchorPointLocation::TopLeft);
		
		virtual void Update() override;

	private:
		// TODO 删除m_anchorPoints中不存在的子组件对应的键值对
		std::unordered_map<BaseComponent*, AnchorPoint> m_anchorPoints;

		void UpdateAnchorPointLocation(BaseComponent* cmp, const AnchorPoint& point);
		void UpdateChildPosition(BaseComponent* cmp, AnchorPointLocation cmpLocation, const Vec2& distance, const Vec2& layoutAnchorPos) const;
	};
}