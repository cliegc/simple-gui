#pragma once
#include "base_component.hpp"
#include "label.hpp"
#include "signal.hpp"


namespace SimpleGui {
	class ComboBox final : public BaseComponent {
	public:
		explicit ComboBox(const std::vector<std::string>& items = {});
		~ComboBox() override = default;

		bool HandleEvent(Event* event) override;
		void Update() override;
		void Render(Renderer& renderer) override;

		std::string GetCurrentItem() const { return m_currItemLbl->GetText(); }
		std::string GetItem(size_t index) const;
		std::vector<std::string> GetItems() const;

		void SetCurrentItem(size_t index);
		void SetCurrentItem(std::string_view item);

		void AddItem(std::string_view item);
		void AddItems(const std::vector<std::string>& items);

		void RemoveItem(size_t index);
		void RemoveItem(std::string_view item);
		void ClearItems();

	public:
		// size_t index, const std::string& item
		Signal<size_t, const std::string&> currentItemChanged;

	protected:
		void EnteredComponentTree() override;

	private:
		std::vector<std::unique_ptr<Label>> m_lbls;
		std::unique_ptr<Label> m_currItemLbl;
		ComponentElementRect m_toggleRect;
		ComponentElementRect m_itemsRect;
		bool m_showItemsList{};

		// constexpr float m_maxItemsWidth =

		void CreateItemLabel(std::string_view item);
		bool HandleToggleItemsList(Event* event);
		bool HandleSelectItem(Event* event);
		void UpdateItemsList();
		void RenderItemsList(Renderer& renderer);
	};
}