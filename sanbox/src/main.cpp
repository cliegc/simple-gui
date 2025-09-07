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
		auto lbl = (Label*)m_target;
		lbl->SetText(std::format("FPS: {:.2f}", SG_GuiManager.GetRealFrameRate()));
	}
};

class DisplayDeltaForLabel : public ExtendedFunctions {
protected:
	virtual void Update() override {
		auto lbl = (Label*)m_target;
		lbl->SetText(std::format("delta: {:.4f}", SG_GuiManager.GetDelta()));
	}
};

class TestFrameRateControllerFunctions : public ExtendedFunctions {
protected:
	Vec2 circlePos{ 0.f, 100.f };
	int direction = 1;

	virtual void Update() override {
		circlePos.x += 100 * direction * SG_GuiManager.GetDelta();
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

class DrawBackgroundFunctions : public ExtendedFunctions {
public:
	DrawBackgroundFunctions(std::shared_ptr<Texture> texture) {
		m_texture = texture;
	}

	~DrawBackgroundFunctions() = default;

protected:
	virtual void Render(const Renderer& renderer) override {
		Rect srcRect(0, 0, m_texture->GetWidth(), m_texture->GetHeight());
		renderer.DrawTexture(*m_texture.get(), srcRect, m_target->GetGlobalRect(), 0, srcRect.Center(), SDL_FLIP_NONE);
		renderer.FillRect(m_target->GetGlobalRect(), Color(0, 0, 0, 200));
	}

private:
	std::shared_ptr<Texture> m_texture;
};

int main(int argc, char** argv) {
	 GuiManager::Init(argc, argv, "C:\\WINDOWS\\Fonts\\simhei.ttf");
	 Window& win = SG_GuiManager.GetWindow("win1-60fps", 640, 480);

	 win.SetPosition(100, 200);
	 win.GetRootComponent().AddExtendedFunctions<DrawBackgroundFunctions>(win.GetRenderer().CreateSharedTexture("C:\\Users\\endif\\Desktop\\jinzi.png"));
	 win.GetRootComponent().AddExtendedFunctions<TestFrameRateControllerFunctions>();

	 win.AddComponent<Label>("")->AddExtendedFunctions<DisplayFPSForLabel>();
	 auto lbl2 = win.AddComponent<Label>("");
	 lbl2->AddExtendedFunctions<DisplayDeltaForLabel>();
	 lbl2->SetPositionY(100);

	 //win.EnableVsync(true);
	 SG_GuiManager.SetTargetFrameRate(60);
	 //SG_GuiManager.SetUnlimitedFrameRate(true);
	 SG_GuiManager.Run();
	 GuiManager::Quit();
	
	return 0;
}
