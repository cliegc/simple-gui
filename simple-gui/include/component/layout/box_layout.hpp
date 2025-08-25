#pragma once
#include "layout.hpp"


namespace SimpleGui {
	class BoxLayout final : public Layout {
	public:
		BoxLayout(Direction direction);

		int GetSpacing() const;
		void SetSpacing(int spacing);

		Direction GetDirection() const;
		void SetDirection(Direction direction);

		Alignment GetAlignment() const;
		void SetAlignment(Alignment alignment);

		const std::vector<int>& GetWeights() const;
		void SetWeights(const std::vector<int>& weights);

		virtual void Update() override;

	private:
		Direction m_direction;
		Alignment m_alignment;
		int m_spacing;
		std::vector<int> m_weigths;

		void UpdateHorizontalDirection();
		void UpdateVerticalDirection();

		int GetVisibleChildrenCount() const;

	};
}