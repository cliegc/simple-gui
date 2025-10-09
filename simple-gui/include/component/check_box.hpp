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

		inline static std::shared_ptr<CheckBoxGroup> Create() {
			return std::move(std::make_shared<CheckBoxGroup>());
		}

		inline bool IsUniqueCheck() const { return m_uniqueCheck; }
		void SetUniqueCheck(bool value);

		inline const std::vector<CheckBox*>& GetCheckBoxes() const { return m_boxes; }

	public:
		Signal<CheckBox*> checkStateChanged;

	private:
		std::vector<CheckBox*> m_boxes;
		bool m_uniqueCheck{};
	};

	class CheckBox final : public BaseComponent {
	public:
		CheckBox(std::string_view text);
		~CheckBox() = default;

		virtual bool HandleEvent(Event* event) override;
		virtual void Update() override;
		virtual void Render(Renderer& renderer) override;

		inline bool IsChecked() const { return m_checked; }
		inline void SetChecked(bool value);

		inline std::string GetText() const { return m_textLbl->GetText(); }
		inline void SetText(std::string_view text) { m_textLbl->SetText(text); }

		void SetGoup(std::shared_ptr<CheckBoxGroup> group);

	public:
		Signal<bool> checkStateChanged;

	protected:
		virtual void EnteredComponentTree() override;

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