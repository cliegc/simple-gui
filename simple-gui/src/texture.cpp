#include "texture.hpp"
#include <SDL3_image/SDL_image.h>
#include "gui_manager.hpp"


namespace SimpleGui {
	Texture::Texture(std::string_view path) {
		m_texture = IMG_LoadTexture(&SG_GuiManager.GetRenderer().GetSDLRenderer(), path.data());
		m_path = path;
	}

	Texture::~Texture() {
		if (m_texture) {
			SDL_DestroyTexture(m_texture);
		}
	}
}