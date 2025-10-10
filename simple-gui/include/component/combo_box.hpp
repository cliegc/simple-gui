#pragma once
#include "base_component.hpp"
#include "label.hpp"
#include "signal.hpp"


namespace SimpleGui {
	class ComboBox final : public BaseComponent {
	public:
		ComboBox(const std::vector<std::string>& items);
		~ComboBox() = default;

		virtual bool HandleEvent(Event* event) override;
		virtual void Update() override;
		virtual void Render(Renderer& renderer) override;

		inline std::string GetCurrentItem() const { return m_currItemLbl->GetText(); }
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
		virtual void EnteredComponentTree() override;

	private:
		std::vector<std::unique_ptr<Label>> m_lbls;
		std::unique_ptr<Label> m_currItemLbl;
		ComponentElementRect m_toggleRect;
		ComponentElementRect m_itemsRect;
		bool m_showItemsList{};

	private:
		bool HandleToggleItemsList(Event* event);
		bool HandleSelectItem(Event* event);
		void UpdateItemsList();
		void RenderItemsList(Renderer& renderer);
	};
}