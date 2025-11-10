#pragma once
#include "layout.hpp"


namespace SimpleGui {
    class SplitLayout final : public Layout {
    public:
        explicit SplitLayout(Direction direction = Direction::Horizontal);
        ~SplitLayout() override = default;

        bool HandleEvent(Event *event) override;
        void Update() override;
        void Render(Renderer &renderer) override;

        Direction GetDirection() const { return m_direction; }
        void SetDirection(Direction direction) { m_direction = direction; }

        std::vector<int> GetWeights() const { return m_weights; }
        void SetWeights(const std::vector<int>& weights);

    private:
        struct SplitHandle final {
            ComponentElementRect rect{};
            float toggleRange{ 2.5f };
            bool draw{ true };
            bool visible{ true };
        };

        Direction m_direction;
        SplitHandle m_handle{};
        std::vector<int> m_weights{};

        void UpdateHorizontalDirection();
        void UpdateVerticalDirection();
    };
}