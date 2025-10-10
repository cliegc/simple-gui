#pragma once
#include <SDL3/SDL_render.h>
#include <string>
#include <string_view>
#include "math.hpp"


namespace SimpleGui {
	class Renderer;

	class Texture final {
		friend class Renderer;
	public:
		~Texture();

		SDL_Texture& GetSDLTexture() const { return *m_texture; }
		
		int GetWidth() const { return m_texture->w; }
		int GetHeight() const { return m_texture->h; }
		Rect GetRect() const {
			return Rect{0.f, 0.f, static_cast<float>(m_texture->w), static_cast<float>(m_texture->h) };
		}

		std::string GetPath() const { return m_path; }

		bool IsNull() const { return m_texture == nullptr; }

	private:
		SDL_Texture* m_texture;
		std::string m_path;

		Texture(const Renderer& renderer, std::string_view path);
	};
}