#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <SDL3_image/SDL_image.h>
#include <simple_gui.hpp>
#include <style.hpp>

using namespace SimpleGui;

// TODO add Extended functions for component
class ExtendedFunction {
private:
	friend class BaseComponent;

	virtual void HandleEvent(Event* event) = 0;
	virtual void Update() = 0;
	virtual void Render(const Renderer& renderer) = 0;

	BaseComponent* m_target;
};


int main(int argc, char** argv) {
	 GuiManager::Init(argc, argv, "");
	 Window& win = SG_GuiManager.CreateWindow("win1-60fps", 640, 480);
	 Window& win2 = SG_GuiManager.CreateWindow("win2-30fps", 640, 480);
	 win2.SetTargetFrameRate(30);
	 //win.EnableVsync(true);
	 SDL_Log("win title: %s\n", win.GetTitle().c_str());
	 SG_GuiManager.Run();
	 GuiManager::Quit();
	
	return 0;
}
