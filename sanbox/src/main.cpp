#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <SDL3_image/SDL_image.h>
#include <simple_gui.hpp>

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

	SDL_Window* win = SDL_CreateWindow("sandbox", 640, 480, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(win, NULL);

	SDL_Event event{};
	bool running = true;

	GuiManager::Init(win, renderer, "C:\\Windows\\Fonts\\simhei.ttf");

	auto cmp1 = SG_GuiManager.AddComponent<BaseComponent>();
	cmp1->SetSize(200, 200);
	cmp1->SetPosition(100, 100);

	auto cmp2 = cmp1->AddChild<BaseComponent>();
	cmp2->SetSize(100, 100);
	cmp2->SetGlobalPosition(50, 50);

	while (running) {
		SG_GuiManager.FrameStart();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}

			SG_GuiManager.HandleEvent(event);
		}

		SG_GuiManager.Update();

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
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
