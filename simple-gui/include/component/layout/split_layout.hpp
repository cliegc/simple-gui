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
        struct Splitter final {
            ComponentElementRect rect{};
            float toggleRange{ 2.5f };
            bool draw{ true };
            bool visible{ true };
        };

        struct DragSplitterData final {
            Vec2 startMousePos;
            Vec2 startSplitterPos;
            bool canDrag{ false };
            bool dragging{ false };
        };

        Direction m_direction;
        Splitter m_splitter{};
        DragSplitterData m_dragSplitterData{};
        std::vector<int> m_weights{};
        bool m_needUpdateWeights{ true };

        void UpdateHorizontalDirection();
        void UpdateVerticalDirection();
        void UpdateWeights(Direction direction);
        void HandleDragSplitter(Event* event, Direction direction);
    };
}