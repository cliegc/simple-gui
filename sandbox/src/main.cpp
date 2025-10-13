#include <format>
#include <simple_gui.hpp>


using namespace SimpleGui;


class DisplayFPSForLabel final : public ExtendedFunctions {
protected:
	void Update() override {
		auto lbl = static_cast<Label *>(m_target);
		lbl->SetText(std::format("FPS: {:.2f}", SG_GuiManager.GetRealFrameRate()));
	}
};

class DisplayDeltaForLabel final : public ExtendedFunctions {
protected:
	void Update() override {
		auto lbl = static_cast<Label *>(m_target);
		lbl->SetText(std::format("delta: {:.4f}", SG_GuiManager.GetDelta()));
	}
};

class TestFrameRateControllerFunctions final : public ExtendedFunctions {
protected:
	Vec2 circlePos{0.f, 100.f};
	int direction = 1;

	void Update() override {
		circlePos.x += 100.f * direction * SG_GuiManager.GetDelta();
		if (circlePos.x <= 0) {
			circlePos.x = 0;
			direction = 1;
		} else if (circlePos.x >= m_target->GetSize().w) {
			circlePos.x = m_target->GetSize().w;
			direction = -1;
		}
	}

	void Render(Renderer &renderer) override {
		//renderer.FillCircle(circlePos, 50, Color::GREEN);
		renderer.SetTopRender(true);
		renderer.RenderCircle(circlePos, 50, Color::GREEN, true);
		renderer.SetTopRender(false);
	}
};

class DrawBackgroundFunctions final : public ExtendedFunctions {
public:
	explicit DrawBackgroundFunctions(const std::shared_ptr<Texture> &texture) {
		m_texture = texture;
	}

	~DrawBackgroundFunctions() override = default;

protected:
	void Render(Renderer &renderer) override {
		Rect srcRect(0, 0, m_texture->GetWidth(), m_texture->GetHeight());
		//renderer.DrawTexture(*m_texture.get(), srcRect, m_target->GetGlobalRect(), 0, srcRect.Center(), SDL_FLIP_NONE);
		renderer.RenderTexture(m_texture.get(), srcRect,
		                       m_target->GetGlobalRect(), 0, srcRect.Center(), SDL_FLIP_NONE);
		//renderer.FillRect(m_target->GetGlobalRect(), Color(0, 0, 0, 200));
		renderer.RenderRect(m_target->GetGlobalRect(), Color(0, 0, 0, 200), true);
	}

private:
	std::shared_ptr<Texture> m_texture;
};


static void TestScrollBar() {
	auto draggablePanel = SG_GuiManager.GetWindow().AddComponent<DraggablePanel>("test scrollbar");
	draggablePanel->SetSize(400, 400);

	auto dp2 = draggablePanel->AddChild<DraggablePanel>("test scrollbar");
	dp2->SetSize(200, 200);

	for (int i = 0; i < 50; ++i) {
		auto btn = dp2->AddChild<Button>(std::format("button {}", i));
		btn->clicked.Connect("on_clicked",
		                     [i]() {
		                     	SG_INFO("click button: {}", i);
		                     });
		btn->SetPosition(
			SDL_randf() * 800 - 300,
			SDL_randf() * 800 - 300
		);
	}

	auto v_scrollbar = draggablePanel->AddChild<ScrollBar>(Direction::Vertical);
	v_scrollbar->SetMouseWheelDelta(10.f);
	v_scrollbar->SetTarget(dp2);

	auto h_scrollbar = draggablePanel->AddChild<ScrollBar>(Direction::Horizontal);
	h_scrollbar->SetPosition(50, 0);
	h_scrollbar->SetMouseWheelDelta(10.f);
	h_scrollbar->SetTarget(dp2);
}


static void TestScrollPanel() {
	auto draggablePanel = SG_GuiManager.GetWindow().AddComponent<DraggablePanel>("test scroll panel");
	draggablePanel->SetSize(400, 400);

	auto dp2 = draggablePanel->AddChild<DraggablePanel>("button count: 0");
	dp2->SetSize(200, 200);

	auto scrollPanel = dp2->AddChild<ScrollPanel>();
	scrollPanel->SetSizeConfigs(ComponentSizeConfig::Expanding, ComponentSizeConfig::Expanding);

	auto btn = draggablePanel->AddChild<Button>("add button");
	btn->clicked.Connect("on_clicked",
	                     [dp2, scrollPanel]() {
		                     auto btn = scrollPanel->AddChildDeferred<Button>(
			                     std::format("button {}", scrollPanel->GetChildrenCount()));
		                     btn->SetPosition(
			                     SDL_randf() * 800 - 300,
			                     SDL_randf() * 800 - 300
		                     );

		                     dp2->SetTitle(std::format("button count: {}", scrollPanel->GetChildrenCount()));
	                     });

	auto btn2 = draggablePanel->AddChild<Button>("clear all");
	btn2->SetPosition(0, 100);
	btn2->clicked.Connect("on_clicked",
	                      [dp2, scrollPanel]() {
		                      scrollPanel->ClearAllChildrenDeferred();
		                      dp2->SetTitle(std::format("button count: 0"));
	                      });
}


static void TestTimer() {
	auto dp = SG_GuiManager.GetWindow().AddComponent<DraggablePanel>("test timer");

	auto timer = SG_GuiManager.GetTimer(1.f);
	auto btn = dp->AddChild<Button>("timer: running");
	auto btn2 = dp->AddChild<Button>("kill timer");
	btn2->SetPosition(0, 100);

	timer->timeout.Connect("on_timeout_change_btn_color",
	                       [dp]() {
		                       Color color(
			                       SDL_randf() * 255,
			                       SDL_randf() * 255,
			                       SDL_randf() * 255
		                       );
		                       dp->CustomThemeColor(ThemeColorFlags::DraggablePanelBackground, color);
	                       });

	btn->clicked.Connect("on_clicked",
	                     [timer, btn]() {
		                     timer->SetPaused(!timer->IsPaused());
		                     btn->SetText(std::format("timer: {}", timer->IsPaused() ? "paused" : "running"));
	                     });

	btn2->clicked.Connect("on_clicked",
	                      [timer]() {
		                      if (timer)
			                      SG_GuiManager.KillTimer(timer);
		                      // else SDL_Log("btn clicked: delete timer");
		                      else SG_INFO("btn clicked: delete timer");
	                      });


	timer->Start();
}


static void TestProgressBar() {
	auto draggablePanel = SG_GuiManager.GetWindow().AddComponent<DraggablePanel>("test progress bar");
	draggablePanel->SetSize(300, 300);

	auto layout = draggablePanel->AddChild<AnchorPointLayout>();
	layout->SetSizeConfigs(ComponentSizeConfig::Expanding, ComponentSizeConfig::Expanding);

	auto bar = draggablePanel->AddChild<ProgressBar>(0, 0, 100);
	bar->SetSize(200, 20);
	bar->ShowProgressText(true);
	//bar->SetIndeterminate(true);

	layout->SetAnchorPoint(bar, AnchorPointType::Fixed, Vec2(10, 10));

	//bar->SetProgressFillMode(ProgressFillMode::RightToLeft);
	auto btn = layout->AddChild<Button>("switch fill mode");
	//btn->SetPosition(500, 400);
	layout->SetAnchorPoint(btn, AnchorPointType::Fixed, Vec2(10, -10), AnchorPointLocation::BottomLeft,
	                       AnchorPointLocation::BottomLeft);

	static int count = 0;
	btn->clicked.Connect("on_clicked_switch_fill_mode",
	                     [bar]() {
		                     auto mode = static_cast<ProgressFillMode>(count % 6);
		                     bar->SetProgressFillMode(mode);

		                     switch (mode) {
			                     case SimpleGui::ProgressFillMode::LeftToRight:
			                     case SimpleGui::ProgressFillMode::RightToLeft:
			                     case SimpleGui::ProgressFillMode::CenterH: {
				                     bar->SetSize(200, 20);
				                     break;
			                     };
			                     case SimpleGui::ProgressFillMode::BottomToTop:
			                     case SimpleGui::ProgressFillMode::TopToBottom:
			                     case SimpleGui::ProgressFillMode::CenterV: {
				                     bar->SetSize(20, 200);
				                     break;
			                     }
		                     }

		                     count++;
	                     });

	auto addBtn = layout->AddChild<Button>("add");
	layout->SetAnchorPoint(addBtn, AnchorPointType::Fixed, Vec2(btn->GetGlobalRect().Right() + 10, -10),
	                       AnchorPointLocation::BottomLeft, AnchorPointLocation::BottomLeft);
	addBtn->clicked.Connect("on_clicked_add",
	                        [bar]() {
		                        float value = bar->GetValue();
		                        bar->SetValue(value + 5);
	                        });

	bar->valueChanged.Connect("on_value_changed",
	                          [bar](float value) {
		                          if (IsEqualApprox(value, bar->GetMaxValue())) {
			                          SDL_Log("reach max value");
		                          }
	                          });

	auto timer = SG_GuiManager.GetTimer(0.5f);
	timer->timeout.Connect("on_timeout",
	                       [bar]() {
		                       float value = bar->GetValue();
		                       if (IsEqualApprox(value, bar->GetMaxValue())) {
			                       bar->SetValue(bar->GetMinValue());
		                       } else {
			                       bar->SetValue(value + 25);
		                       }
	                       });

	auto timerBtn = layout->AddChild<Button>("start timer");
	layout->SetAnchorPoint(timerBtn, AnchorPointType::Fixed, Vec2(addBtn->GetGlobalRect().Right() + 10, -10),
	                       AnchorPointLocation::BottomLeft, AnchorPointLocation::BottomLeft);
	timerBtn->clicked.Connect("on_clicked_timer",
	                          [timer, timerBtn]() {
		                          timer->SetPaused(!timer->IsPaused());
		                          if (timer->IsPaused()) {
			                          timerBtn->SetText("start timer");
		                          } else {
			                          timerBtn->SetText("pause timer");
		                          }
	                          });

	auto btn2 = layout->AddChild<Button>("SetIndeterminate");
	layout->SetAnchorPoint(btn2, AnchorPointType::Fixed, Vec2(timerBtn->GetGlobalRect().Right() + 10, -10),
	                       AnchorPointLocation::BottomLeft, AnchorPointLocation::BottomLeft);
	btn2->clicked.Connect("on_clicked",
	                      [bar]() {
		                      bar->SetIndeterminate(!bar->IsIndeterminate());
	                      });
}


static void TestSlider() {
	auto draggablePanel = SG_GuiManager.GetWindow().AddComponent<DraggablePanel>("test slider");
	draggablePanel->SetPosition(200, 100);
	draggablePanel->SetSize(300, 300);

	auto slider = draggablePanel->AddChild<Slider>(Direction::Horizontal);
	slider->SetSize(200, 20);
	slider->valueChanged.Connect("on_value_changed",
	                             [](float value) {
		                             SDL_Log("h slider value: %f", value);
	                             });

	auto v_slider = draggablePanel->AddChild<Slider>(Direction::Vertical);
	v_slider->SetSize(20, 200);
	v_slider->SetPosition(0, 100);
	v_slider->valueChanged.Connect("on_value_changed",
	                               [](float value) {
		                               SDL_Log("v slider value: %f", value);
	                               });

	//slider->SetScrollable(false);
	// v_slider->SetEditable(false);
}


static void TestCheckBox() {
	auto draggablePanel = SG_GuiManager.GetWindow().AddComponent<DraggablePanel>("test check box");
	draggablePanel->SetPosition(200, 100);
	draggablePanel->SetSize(300, 300);

	auto vBoxLayout = draggablePanel->AddChild<BoxLayout>(Direction::Vertical);
	vBoxLayout->SetSizeConfigs(ComponentSizeConfig::Expanding, ComponentSizeConfig::Expanding);

	auto checkBox1 = vBoxLayout->AddChild<CheckBox>("test check box1");
	auto checkBox2 = vBoxLayout->AddChild<CheckBox>("test check box2");
	auto checkBox3 = vBoxLayout->AddChild<CheckBox>("test check box3");
	auto checkBox4 = vBoxLayout->AddChild<CheckBox>("test check box4");

	auto group = CheckBoxGroup::Create();
	group->SetUniqueCheck(true);
	checkBox1->SetGroup(group);
	checkBox2->SetGroup(group);
	checkBox3->SetGroup(group);
	checkBox4->SetGroup(group);

	group->checkStateChanged.Connect("on_check_state_changed",
	                                 [](CheckBox *box) {
	                                 	std::string boxText = box->GetText();
	                                 	std::string checked =  box->IsChecked() ? "true" : "false";
	                                 	SG_INFO("{0}, checked = {1}", boxText, checked);
	                                 });

	auto btn = vBoxLayout->AddChild<Button>("switch unique: on");
	btn->clicked.Connect("on_clicked",
	                     [btn, group]() {
		                     bool on = group->IsUniqueCheck();
		                     group->SetUniqueCheck(!on);
		                     if (on) btn->SetText(std::format("switch unique: off"));
		                     else btn->SetText(std::format("switch unique: on"));
	                     });
}


static void TestLineEdit() {
	auto dp = SG_GuiManager.GetWindow().AddComponent<DraggablePanel>("test line edit");
	dp->SetSize(300, 300);

	auto lineEdit = dp->AddChild<LineEdit>("input");
	lineEdit->SetPosition(100, 100);
	//lineEdit->SetMaxInputLength(4);
	//lineEdit->CustomThemeColor(ThemeColorFlags::LineEditForeground, Color::RED);

	lineEdit->textChanged.Connect("on_textChanged",
	                              [](const std::string &string) {
		                              SDL_Log("on_textChanged: %s", string.c_str());
	                              });

	lineEdit->textChangeRejected.Connect("on_textChangeRejected",
	                                     [](const std::string &string) {
		                                     SDL_Log("on_textChangeRejected: %s", string.c_str());
	                                     });
}

static void TestDraggablePanel() {
	auto dp = SG_GuiManager.GetWindow().AddComponent<DraggablePanel>("test draggable panel");
	dp->SetSize(300, 300);

	auto layout = dp->AddChild<BoxLayout>(Direction::Vertical);
	layout->SetSizeConfigs(ComponentSizeConfig::Expanding, ComponentSizeConfig::Expanding);

	layout->AddChild<DraggablePanel>("draggable panel 1");
	layout->AddChild<DraggablePanel>("draggable panel 2");
	layout->AddChild<DraggablePanel>("draggable panel 3");
}

static void TestTextureRect() {
	auto dp = SG_GuiManager.GetWindow().AddComponent<DraggablePanel>("test texture rect");
	dp->SetSize(300, 300);

	auto texture = SG_GuiManager.GetWindow().GetRenderer().CreateSharedTexture("D:\\download\\edge\\bg.jpg");
	auto textureRect = dp->AddChild<TextureRect>(texture);
	textureRect->SetSizeConfigs(ComponentSizeConfig::Expanding, ComponentSizeConfig::Expanding);
	textureRect->SetTextureStretchMode(TextureStretchMode::KeepAspectCentered);

	dp->SetTitle(texture->GetPath());
}


static void TestComboBox() {
	auto ccb = SG_GuiManager.GetWindow().AddComponent<ComboBox>();
}


int main(int argc, char **argv) {
	//GuiManager::Init(argc, argv, "C:\\WINDOWS\\Fonts\\simhei.ttf");
	GuiManager::Init(argc, argv, R"(C:\Users\endif\Desktop\test_font.ttf)");
	Window &win = SG_GuiManager.GetWindow("win1-60fps", 640, 480);
	win.GetFont().SetSize(16);
	//win.SwitchStyle(StyleManager::LightStyle);

	win.GetRootComponent().AddExtendedFunctions<DrawBackgroundFunctions>(
		win.GetRenderer().CreateSharedTexture(R"(C:\Users\endif\Desktop\xiang_cheng.png)"));
	//win.GetRootComponent().AddExtendedFunctions<TestFrameRateControllerFunctions>();

	// TestScrollBar();
	// TestScrollPanel();
	// TestLineEdit();
	// TestTimer();
	// TestProgressBar();
	// TestSlider();
	TestCheckBox();
	TestTextureRect();
	TestDraggablePanel();

	auto fpsLbl = win.AddComponent<Label>("");
	fpsLbl->AddExtendedFunctions<DisplayFPSForLabel>();
	fpsLbl->CustomThemeColor(ThemeColorFlags::LabelForeground, Color::GREEN);
	fpsLbl->CustomThemeColor(ThemeColorFlags::LabelBackground, Color(0, 0, 0, 50));

	auto deltaLbl = win.AddComponent<Label>("");
	// deltaLbl->AddExtendedFunctions<DisplayDeltaForLabel>();
	deltaLbl->CustomThemeColor(ThemeColorFlags::LabelForeground, Color::GREEN);
	deltaLbl->CustomThemeColor(ThemeColorFlags::LabelBackground, Color(0, 0, 0, 50));
	deltaLbl->SetPositionY(100);

	auto timer = SG_GuiManager.GetTimer(0.5f);
	timer->timeout.Connect("on_timeout",
	                       [deltaLbl] {
	                       	deltaLbl->SetText(std::format("delta: {:.4f}", SG_GuiManager.GetDelta()));
	                       });
	timer->Start();

	// win.EnableVsync(true);
	SG_GuiManager.SetTargetFrameRate(60);
	//SG_GuiManager.SetUnlimitedFrameRate(true);
	SG_GuiManager.Run();
	GuiManager::Quit();

	return 0;
}
