#pragma once


namespace SimpleGui {
	struct Vec2;
	class Window;

	class IMEUtils final {
	public:
		static void SetIMECandidateWindowPosition(const Window& win, const Vec2& pos);
		static void SetIMECompositionWindowPosition(const Window& win, const Vec2& pos);
	};

}