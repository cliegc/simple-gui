#pragma once
#include "signal.hpp"


namespace SimpleGui {
	class Range {
	public:
		Range(float value, float minValue, float maxValue);
		virtual ~Range() = default;

		float GetValue() const { return m_value; }
		virtual void SetValue(float value);

		float GetMinValue() const { return m_minValue; }
		virtual void SetMinValue(float value);

		float GetMaxValue() const { return m_maxValue; }
		virtual void SetMaxValue(float value);

		float GetInterval() const { return m_maxValue - m_minValue; }
		float GetValueToMinValueInterval() const { return m_value - m_minValue; }
		float GetMaxValueToValueInterval() const { return m_maxValue - m_value; }

	public:
		Signal<float> valueChanged;
		Signal<float> minValueChanged;
		Signal<float> maxValueChanged;

	private:
		float m_value;
		float m_minValue;
		float m_maxValue;
	};
}