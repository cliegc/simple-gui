#pragma once
#include "base_component.hpp"
#include "label.hpp"
#include "signal.hpp"


namespace SimpleGui {
	class CheckBox;

	class CheckBoxGroup final {
		friend class CheckBox;
	public:
		CheckBoxGroup() = default;
		~CheckBoxGroup() = default;

		static std::shared_ptr<CheckBoxGroup> Create() {
			return std::make_shared<CheckBoxGroup>();
		}

		bool IsUniqueCheck() const { return m_uniqueCheck; }
		void SetUniqueCheck(bool value);

		const std::vector<CheckBox*>& GetCheckBoxes() const { return m_boxes; }

	public:
		Signal<CheckBox*> checkStateChanged;

	private:
		std::vector<CheckBox*> m_boxes;
		bool m_uniqueCheck{};
	};

	class CheckBox final : public BaseComponent {
	public:
		explicit CheckBox(std::string_view text = "check box");
		~CheckBox() override = default;

		bool HandleEvent(Event* event) override;
		void Update() override;
		void Render(Renderer& renderer) override;

		bool IsChecked() const { return m_checked; }
		inline void SetChecked(bool value);

		std::string GetText() const { return m_textLbl->GetText(); }
		void SetText(std::string_view text) const { m_textLbl->SetText(text); }

		void SetGroup(const std::shared_ptr<CheckBoxGroup>& group);

	public:
		Signal<bool> checkStateChanged;

	protected:
		void EnteredComponentTree() override;

	private:
		bool m_checked;
		ComponentElementRect m_checkRect{};
		ComponentElementRect m_checkedFlagRect{};
		std::unique_ptr<Label> m_textLbl;
		std::shared_ptr<CheckBoxGroup> m_group;

	private:
		bool HandleMouseClicked(Event* event);
	};
}