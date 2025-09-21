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


static void TestScrollBar() {
	auto draggablePanel = SG_GuiManager.GetWindow().AddComponent<DraggablePanel>("test scrollbar");
	draggablePanel->SetSize(200, 200);
	draggablePanel->AddChild<Label>("this is a lebel")->SetPositionX(-50);
	draggablePanel->AddChild<Button>("this is a button")->SetPosition(180, 100);

	for (int i = 0; i < 50; ++i) {
		auto btn = draggablePanel->AddChild<Button>(std::format("button {}", i));
		btn->clicked.Connect("on_clicked",
			[i]() {
				SDL_Log("click button: %d\n", i);
			});
		btn->SetPosition(
			SDL_randf() * 800 - 300,
			SDL_randf() * 800 - 300
		);
	}

	auto v_scrollbar = SG_GuiManager.GetWindow().AddComponent<ScrollBar>(Direction::Vertical);
	v_scrollbar->SetPosition(200, 350);
	v_scrollbar->SetMouseWheelDelta(10.f);
	v_scrollbar->SetTarget(draggablePanel);

	auto h_scrollbar = SG_GuiManager.GetWindow().AddComponent<ScrollBar>(Direction::Horizontal);
	h_scrollbar->SetPosition(200, 320);
	h_scrollbar->SetMouseWheelDelta(10.f);
	h_scrollbar->SetTarget(draggablePanel);
}


static void TestScrollPanel() {
	auto draggablePanel = SG_GuiManager.GetWindow().AddComponent<DraggablePanel>("test scroll panel");
	draggablePanel->SetSize(300, 300);

	auto scrollPanel = draggablePanel->AddChild<ScrollPanel>();
	scrollPanel->SetSizeConfigs(ComponentSizeConfig::Expanding, ComponentSizeConfig::Expanding);

	for (int i = 0; i < 500; ++i) {
		auto btn = scrollPanel->AddChild<Button>(std::format("button {}", i));
		btn->clicked.Connect("on_clicked",
			[i]() {
				SDL_Log("click button: %d\n", i);
			});
		btn->SetPosition(
			SDL_randf() * 800 - 300,
			SDL_randf() * 800 - 300
		);
	}

	auto btn = SG_GuiManager.GetWindow().AddComponent<Button>("add button");
	btn->SetPosition(100, 400);
	btn->clicked.Connect("on_clicked",
		[scrollPanel]() {
			auto btn = scrollPanel->AddChildDeferred<Button>(std::format("button {}", scrollPanel->GetChildrenCount()));
			btn->SetPosition(
				SDL_randf() * 800 - 300,
				SDL_randf() * 800 - 300
			);
		});
	
	auto btn2 = SG_GuiManager.GetWindow().AddComponent<Button>("clear all");
	btn2->SetPosition(100, 450);
	btn2->clicked.Connect("on_clicked", 
		[scrollPanel]() {
			scrollPanel->ClearAllChildrenDeferred();
		});

	//scrollPanel->CustomThemeColor(ThemeColorFlags::LabelForeground, Color::RED);
	//SG_GuiManager.GetWindow().GetRootComponent().CustomThemeColor(ThemeColorFlags::ScrollPanelBackground, Color::YELLOW);
	//SG_GuiManager.GetWindow().GetRootComponent().CustomThemeColor(ThemeColorFlags::ScrollbarSlider_H, Color::YELLOW);
	//scrollPanel->CustomThemeColor(ThemeColorFlags::ScrollbarSlider_H, Color::BLUE);
}

int main(int argc, char** argv) {
	GuiManager::Init(argc, argv, "C:\\WINDOWS\\Fonts\\simhei.ttf");
	Window& win = SG_GuiManager.GetWindow("win1-60fps", 640, 480);
	//win.SwitchStyle(StyleManager::LightStyle);

	//win.GetRootComponent().AddExtendedFunctions<DrawBackgroundFunctions>(win.GetRenderer().CreateSharedTexture("C:\\Users\\endif\\Desktop\\jinzi.png"));
	//win.GetRootComponent().AddExtendedFunctions<TestFrameRateControllerFunctions>();

	win.AddComponent<Label>("")->AddExtendedFunctions<DisplayFPSForLabel>();
	auto lbl2 = win.AddComponent<Label>("");
	lbl2->AddExtendedFunctions<DisplayDeltaForLabel>();
	lbl2->SetPositionY(100);

	//TestScrollBar();
	TestScrollPanel();

	win.EnableVsync(true);
	//SG_GuiManager.SetTargetFrameRate(165);
	//SG_GuiManager.SetUnlimitedFrameRate(true);
	SG_GuiManager.Run();
	GuiManager::Quit();

	return 0;
}
