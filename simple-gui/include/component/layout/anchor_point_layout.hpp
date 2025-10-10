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
		AnchorPointLocation layoutLocation{};
		AnchorPointLocation cmpLocation{};
		AnchorPointType type{};
		Vec2 distance;							// cmpLocation与layoutLocation之间的距离。AnchorPointType为Fixed时，该值表示像素距离；若为Percentage时，该值为布局宽高的缩放比例
	};

	class AnchorPointLayout : public Layout {
	public:
		AnchorPointLayout();
		~AnchorPointLayout() override = default;

		void SetAnchorPoint(BaseComponent* cmp, const AnchorPoint& point);
		void SetAnchorPoint(BaseComponent* cmp, AnchorPointType type, const Vec2& distance,
			AnchorPointLocation layoutLocation = AnchorPointLocation::TopLeft,
			AnchorPointLocation cmpLocation = AnchorPointLocation::TopLeft);
		
		void Update() override;

		std::unique_ptr<BaseComponent> RemoveChild(BaseComponent* cmp) override;
		std::unique_ptr<BaseComponent> RemoveChildDeferred(BaseComponent* cmp) override;

	private:
		std::unordered_map<BaseComponent*, AnchorPoint> m_anchorPoints;

		void UpdateAnchorPointLocation(BaseComponent* cmp, const AnchorPoint& point) const;
		void UpdateChildPosition(BaseComponent* cmp, AnchorPointLocation cmpLocation, const Vec2& distance, const Vec2& layoutAnchorPos) const;
	};
}