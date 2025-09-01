#pragma once
#include <SDL3/SDL_render.h>
#include <string>
#include <string_view>


namespace SimpleGui {
	class Texture final {
	public:
		Texture(std::string_view path);
		~Texture();

		inline SDL_Texture* GetSDLTexture() const { return m_texture; }
		
		inline int GetWidth() const { return m_texture->w; }
		inline int GetHeight() const { return m_texture->h; }

		inline std::string GetPath() const { return m_path; }

		inline bool IsNull() const { return m_texture == nullptr; }

	private:
		SDL_Texture* m_texture;
		std::string m_path;
	};
}