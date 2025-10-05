#include "component/common.hpp"
#include "gui_manager.hpp"

#ifdef _WIN32
#include <Windows.h>
#include <Imm.h>
#pragma comment(lib, "Imm32.lib")
#endif // _WIN32


namespace SimpleGui {
#pragma region Caret
	Caret::Caret(): 
		m_color(Color::WHITE), m_blink(false), m_blinkFlag(true), m_visible(true) {
		m_gRect.size.w = 1;
		m_timer = std::make_unique<Timer>(0.65f);
		m_timer->timeout.Connect("SG_CARET_TIMER_on_timeout",
			[this]() {
				m_blinkFlag = !m_blinkFlag;
			});

		if (m_blink) m_timer->Start();
	}

	void Caret::Update() const {
		m_timer->Update();
	}

	void Caret::Render(const Renderer& renderer) const {
		if (!m_visible || !m_blinkFlag) return;
		renderer.FillRect(m_gRect, m_color);
	}
#pragma endregion

#pragma region IMEUtils
	void IMEUtils::SetIMECandidateWindowPosition(const Window& win, const Vec2& pos) {
#ifdef _WIN32
		SDL_PropertiesID props = SDL_GetWindowProperties(&win.GetSDLWindow());
		// 获取 HWND 指针
		HWND hwnd = (HWND)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
		if (!hwnd) {
			SDL_Log("无法获取 HWND: %s", SDL_GetError());
			return;
		}

		HIMC hImc = ImmGetContext(hwnd);
		if (hImc) {
			CANDIDATEFORM cf;
			cf.dwIndex = 0;
			cf.dwStyle = CFS_CANDIDATEPOS;
			cf.ptCurrentPos.x = static_cast<long>(pos.x);
			cf.ptCurrentPos.y = static_cast<long>(pos.y);
			cf.rcArea.left = 0;
			cf.rcArea.top = 0;
			cf.rcArea.right = 0;
			cf.rcArea.bottom = 0;

			ImmSetCandidateWindow(hImc, &cf);
			ImmReleaseContext(hwnd, hImc);
		}
#endif // _WIN32
	}

	void IMEUtils::SetIMECompositionWindowPosition(const Window& win, const Vec2& pos) {
#ifdef _WIN32
		SDL_PropertiesID props = SDL_GetWindowProperties(&win.GetSDLWindow());
		// 获取 HWND 指针
		HWND hwnd = (HWND)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
		if (!hwnd) {
			SDL_Log("无法获取 HWND: %s", SDL_GetError());
			return;
		}

		HIMC hImc = ImmGetContext(hwnd);
		if (hImc) {
			COMPOSITIONFORM cf;
			cf.dwStyle = CFS_POINT;
			cf.ptCurrentPos.x = static_cast<long>(pos.x);
			cf.ptCurrentPos.y = static_cast<long>(pos.y);
			cf.rcArea.left = 0;
			cf.rcArea.top = 0;
			cf.rcArea.right = 0;
			cf.rcArea.bottom = 0;

			ImmSetCompositionWindow(hImc, &cf);
			ImmReleaseContext(hwnd, hImc);
		}
	}
#endif // _WIN32
#pragma endregion

#pragma region Range
	Range::Range(float value, float minValue, float maxValue) {
		SetMinValue(minValue);
		SetMaxValue(maxValue);
		SetValue(value);
	}

	void Range::SetValue(float value) {
		float old = m_value;
		value = SDL_clamp(value, m_minValue, m_maxValue);
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
	
#pragma endregion
}