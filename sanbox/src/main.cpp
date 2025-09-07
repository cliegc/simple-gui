#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <SDL3_image/SDL_image.h>
#include <format>
#include <simple_gui.hpp>
#include <style.hpp>

using namespace SimpleGui;

class DisplayFPSForLabel : public ExtendedFunctions {
protected:
	virtual void Update() override {
		Label* lbl = (Label*)m_target;
		lbl->SetText(std::format("FPS: {:.2f}", lbl->GetWindow()->GetRealFrameRate()));
	}
};

class TestFrameRateControllerFunctions : public ExtendedFunctions {
protected:
	// test FrameRateController
	Vec2 circlePos{ 0.f, 100.f };
	int direction = 1;

	virtual void Update() override {
		// test FrameRateController
		//circlePos.x += 100 * direction * m_fpsController.m_delta;
		circlePos.x += 10 * direction;
		if (circlePos.x <= 0) {
			circlePos.x = 0;
			direction = 1;
		}
		else if (circlePos.x >= m_target->GetSize().w) {
			circlePos.x = m_target->GetSize().w;
			direction = -1;
		}
	}

	virtual void Render(const Renderer& renderer) override {
		renderer.FillCircle(circlePos, 50, Color::GREEN);
	}
};


int main(int argc, char** argv) {
	 GuiManager::Init(argc, argv, "C:\\WINDOWS\\Fonts\\simhei.ttf");
	 Window& win = SG_GuiManager.CreateWindow("win1-60fps", 640, 480);
	 Window& win2 = SG_GuiManager.CreateWindow("win2-30fps", 640, 480);
	 //win2.SetTargetFrameRate(30);

	 //win2.EnableVsync(true);

	 win.SetPosition(100, 200);
	 win2.SetPosition(800, 200);

	 win.GetRootComponent().AddExtendedFunctions<TestFrameRateControllerFunctions>();
	 win2.GetRootComponent().AddExtendedFunctions<TestFrameRateControllerFunctions>();

	 auto lbl1 = win.AddComponent<Label>("FPS: 60");
	 lbl1->AddExtendedFunctions<DisplayFPSForLabel>();
	 lbl1->RemoveExtendedFunctions<DisplayFPSForLabel>();

	 win2.AddComponent<Label>("FPS: 30")->AddExtendedFunctions<DisplayFPSForLabel>();

	 //win.EnableVsync(true);
	 SDL_Log("win title: %s\n", win.GetTitle().c_str());
	 SG_GuiManager.Run();
	 GuiManager::Quit();
	
	return 0;
}
