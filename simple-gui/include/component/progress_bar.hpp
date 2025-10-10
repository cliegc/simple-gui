#pragma once
#include "base_component.hpp"
#include "label.hpp"
#include "common/range.hpp"


namespace SimpleGui {
	enum class ProgressFillMode {
		LeftToRight,
		RightToLeft,
		BottomToTop,
		TopToBottom,
		CenterH,
		CenterV,
	};

	class ProgressBar final : public BaseComponent, public Range {
	public:
		explicit ProgressBar(float value = 0.f, float minValue = 0.f, float maxValue = 100.f);
		~ProgressBar() override = default;

		void Update() override;
		void Render(Renderer& renderer) override;

		void SetValue(float value) override;

		ProgressFillMode GetProgressFillMode() const { return m_fillMode; }
		void SetProgressFillMode(ProgressFillMode mode) { m_fillMode = mode; }

		bool IsShowProgressText() const { return m_showProgress; }
		void ShowProgressText(bool show) { m_showProgress = show; }

		bool IsIndeterminate() const { return m_indeterminate; }
		void SetIndeterminate(bool value) {
			m_indeterminate = value;
			m_indeterminateProgressData.delta = 0;
			m_progressLbl->SetVisible(!m_indeterminate);
		}

	protected:
		void EnteredComponentTree() override;

	private:
		struct IndeterminateProgressData final {
			float delta;
			float speed;
			float lengthScale;
			float minLength;
		};

	private:
		Rect m_currProgressGRect{};
		ProgressFillMode m_fillMode{};
		bool m_showProgress{};
		bool m_indeterminate{};

		IndeterminateProgressData m_indeterminateProgressData{};

		std::unique_ptr<Label> m_progressLbl;

	private:
		void UpdateProgressGlobalRect(const Rect& contentGRect);
		void UpdateIndeterminateProgressGlobalRect(const Rect& contentGRect);
	};
}