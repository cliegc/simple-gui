#pragma once
#include <string>
#include <sstream>

struct SDL_FPoint;
struct SDL_Point;
struct SDL_FRect;
struct SDL_Rect;
struct SDL_Color;
struct SDL_FColor;

namespace SimpleGui {

    constexpr const float CMP_EPSILON = 0.00001f;

    bool IsEqualApprox(float left, float right);
    bool IsZeroApprox(double value);

    inline float Clamp(float value, float minValue, float maxValue) {
        return value < minValue ? minValue : (value > maxValue ? maxValue : value);
    }

    struct Vec2 final {
        union {
            float x;
            float w;
        };

        union {
            float y;
            float h;
        };

        Vec2();
        explicit Vec2(float x, float y);
        explicit Vec2(const SDL_FPoint& point);
        explicit Vec2(const SDL_Point& point);

        Vec2& operator*=(const Vec2&);
        Vec2& operator*=(float);
        Vec2& operator/=(const Vec2&);
        Vec2& operator/=(float);
        Vec2& operator+=(const Vec2&);
        Vec2& operator-=(const Vec2&);

        friend bool operator==(const Vec2&, const Vec2&);

        friend Vec2 operator*(float, const Vec2&);
        friend Vec2 operator*(const Vec2&, float);
        friend Vec2 operator*(const Vec2&, const Vec2&);
        friend Vec2 operator/(const Vec2&, float);
        friend Vec2 operator/(const Vec2&, const Vec2&);
        friend Vec2 operator+(const Vec2&, const Vec2&);
        friend Vec2 operator-(const Vec2&, const Vec2&);

        constexpr float Dot(const Vec2&) const;
        constexpr float Cross(const Vec2&) const;
        
        constexpr float LengthSquared() const;
        float Length() const;

        float DistanceSquard(const Vec2&) const;
        float Distance(const Vec2&) const;

        SDL_FPoint ToSDLFPoint() const;
        SDL_Point ToSDLPoint() const;

        Vec2 Max(const Vec2& other) const;
        Vec2 Min(const Vec2& other) const;
        Vec2 Clamp(const Vec2& min, const Vec2& max) const;
        void Clamp(const Vec2& min, const Vec2& max);
        bool IsEqualApprox(const Vec2& other) const;
        bool IsZeroApprox() const;

        Vec2 Normalize() const;
        void Normalized();
        bool IsNormalized() const;
    };

    struct Rect final {
        Vec2 position;
        Vec2 size;

        Rect();
        Rect(const Vec2& pos, const Vec2& size);
        Rect(float x, float y, float w, float h);
        explicit Rect(const SDL_FRect& rect);
        explicit Rect(const SDL_Rect& rect);

        bool ContainPoint(const Vec2& point) const;
        bool ContainRect(const Rect& rect) const;
        SDL_FRect ToSDLFRect() const;
        SDL_Rect ToSDLRect() const;

        inline constexpr float Area() const { return size.w * size.h; }

        void AdjustSizeToFitOtherRect(const Rect& other);
        void AdjustSizeToCoverOtherRect(const Rect& other);
        Rect Grow(float left, float top, float right, float bottom) const;

        inline Vec2 TopLeft() const { return position; };
        inline Vec2 TopRight() const { return Vec2(position.x + size.w, position.y); };
        inline Vec2 BottomLeft() const { return Vec2(position.x, position.y + size.h); };
        inline Vec2 BottomRight() const { return Vec2(position.x + size.w, position.y + size.h); };
        inline Vec2 Center() const { return Vec2(position.x + size.w / 2, position.y + size.h / 2); };
        inline constexpr float Left() const { return position.x; }
        inline constexpr float Top() const { return position.y; }
        inline constexpr float Right() const { return position.x + size.w; }
        inline constexpr float Bottom() const { return position.y + size.h; }

        bool IsIntersect(const Rect& rect) const;
        Rect GetIntersection(const Rect& rect) const;
    };

    using uint8 = unsigned char;
    using uint32 = unsigned int;

    struct Color final {
    public:
        static const Color TRANSPARENT;
        static const Color WHITE;
        static const Color BLACK;
        static const Color GRAY;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color YELLOW;
        static const Color MAGENTA;
        static const Color AQUA;

    public:
        uint8 r = 0;
        uint8 g = 0;
        uint8 b = 0;
        uint8 a = 255;

        Color() = default;
        Color(uint8 r, uint8 g, uint8 b, uint8 a = 255);
        Color(const std::string& code);
        Color(uint32 hex);

        bool operator==(const Color& clr);

        std::string ToColorCode(bool prefix = true) const;
        SDL_Color ToSDLColor() const;
        SDL_FColor ToSDLFColor() const;

        static Color FromFloat(float r, float g, float b, float a = 1.f);

    private:
        uint8 ParseCol4(const std::string& str, int ofs) const;
        uint8 ParseCol8(const std::string& str, int ofs) const;
        uint8 ConvertParseCol4Result(uint8 value) const;
        void ToHexString(std::stringstream& ss, uint8 value) const;
    };

    struct GradientColor final {
        enum class Type {
            Horizontal,
            Vertival,
            MainDiagonal,
            SecondaryDiagonal,
        };
        
        Color start;
        Color end;
        Type type;
    };

    struct Circle final {
        Vec2 center;
        float radius;
    };
}

