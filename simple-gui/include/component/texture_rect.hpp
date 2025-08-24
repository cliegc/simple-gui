#pragma once
#include "base_component.hpp"


namespace SimpleGui {
	class TextureRect final : public BaseComponent {
	public:
		TextureRect(std::shared_ptr<SDL_Texture> texture);
		virtual ~TextureRect() = default;

		std::shared_ptr<SDL_Texture> GetTexture() const;
		void SetTexture(std::shared_ptr<SDL_Texture> texture);

		TextureStretchMode GetTextureStretchMode() const;
		void SetTextureStretchMode(TextureStretchMode mode);
		void SetScaleMode(SDL_ScaleMode mode);

		bool IsFlipH() const;
		void SetFlipH(bool flip);

		bool IsFlipV() const;
		void SetFlipV(bool flip);

		virtual void Update() override;
		virtual void Render(const Renderer& renderer) override;

	private:
		std::shared_ptr<SDL_Texture> m_texture;
		TextureStretchMode m_textureStretchMode = TextureStretchMode::Scale;
		SDL_ScaleMode m_scaleMode;
		SDL_FlipMode m_flipMode;
		Rect m_textureGRect;

		void UpdateTextureStretchMode();
		void RenderTexture(SDL_Renderer* renderer);
	};
}