#pragma once
#include "base_component.hpp"
#include "label.hpp"
#include "scroll_panel.hpp"
#include "signal.hpp"


namespace SimpleGui {
	class ComboBox final : public BaseComponent {
	public:
		explicit ComboBox(const std::vector<std::string>& items = {});
		~ComboBox() override = default;

		bool HandleEvent(Event* event) override;
		void Update() override;
		void Render(Renderer& renderer) override;

		std::string GetCurrentItem() const { return m_masterItemLbl->GetText(); }
		std::string GetItem(size_t index) const;
		std::vector<std::string> GetItems() const;
		size_t GetItemCount() const;
		size_t GetCurrentItemIndex() const { return m_currIndex; }

		void SetCurrentItem(size_t index);
		void SetCurrentItem(const std::string& item);

		void AddItem(const std::string& item);
		void AddItems(const std::vector<std::string>& items);

		void RemoveItem(size_t index);
		void RemoveItem(const std::string& item);
		void ClearItems();

		float GetMaxItemsListHeight() const { return m_maxItemsListHeight; }
		void SetMaxItemsListHeight(float height) { m_maxItemsListHeight = height; }

		bool IsAutoHideItemsList() const { return m_autoHideItemsList; }
		void SetAutoHideItemsList(bool hide) { m_autoHideItemsList = hide; }

	public:
		// size_t index, const std::string& item
		Signal<size_t, const std::string&> currentItemChanged;

	protected:
		void EnteredComponentTree() override;

	private:
		std::vector<std::string> m_items;
		std::vector<Label*> m_itemLbls;
		std::unique_ptr<Label> m_masterItemLbl;
		std::unique_ptr<ScrollPanel> m_itemsPanel;
		ComponentElementRect m_toggleRect;
		size_t m_currIndex{};
		float m_maxItemsListHeight{};
		float m_maxItemsListWidth{};
		bool m_autoHideItemsList{ true };

		BaseComponent* m_lastHoveredLbl{};
		BaseComponent* m_hoveringLbl{};
		BaseComponent* m_lastSelectedLbl{};
		BaseComponent* m_selectedLbl{};

		void AddItem(const std::string& item, bool addToItems);

		bool HandleToggleItemsList(Event* event) const;
		bool HandleSelectItem(Event* event);
		void RenderToggleRect(Renderer& renderer);
		void RenderItemsList(Renderer& renderer);

		void SetSafePositionForItemList() const;
	};
}