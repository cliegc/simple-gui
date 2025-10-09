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
		ProgressBar(float value = 0.f, float minValue = 0.f, float maxValue = 100.f);
		~ProgressBar() = default;

		virtual void Update() override;
		virtual void Render(Renderer& renderer) override;

		virtual void SetValue(float value) override;

		inline ProgressFillMode GetProgressFillMode() const { return m_fillMode; }
		inline void SetProgressFillMode(ProgressFillMode mode) { m_fillMode = mode; }

		inline bool IsShowProgressText() const { return m_showProgress; }
		inline void ShowProgressText(bool show) { m_showProgress = show; }

		inline bool IsIndeterminate() const { return m_indeterminate; }
		inline void SetIndeterminate(bool value) {
			m_indeterminate = value;
			m_indeterminateProgressData.delta = 0;
			m_progressLbl->SetVisible(!m_indeterminate);
		}

	protected:
		virtual void EnteredComponentTree() override;

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

		IndeterminateProgressData m_indeterminateProgressData;

		std::unique_ptr<Label> m_progressLbl;

	private:
		void UpdateProgressGlobalRect(const Rect& contentGRect);
		void UpdateIndeterminateProgressGlobalRect(const Rect& contentGRect);
	};
}