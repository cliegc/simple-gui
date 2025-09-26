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

	//for (int i = 0; i < 500; ++i) {
	//	auto btn = scrollPanel->AddChild<Button>(std::format("button {}", i));
	//	btn->clicked.Connect("on_clicked",
	//		[i]() {
	//			SDL_Log("click button: %d\n", i);
	//		});
	//	btn->SetPosition(
	//		SDL_randf() * 800 - 300,
	//		SDL_randf() * 800 - 300
	//	);
	//}

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
	btn2->SetSize(300, 300);
	btn2->clicked.Connect("on_clicked", 
		[scrollPanel]() {
			scrollPanel->ClearAllChildrenDeferred();
		});

	auto btn3 = btn2->AddChild<Button>("btn3");
	btn3->SetPosition(10, 10);
	btn3->SetSize(200, 200);

	//scrollPanel->CustomThemeColor(ThemeColorFlags::LabelForeground, Color::RED);
	//SG_GuiManager.GetWindow().GetRootComponent().CustomThemeColor(ThemeColorFlags::ScrollPanelBackground, Color::YELLOW);
	//SG_GuiManager.GetWindow().GetRootComponent().CustomThemeColor(ThemeColorFlags::ScrollbarSlider_H, Color::YELLOW);
	//scrollPanel->CustomThemeColor(ThemeColorFlags::ScrollbarSlider_H, Color::BLUE);
}


static void TestTimer() {
	auto btn = SG_GuiManager.GetWindow().AddComponent<Button>("kill timer");
	auto timer = SG_GuiManager.GetTimer(1.f);
	
	timer->timeout.Connect("on_timeout_change_btn_color",
		[]() {
			Color color(
				SDL_randf() * 255,
				SDL_randf() * 255,
				SDL_randf() * 255
			);
			SG_GuiManager.GetWindow().GetRootComponent().CustomThemeColor(ThemeColorFlags::Background, color);
		});

	btn->SetPosition(200, 200);
	btn->clicked.Connect("on_clicked_kill_timer",
		[timer]() {
			if (timer) SG_GuiManager.KillTimer(timer);
			else SDL_Log("btn clicked: delete timer");
		});


	timer->Start();
}


static void TestLineEdit() {
	auto lineEdit = SG_GuiManager.GetWindow().AddComponent<LineEdit>("input");
	lineEdit->SetPosition(200, 200);
	lineEdit->CustomThemeColor(ThemeColorFlags::LineEditForeground, Color::RED);
}

int main(int argc, char** argv) {
	//GuiManager::Init(argc, argv, "C:\\WINDOWS\\Fonts\\simhei.ttf");
	GuiManager::Init(argc, argv, "C:\\WINDOWS\\Fonts\\msyh.ttc");
	Window& win = SG_GuiManager.GetWindow("win1-60fps", 640, 480);
	//win.SwitchStyle(StyleManager::LightStyle);

	//win.GetRootComponent().AddExtendedFunctions<DrawBackgroundFunctions>(win.GetRenderer().CreateSharedTexture("C:\\Users\\endif\\Desktop\\jinzi.png"));
	//win.GetRootComponent().AddExtendedFunctions<TestFrameRateControllerFunctions>();

	win.AddComponent<Label>("")->AddExtendedFunctions<DisplayFPSForLabel>();
	auto lbl2 = win.AddComponent<Label>("");
	lbl2->AddExtendedFunctions<DisplayDeltaForLabel>();
	lbl2->SetPositionY(100);

	//TestScrollBar();
	//TestScrollPanel();
	TestLineEdit();
	//TestTimer();

	//win.EnableVsync(true);
	SG_GuiManager.SetTargetFrameRate(60);
	//SG_GuiManager.SetUnlimitedFrameRate(true);
	SG_GuiManager.Run();
	GuiManager::Quit();

	return 0;
}

#pragma region input demo
//#include <SDL3/SDL.h>
//#include <string>
//#include <vector>
//#include <iostream>
//
//// 文本输入框类
//class TextInputBox {
//private:
//    SDL_FRect rect;           // 输入框位置和大小
//    std::string text;         // 存储的文本
//    std::string composition;  // 正在输入的文本（用于中文输入法）
//    bool active;              // 是否处于激活状态
//    SDL_Color textColor;      // 文本颜色
//    SDL_Color bgColor;        // 背景颜色
//    SDL_Color borderColor;    // 边框颜色
//    TTF_Font* font;           // 字体
//
//    SDL_Window* win;
//
//public:
//    TextInputBox(float x, float y, float w, float h, TTF_Font* font, SDL_Window* win)
//        : rect{ x, y, w, h }, text(""), composition(""), active(false),
//        textColor{ 255, 255, 255, 255 }, bgColor{ 50, 50, 50, 255 },
//        borderColor{ 100, 100, 255, 255 }, font(font), win(win) {
//    }
//
//    // 处理事件
//    bool handleEvent(const SDL_Event& event) {
//        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
//            float mouseX = event.button.x;
//            float mouseY = event.button.y;
//
//            // 检查是否点击了输入框
//            if (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
//                mouseY >= rect.y && mouseY <= rect.y + rect.h) {
//                active = true;
//                // 开始文本输入
//                SDL_StartTextInput(win);
//                return true;
//            }
//            else {
//                active = false;
//                // 停止文本输入
//                SDL_StopTextInput(win);
//                return false;
//            }
//        }
//
//        if (active) {
//            if (event.type == SDL_EVENT_TEXT_INPUT) {
//                // 添加输入的文本
//                text += event.text.text;
//                SDL_Log("text input: %s", text.c_str());
//                return true;
//            }
//            else if (event.type == SDL_EVENT_TEXT_EDITING) {
//                // 更新正在编辑的文本（用于中文输入法）
//                composition = event.edit.text;
//                SDL_Log("text editiing: %s", composition.c_str());
//                return true;
//            }
//            else if (event.type == SDL_EVENT_KEY_DOWN) {
//                if (event.key.key == SDLK_BACKSPACE) {
//                    // 处理退格键
//                    if (!text.empty()) {
//                        text.pop_back();
//                    }
//                    return true;
//                }
//                else if (event.key.key == SDLK_RETURN || event.key.key == SDLK_KP_ENTER) {
//                    // 处理回车键
//                    std::cout << "输入的内容: " << text << std::endl;
//                    return true;
//                }
//            }
//        }
//
//        return false;
//    }
//
//    // 渲染输入框
//    void render(SDL_Renderer* renderer) {
//        // 绘制背景
//        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
//        SDL_RenderFillRect(renderer, &rect);
//
//        // 绘制边框
//        if (active) {
//            SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
//        }
//        else {
//            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
//        }
//        SDL_RenderRect(renderer, &rect);
//
//        // 如果有文本，渲染文本
//        if (!text.empty() || !composition.empty()) {
//            std::string displayText = text + composition;
//
//            // 创建文本表面
//            SDL_Surface* textSurface = TTF_RenderText_Blended(font, displayText.c_str(), displayText.length(), textColor);
//            //std::cout << "text: " << displayText << std::endl;
//                //TTF_RenderUTF8_Blended(font, displayText.c_str(), textColor);
//            if (textSurface) {
//                // 创建纹理
//                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
//                if (textTexture) {
//                    // 计算文本位置（居中）
//                    SDL_FRect textRect = {
//                        rect.x + 5,
//                        rect.y + (rect.h - textSurface->h) / 2,
//                        static_cast<float>(textSurface->w),
//                        static_cast<float>(textSurface->h)
//                    };
//
//                    // 如果文本太长，裁剪显示
//                    if (textRect.w > rect.w - 10) {
//                        textRect.w = rect.w - 10;
//                    }
//
//                    // 渲染文本
//                    SDL_RenderTexture(renderer, textTexture, NULL, &textRect);
//
//                    // 清理
//                    SDL_DestroyTexture(textTexture);
//                }
//                SDL_DestroySurface(textSurface);
//            }
//        }
//
//        // 如果处于激活状态且没有文本，显示光标
//        if (active && text.empty() && composition.empty()) {
//            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//            SDL_FRect cursor = { rect.x + 5, rect.y + 5, 2, rect.h - 10 };
//            SDL_RenderFillRect(renderer, &cursor);
//        }
//    }
//
//    // 获取文本内容
//    std::string getText() const { return text; }
//
//    // 设置文本内容
//    void setText(const std::string& newText) { text = newText; }
//
//    // 检查是否激活
//    bool isActive() const { return active; }
//};
//
//int main(int argc, char* argv[]) {
//    // 初始化 SDL
//    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//        std::cerr << "SDL 初始化失败: " << SDL_GetError() << std::endl;
//        return 1;
//    }
//
//    // 初始化 TTF
//    if (TTF_Init() < 0) {
//        std::cerr << "TTF 初始化失败: " << SDL_GetError() << std::endl;
//        SDL_Quit();
//        return 1;
//    }
//
//    // 创建窗口
//    SDL_Window* window = SDL_CreateWindow("SDL3 中文文本输入框", 800, 600, SDL_WINDOW_RESIZABLE);
//    if (!window) {
//        std::cerr << "窗口创建失败: " << SDL_GetError() << std::endl;
//        TTF_Quit();
//        SDL_Quit();
//        return 1;
//    }
//
//    // 创建渲染器
//    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
//    if (!renderer) {
//        std::cerr << "渲染器创建失败: " << SDL_GetError() << std::endl;
//        SDL_DestroyWindow(window);
//        TTF_Quit();
//        SDL_Quit();
//        return 1;
//    }
//
//    // 加载字体 - 请确保系统中存在此字体文件，或替换为其他中文字体
//    TTF_Font* font = TTF_OpenFont("simhei.ttf", 24); // 使用黑体
//    if (!font) {
//        // 尝试其他常见中文字体
//        const char* fontPaths[] = {
//            "C:/Windows/Fonts/simhei.ttf",  // Windows
//            "/System/Library/Fonts/PingFang.ttc",  // macOS
//            "/usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf",  // Linux
//            nullptr
//        };
//
//        for (int i = 0; fontPaths[i] != nullptr; i++) {
//            font = TTF_OpenFont(fontPaths[i], 24);
//            if (font) break;
//        }
//
//        if (!font) {
//            std::cerr << "字体加载失败: " << SDL_GetError() << std::endl;
//            // 尝试使用默认字体
//            font = TTF_OpenFont(nullptr, 24);
//            if (!font) {
//                SDL_DestroyRenderer(renderer);
//                SDL_DestroyWindow(window);
//                TTF_Quit();
//                SDL_Quit();
//                return 1;
//            }
//        }
//    }
//
//    // composition
//    // candidates
//    SDL_SetHint(SDL_HINT_IME_IMPLEMENTED_UI, "composition");
//
//    // 创建文本输入框
//    TextInputBox textInput(100, 100, 400, 40, font, window);
//
//    // 主循环
//    bool running = true;
//    while (running) {
//        SDL_Event event;
//        while (SDL_PollEvent(&event)) {
//            if (event.type == SDL_EVENT_QUIT) {
//                running = false;
//            }
//
//            // 处理文本输入框事件
//            textInput.handleEvent(event);
//        }
//
//        // 清屏
//        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
//        SDL_RenderClear(renderer);
//
//        // 渲染文本输入框
//        textInput.render(renderer);
//
//        // 显示提示文本
//        TTF_Font* smallFont = TTF_OpenFont("C:/Windows/Fonts/simhei.ttf", 18);
//        if (smallFont) {
//            SDL_Color hintColor = { 200, 200, 200, 255 };
//            SDL_Surface* hintSurface = TTF_RenderText_Blended(smallFont,
//                "点击文本框输入中文，按回车确认", 46, hintColor);
//            if (hintSurface) {
//                SDL_Texture* hintTexture = SDL_CreateTextureFromSurface(renderer, hintSurface);
//                if (hintTexture) {
//                    SDL_FRect hintRect = { 100, 160, static_cast<float>(hintSurface->w),
//                                         static_cast<float>(hintSurface->h) };
//                    SDL_RenderTexture(renderer, hintTexture, NULL, &hintRect);
//                    SDL_DestroyTexture(hintTexture);
//                }
//                SDL_DestroySurface(hintSurface);
//            }
//            TTF_CloseFont(smallFont);
//        }
//
//        // 更新屏幕
//        SDL_RenderPresent(renderer);
//
//        // 稍微延迟以减少CPU占用
//        SDL_Delay(16);
//    }
//
//    // 清理资源
//    TTF_CloseFont(font);
//    SDL_DestroyRenderer(renderer);
//    SDL_DestroyWindow(window);
//    TTF_Quit();
//    SDL_Quit();
//
//    return 0;
//}
#pragma endregion
