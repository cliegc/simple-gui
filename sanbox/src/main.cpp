#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <SDL3_image/SDL_image.h>
#include <simple_gui.hpp>
#include <style.hpp>

using namespace SimpleGui;



struct TextureDeleter final {
	void operator()(SDL_Texture* texture) {
		if (!texture) return;
		SDL_DestroyTexture(texture);
	}
};



int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	TTF_Init();

	SDL_Window* win = SDL_CreateWindow("sandbox", 1208, 720, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(win, NULL);

	SDL_Event event{};
	bool running = true;

	GuiManager::Init(win, renderer, "C:\\Windows\\Fonts\\simhei.ttf");
	SG_GuiManager.SwitchStyle(StyleManager::LightStyle);

	auto lbl1 = SG_GuiManager.AddComponent<Label>("this is a label");
	lbl1->SetPosition(100, 100);
	lbl1->SetSize(200, 200);
	lbl1->SetTextAlignments(TextAlignment::Center, TextAlignment::Center);

	lbl1->AddChild<Label>("test lable1");

	auto lbl2 = lbl1->AddChild<Label>("label 2");
	lbl2->SetPosition(100, 0);
	lbl2->CustomThemeColor(ThemeColorFlags::LabelForeground, Color::GREEN);

	lbl1->SetFontSize(36);
	lbl1->CustomThemeColor(ThemeColorFlags::Foreground, Color::RED);
	lbl1->CustomThemeColor(ThemeColorFlags::Foreground, Color::MAGENTA);
	//lbl1->ClearCustomThemeColors();

	lbl1->AddChild<Label>("test label2");


	auto btn1 = SG_GuiManager.AddComponent<Button>("click me");
	btn1->SetPosition(100, 300);
	btn1->SetSize(300, 300);
	btn1->SetDisabled(true);
	btn1->clicked.Connect("on_clicked", 
		[btn1, lbl2, lbl1]() {
			SDL_Log("clicked\n");
			//auto ll = lbl1->RemoveChildDeferred(lbl2);
			////ll->CustomThemeColor(ThemeColorFlags::LabelForeground, Color::RED);
			//ll->ClearCustomThemeColor(ThemeColorFlags::LabelForeground);
			//btn1->AddChildDeferred(std::move(ll));
			SG_GuiManager.SwitchStyle(StyleManager::DarkStyle);
		}
	);
	btn1->SetDisabled(false);

	auto lbl3 = btn1->AddChild<Label>("test label1");
	auto lbl4 = btn1->AddChild<Label>("test label2");
	lbl3->SetPosition(lbl3->GetRect().BottomLeft());


	while (running) {
		SG_GuiManager.FrameStart();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}

			SG_GuiManager.HandleEvent(event);
		}

		SG_GuiManager.Update();

		SDL_SetRenderDrawColor(renderer, 47, 47, 47, 255);
		SDL_RenderClear(renderer);
		
		//SDL_SetRenderLogicalPresentation(renderer, 640, 360, SDL_LOGICAL_PRESENTATION_STRETCH);
		//SDL_FRect rect = { 0, 0, 300, 300 };
		//SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		//SDL_RenderFillRect(renderer, &rect);
		//SDL_SetRenderLogicalPresentation(renderer, 640, 360, SDL_LOGICAL_PRESENTATION_DISABLED);

		SG_GuiManager.Render();

		SDL_RenderPresent(renderer);

		SG_GuiManager.FrameEnd();

		SDL_Delay(17);
	}

	GuiManager::Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	TTF_Quit();
	SDL_Quit();

	return 0;
}
