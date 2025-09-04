#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <SDL3_image/SDL_image.h>
#include <simple_gui.hpp>
#include <style.hpp>

using namespace SimpleGui;


int main(int argc, char** argv) {
	 GuiManager::Init(argc, argv, "");
	 Window& win = SG_GuiManager.CreateWindow("demo", 1280, 720);
	 SDL_Log("win title: %s\n", win.GetTitle().c_str());
	 GuiManager::Quit();
	
	return 0;
}
