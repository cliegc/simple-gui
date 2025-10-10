#pragma once
#include "base_component.hpp"
#include "texture.hpp"
#include "label.hpp"


namespace SimpleGui {
	class TextureRect final : public BaseComponent {
	public:
		explicit TextureRect(const std::shared_ptr<Texture> &texture = nullptr);
		~TextureRect() override = default;

		std::shared_ptr<Texture> GetTexture() const;
		void SetTexture(const std::shared_ptr<Texture> &texture);

		TextureStretchMode GetTextureStretchMode() const;
		void SetTextureStretchMode(TextureStretchMode mode);
		void SetScaleMode(SDL_ScaleMode mode);

		bool IsFlipH() const;
		void SetFlipH(bool flip);

		bool IsFlipV() const;
		void SetFlipV(bool flip);

		void Update() override;
		void Render(Renderer& renderer) override;

	private:
		std::shared_ptr<Texture> m_texture;
		std::unique_ptr<Label> m_tipLbl;
		TextureStretchMode m_textureStretchMode = TextureStretchMode::Scale;
		SDL_ScaleMode m_scaleMode;
		SDL_FlipMode m_flipMode;
		Rect m_textureGRect;

	private:
		void SetupTipLabel() const;
		void UpdateTextureStretchMode();
		void RenderTexture(SDL_Renderer* renderer) const;
	};
}