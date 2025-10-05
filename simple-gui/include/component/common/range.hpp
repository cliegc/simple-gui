#pragma once
#include "signal.hpp"


namespace SimpleGui {
	class Range {
	public:
		Range(float value, float minValue, float maxValue);
		virtual ~Range() = default;

		inline float GetValue() const { return m_value; }
		virtual void SetValue(float value);

		inline float GetMinValue() const { return m_minValue; }
		virtual void SetMinValue(float value);

		inline float GetMaxValue() const { return m_maxValue; }
		virtual void SetMaxValue(float value);

		inline float GetInterval() const { return m_maxValue - m_minValue; }
		inline float GetValueToMinValueInterval() const { return m_value - m_minValue; }
		inline float GetMaxValueToValueInterval() const { return m_maxValue - m_value; }

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