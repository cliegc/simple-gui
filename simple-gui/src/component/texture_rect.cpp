#include "component/texture_rect.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	TextureRect::TextureRect(std::shared_ptr<SDL_Texture> texture) : BaseComponent() {
		m_texture = texture;
		m_scaleMode = SDL_SCALEMODE_LINEAR;
		m_flipMode = SDL_FLIP_NONE;
		m_padding = SG_GuiManager.GetCurrentStyle().componentPadding;

		if (m_texture) {
			m_size.w = m_texture->w;
			m_size.h = m_texture->h;
			m_textureGRect = GetContentRect();
			m_textureGRect.position += GetGlobalPosition();

			SDL_SetTextureScaleMode(m_texture.get(), m_scaleMode);
		}
	}

	void TextureRect::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		if (m_texture) {
			UpdateTextureStretchMode();
		}

		BaseComponent::Update();
	}

	void TextureRect::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		// draw bg
		renderer.FillRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::TextureRectBackround));
		
		renderer.SetClipRect(m_visibleGRect);
		if (m_texture) {
			Rect rect(0, 0, m_texture->w, m_texture->h);
			renderer.DrawTexture(m_texture.get(), rect, m_textureGRect, 0, rect.Center(), m_flipMode);
		}
		renderer.DrawRect(GetGlobalRect(), GetThemeColor(ThemeColorFlags::TextureRectBorder));
		renderer.ClearClipRect();

		BaseComponent::Render(renderer);
	}

	std::shared_ptr<SDL_Texture> TextureRect::GetTexture() const {
		return m_texture;
	}

	void TextureRect::SetTexture(std::shared_ptr<SDL_Texture> texture) {
		m_texture = texture;
		SetScaleMode(m_scaleMode);
	}

	TextureStretchMode TextureRect::GetTextureStretchMode() const {
		return m_textureStretchMode;
	}

	void TextureRect::SetTextureStretchMode(TextureStretchMode mode) {
		m_textureStretchMode = mode;
	}

	void TextureRect::SetScaleMode(SDL_ScaleMode mode) {
		if (!m_texture) return;
		SDL_SetTextureScaleMode(m_texture.get(), mode);
		m_scaleMode = mode;
	}

	bool TextureRect::IsFlipH() const {
		return (m_flipMode & SDL_FLIP_HORIZONTAL) == SDL_FLIP_HORIZONTAL;
	}

	void TextureRect::SetFlipH(bool flip) {
		bool isFlipV = IsFlipV();

		m_flipMode = isFlipV ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
		if (flip) m_flipMode = (SDL_FlipMode)(m_flipMode | SDL_FLIP_HORIZONTAL);
	}

	bool TextureRect::IsFlipV() const {
		return (m_flipMode & SDL_FLIP_VERTICAL) == SDL_FLIP_VERTICAL;
	}

	void TextureRect::SetFlipV(bool flip) {
		bool isFlipH = IsFlipH();

		m_flipMode = isFlipH ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
		if (flip) m_flipMode = (SDL_FlipMode)(m_flipMode | SDL_FLIP_VERTICAL);
	}

	void TextureRect::UpdateTextureStretchMode() {
		//Scale,							// 图片大小跟随组件大小（不超过组件大小），不保证图片的宽高比
		//Tile,							// 图片平铺整个控件
		//Keep,							// 保持图片的大小，并在组件的左上角显示
		//KeepCentered,					// 保持图片的大小，并在组件中心位置显示
		//KeepAspect,						// 图片大小跟随组件大小（不超过组件大小），保证图片的宽高比，并在组件的左上角显示
		//KeepAspectCentered,				// 图片大小跟随组件大小（不超过组件大小），保证图片的宽高比，并在组件中心位置显示
		//KeepAspectCovered,				// 图片大小跟随组件大小（超过组件大小），保证图片的宽高比，并在组件中心位置显示

		Rect globaleRect = GetGlobalRect();
		Rect contentRect = GetContentRect();
		contentRect.position += globaleRect.position;

		switch (m_textureStretchMode) {
		case TextureStretchMode::Scale:
		case TextureStretchMode::Tile:
		{
			m_textureGRect = contentRect;
			break;
		}
		case TextureStretchMode::Keep:
		{
			m_textureGRect.position = globaleRect.position + Vec2(m_padding.left, m_padding.top);
			m_textureGRect.size = Vec2(m_texture->w, m_texture->h);
			break;
		}
		case TextureStretchMode::KeepCentered:
		{
			m_textureGRect.size = Vec2(m_texture->w, m_texture->h);
			m_textureGRect.position.x = (globaleRect.size.w - m_padding.left - m_padding.right - m_textureGRect.size.w) / 2 + globaleRect.position.x;
			m_textureGRect.position.y = (globaleRect.size.h - m_padding.top - m_padding.bottom - m_textureGRect.size.h) / 2 + globaleRect.position.y;
			break;
		}
		case TextureStretchMode::KeepAspect:
		{
			m_textureGRect.size = Vec2(m_texture->w, m_texture->h);
			m_textureGRect.AdjustSizeToFitOtherRect(contentRect);
			m_textureGRect.position = contentRect.position;
			break;
		}
		case TextureStretchMode::KeepAspectCentered:
		{
			m_textureGRect.size = Vec2(m_texture->w, m_texture->h);
			m_textureGRect.AdjustSizeToFitOtherRect(contentRect);
			break;
		}
		case TextureStretchMode::KeepAspectCovered:
		{
			m_textureGRect.size = Vec2(m_texture->w, m_texture->h);
			m_textureGRect.AdjustSizeToCoverOtherRect(contentRect);
			break;
		}
		default:
			break;
		}
	}

	void TextureRect::RenderTexture(SDL_Renderer* renderer) {
		SDL_FRect rect = m_textureGRect.ToSDLFRect();

		if (m_textureStretchMode == TextureStretchMode::Tile) {
			for (int i = 0; i < rect.w;) {
				for (int j = 0; j < rect.h;) {
					SDL_FRect tileRect = { i, j, m_texture->w, m_texture->h };
					SDL_RenderTextureRotated(renderer, m_texture.get(), NULL, &tileRect, 0, NULL, m_flipMode);
					j += m_texture->h;
				}
				i += m_texture->w;
			}
		}
		else {
			SDL_RenderTextureRotated(renderer, m_texture.get(), NULL, &rect, 0, NULL, m_flipMode);
		}
	}
}