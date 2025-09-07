#include "texture.hpp"
#include <SDL3_image/SDL_image.h>
#include "renderer.hpp"


namespace SimpleGui {
	Texture::Texture(const Renderer& renderer, std::string_view path) {
		m_texture = IMG_LoadTexture(&renderer.GetSDLRenderer(), path.data());
		m_path = path;
	}

	Texture::~Texture() {
		if (m_texture) {
			SDL_DestroyTexture(m_texture);
		}
	}
}