#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <memory>


namespace SimpleGui {
    struct TextEngineDeleter {
        void operator()(TTF_TextEngine* engine) const noexcept;
    };
    using UniqueTextEnginePtr = std::unique_ptr<TTF_TextEngine, TextEngineDeleter>;

    struct FontDeleter {
        void operator()(TTF_Font* font) const noexcept;
    };
    using UniqueFontPtr = std::unique_ptr<TTF_Font, FontDeleter>;

    struct TextDeleter {
        void operator()(TTF_Text* text) const noexcept;
    };
    using UniqueTextPtr = std::unique_ptr<TTF_Text, TextDeleter>;

    struct CursorDeleter {
        void operator()(SDL_Cursor* cursor) const noexcept;
    };
    using UniqueCursorPtr = std::unique_ptr<SDL_Cursor, CursorDeleter>;

    struct TextureDeleter final {
        void operator()(SDL_Texture* texture) const noexcept;
    };
}