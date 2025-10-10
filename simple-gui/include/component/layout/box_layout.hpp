#pragma once
#include "layout.hpp"


namespace SimpleGui {
	class BoxLayout final : public Layout {
	public:
		explicit BoxLayout(Direction direction);

		int GetSpacing() const;
		void SetSpacing(int spacing);

		Direction GetDirection() const;
		void SetDirection(Direction direction);

		Alignment GetAlignment() const;
		void SetAlignment(Alignment alignment);

		const std::vector<int>& GetWeights() const;
		void SetWeights(const std::vector<int>& weights);

		void Update() override;

	private:
		Direction m_direction;
		Alignment m_alignment;
		int m_spacing;
		std::vector<int> m_weights;

		void UpdateHorizontalDirection() const;
		void UpdateVerticalDirection() const;

		int GetVisibleChildrenCount() const;

	};
}