#pragma once
#include "base_component.hpp"
#include "label.hpp"
#include "signal.hpp"


namespace SimpleGui {
	enum class ProgressFillMode {
		LeftToRight,
		RightToLeft,
		BottomToTop,
		TopToBottom,
		CenterH,
		CenterV,
	};

	class ProgressBar final : public BaseComponent {
	public:
		ProgressBar(float value = 0.f, float minValue = 0.f, float maxValue = 100.f);
		~ProgressBar() = default;

		virtual void Update() override;
		virtual void Render(const Renderer& renderer) override;

		inline float GetValue() const { return m_value; }
		void SetValue(float value);

		inline float GetMinValue() const { return m_minValue; }
		void SetMinValue(float value);

		inline float GetMaxValue() const { return m_maxValue; }
		void SetMaxValue(float value);

		inline ProgressFillMode GetProgressFillMode() const { return m_fillMode; }
		inline void SetProgressFillMode(ProgressFillMode mode) { m_fillMode = mode; }

		inline bool IsShowProgressText() const { return m_showProgress; }
		inline void ShowProgressText(bool show) { m_showProgress = show; }

		inline bool IsIndeterminate() const { return m_indeterminate; }
		inline void SetIndeterminate(bool value) {
			m_indeterminate = value;
			m_progressLbl->SetVisible(!m_indeterminate);
		}

	public:
		Signal<float> valueChanged;
		Signal<float> minValueChanged;
		Signal<float> maxValueChanged;

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
		float m_value{};
		float m_minValue{};
		float m_maxValue{};
		
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