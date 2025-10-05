#include "component/common/range.hpp"
#include "math.hpp"


namespace SimpleGui {
	Range::Range(float value, float minValue, float maxValue) {
		SetMinValue(minValue);
		SetMaxValue(maxValue);
		SetValue(value);
	}

	void Range::SetValue(float value) {
		float old = m_value;
		value = Clamp(value, m_minValue, m_maxValue);
		m_value = value;
		if (!IsEqualApprox(m_value, old)) valueChanged.Emit(m_value);
	}

	void Range::SetMinValue(float value) {
		if (value > m_value) m_value = value;
		if (value > m_maxValue) m_maxValue = value;
		float old = m_minValue;
		m_minValue = value;
		if (!IsEqualApprox(m_minValue, old)) minValueChanged.Emit(m_minValue);
	}

	void Range::SetMaxValue(float value) {
		if (value < m_value) m_value = value;
		if (value < m_minValue) m_minValue = value;
		float old = m_maxValue;
		m_maxValue = value;
		if (!IsEqualApprox(m_maxValue, old)) maxValueChanged.Emit(m_maxValue);
	}
}