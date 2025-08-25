#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <SDL3_image/SDL_image.h>
#include <simple_gui.hpp>
#include <style.hpp>

using namespace SimpleGui;

void TestLabelAndButton() {
	auto lbl1 = SG_GuiManager.AddComponent<Label>("this is a label");
	lbl1->SetPosition(100, 100);
	lbl1->SetSize(200, 200);
	lbl1->SetTextAlignments(TextAlignment::Center, TextAlignment::Center);

	lbl1->SetFontSize(36);
	lbl1->CustomThemeColor(ThemeColorFlags::LabelForeground, Color::RED);
	lbl1->CustomThemeColor(ThemeColorFlags::LabelBackgound, Color(0, 0, 0, 100));
	//lbl1->ClearCustomThemeColors();

	lbl1->AddChild<Label>("test label2");

	auto btn1 = SG_GuiManager.AddComponent<Button>("click me");
	btn1->SetPosition(100, 300);
	btn1->SetSize(300, 300);
	btn1->SetFontSize(36);

	auto style = SG_GuiManager.GetStyle(StyleManager::DarkStyle);
	if (style.has_value()) style.value().get().colors[ThemeColorFlags::LabelForeground] = Color::AQUA;

	
	btn1->clicked.Connect("on_clicked",
		[]() {
			SDL_Log("switch style\n");
			static std::string styleName = StyleManager::DarkStyle;
			SG_GuiManager.SwitchStyle(styleName);
			if (styleName == StyleManager::DarkStyle)
				styleName = StyleManager::LightStyle;
			else
				styleName = StyleManager::DarkStyle;
		}
	);
}

void TestDraggablePanel(SDL_Renderer* renderer) {
	auto draggablePanel = SG_GuiManager.AddComponent<DraggablePanel>("test");
	draggablePanel->SetSize(500, 500);
	draggablePanel->SetPosition(300, 100);
	//draggablePanel->SetHandleVisible(false);
	draggablePanel->SetGlobalDragEnable(true);

	auto lbl2 = draggablePanel->AddChild<Label>("test label");
	lbl2->SetPosition(-10, 20);
	lbl2->SetFontSize(36);
	lbl2->CustomThemeColor(ThemeColorFlags::LabelBorder, Color::GREEN);

	auto textureRect = 
		draggablePanel->AddChild<TextureRect>(SG_GuiManager.GetRenderer().CreateSharedTexture("C:\\Users\\15310\\Desktop\\11.png"));
	textureRect->SetSizeConfigs(ComponentSizeConfig::Expanding, ComponentSizeConfig::Expanding);
	textureRect->SetTextureStretchMode(TextureStretchMode::KeepAspectCentered);

	auto draggablePanel2 = draggablePanel->AddChild<DraggablePanel>("test2");
	//draggablePanel2->SetSize(200, 200);

	auto btn = draggablePanel2->AddChild<Button>("test button");
	btn->SetSizeConfigs(ComponentSizeConfig::Expanding, ComponentSizeConfig::Expanding);
	btn->clicked.Connect("on_clicked",
		[draggablePanel]() {
			draggablePanel->SetGlobalDragEnable(!draggablePanel->IsGlobalDragEnable());
		}
	);

	SG_GuiManager.GetRootComponent().AddChild<Label>("add child from root component");
}

void TestBoxLayout() {
	auto layout = SG_GuiManager.AddComponent<BoxLayout>(Direction::Horizontal);
	layout->SetSizeConfigs(ComponentSizeConfig::Expanding, ComponentSizeConfig::Expanding);
	layout->SetAlignment(Alignment::End);
	layout->SetWeights({ 2, 5 });
	layout->SetDirection(Direction::Vertical);

	auto lbl1 = layout->AddChild<Label>("Label 1");
	lbl1->SetTextAlignments(TextAlignment::Center, TextAlignment::Center);
	//lbl1->SetSizeConfigW(ComponentSizeConfig::Expanding);
	//lbl1->SetSizeConfigH(ComponentSizeConfig::Expanding);

	auto btn1 = layout->AddChild<Button>("Button 1");
	//btn1->SetSizeConfigW(ComponentSizeConfig::Expanding);
	//btn1->SetSizeConfigH(ComponentSizeConfig::Expanding);
}

void TestAnchorPointLayout() {
	auto layout = SG_GuiManager.AddComponent<AnchorPointLayout>();
	layout->SetSizeConfigs(ComponentSizeConfig::Expanding, ComponentSizeConfig::Expanding);

	auto btn = layout->AddChild<Button>("test anchor point layout");
	btn->SetSize(100, 100);
	btn->clicked.Connect("on_clicked_print", 
		[btn]() {
			SDL_Log("%s\n", btn->GetText().c_str());
		});

	//layout->SetAnchorPoint(btn, AnchorPointType::Percentage, Vec2(0.0f, 0.0f), 
	//	AnchorPointLocation::Center, AnchorPointLocation::Center);

	layout->SetAnchorPoint(btn, AnchorPointType::Fixed, Vec2(-50.f, -50.f),
		AnchorPointLocation::BottomRight, AnchorPointLocation::BottomRight);
}

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	TTF_Init();

	SDL_Window* win = SDL_CreateWindow("sandbox", 1208, 720, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(win, NULL);

	SDL_Event event{};
	bool running = true;

	if (SDL_SetRenderVSync(renderer, 1)) {
		SDL_Log("set renderrer Vsync\n");
	}

	GuiManager::Init(win, renderer, "C:\\Windows\\Fonts\\simhei.ttf");
	SG_GuiManager.SwitchStyle(StyleManager::LightStyle);
	SG_GuiManager.SetStyleFollowSystem();

	//TestLabelAndButton();
	//TestDraggablePanel(renderer);
	//TestBoxLayout();
	TestAnchorPointLayout();

	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}

			SG_GuiManager.HandleEvent(event);
		}

		SG_GuiManager.Update();

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderClear(renderer);
		
		//SDL_SetRenderLogicalPresentation(renderer, 640, 360, SDL_LOGICAL_PRESENTATION_STRETCH);
		//SDL_FRect rect = { 0, 0, 300, 300 };
		//SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		//SDL_RenderFillRect(renderer, &rect);
		//SDL_SetRenderLogicalPresentation(renderer, 640, 360, SDL_LOGICAL_PRESENTATION_DISABLED);
		SG_GuiManager.Render();
		//SDL_SetRenderLogicalPresentation(renderer, 640, 360, SDL_LOGICAL_PRESENTATION_DISABLED);

		SDL_RenderPresent(renderer);

		//SDL_Delay(16);
	}

	GuiManager::Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	TTF_Quit();
	SDL_Quit();

	return 0;
}
