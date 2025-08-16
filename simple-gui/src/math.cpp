#include "math.hpp"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_pixels.h>

namespace SimpleGui {

    bool IsEqualApprox(float left, float right) {
        if (left == right) {
            return true;
        }

        float tolerance = (float)CMP_EPSILON * abs(left);
        if (tolerance < (float)CMP_EPSILON) {
            tolerance = (float)CMP_EPSILON;
        }

        return abs(left - right) < tolerance;
    }

    bool IsZeroApprox(double value) {
        return abs(value) < CMP_EPSILON;
    }

#pragma region Vec
    Vec2::Vec2() : x(0), y(0) {
    }

    Vec2::Vec2(float x, float y) : x(x), y(y) {
    }

    Vec2::Vec2(const SDL_FPoint& point) : x(point.x), y(point.y) {
    }

    Vec2::Vec2(const SDL_Point& point) : x(static_cast<float>(point.x)), y(static_cast<float>(point.y)) {
    }

    Vec2& Vec2::operator*=(const Vec2& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    Vec2& Vec2::operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vec2& Vec2::operator/=(const Vec2& other) {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    Vec2& Vec2::operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    Vec2& Vec2::operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2& Vec2::operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    bool operator==(const Vec2& a, const Vec2& b) {
        return a.x == b.x && a.y == b.y;
    }

    Vec2 operator*(float value, const Vec2& vec2) {
        return Vec2(value * vec2.x, value * vec2.y);
    }

    Vec2 operator*(const Vec2& vec2, float value) {
        return Vec2(value * vec2.x, value * vec2.y);
    }

    Vec2 operator*(const Vec2& v1, const Vec2& v2) {
        return Vec2(v1.x * v2.x, v1.y * v2.y);
    }

    Vec2 operator/(const Vec2& vec2, float value) {
        return Vec2(vec2.x / value, vec2.y / value);
    }

    Vec2 operator/(const Vec2& v1, const Vec2& v2) {
        return Vec2(v1.x / v2.x, v1.y / v2.y);
    }

    Vec2 operator+(const Vec2& v1, const Vec2& v2) {
        return Vec2(v1.x + v2.x, v1.y + v2.y);
    }

    Vec2 operator-(const Vec2& v1, const Vec2& v2) {
        return Vec2(v1.x - v2.x, v1.y - v2.y);
    }

    float Vec2::Dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    float Vec2::Cross(const Vec2& other) const {
        return x * other.y - y * other.x;
    }

    float Vec2::LengthSquared() const {
        return x * x + y * y;
    }

    float Vec2::Length() const {
        return std::sqrt(LengthSquared());
        ;
    }

    SDL_FPoint Vec2::ToSDLFPoint() const {
        return { x, y };
    }

    SDL_Point Vec2::ToSDLPoint() const {
        return { static_cast<int>(x), static_cast<int>(y) };
    }

    Vec2 Vec2::Max(const Vec2& other) {
        return Vec2(
            fmaxf(x, other.x),
            fmaxf(y, other.y));
    }

    Vec2 Vec2::Min(const Vec2& other) {
        return Vec2(
            fminf(x, other.x),
            fminf(y, other.y));
    }

    bool Vec2::IsEqualApprox(const Vec2& other) const {
        return SimpleGui::IsEqualApprox(x, other.x) &&
            SimpleGui::IsEqualApprox(y, other.y);
    }

    bool Vec2::IsZeroApprox() const {
        return SimpleGui::IsZeroApprox(x) &&
            SimpleGui::IsZeroApprox(y);
    }
#pragma endregion

#pragma region Rect
    Rect::Rect() : position(), size() {
    }

    Rect::Rect(const Vec2& pos, const Vec2& size) : position(pos), size(size) {
    }

    Rect::Rect(float x, float y, float w, float h) : position(x, y), size(w, h) {
    }

    Rect::Rect(const SDL_FRect& rect) : position(rect.x, rect.y), size(rect.w, rect.h) {
    }

    Rect::Rect(const SDL_Rect& rect) : 
        position(static_cast<float>(rect.x), static_cast<float>(rect.y)), 
        size(static_cast<float>(rect.w), static_cast<float>(rect.h)) {
    }

    bool Rect::ContainPoint(const Vec2& point) const {
        return (point.x >= position.x && point.x <= (position.x + size.w)) &&
            (point.y >= position.y && point.y <= (position.y + size.h));
    }

    bool Rect::ContainRect(const Rect& rect) const {
        return ContainPoint(rect.position) && ContainPoint(rect.position + rect.size);
    }

    SDL_FRect Rect::ToSDLFRect() const {
        return { position.x, position.y, size.w, size.h };
    }

    SDL_Rect Rect::ToSDLRect() const {
        return { static_cast<int>(position.x), static_cast<int>(position.y),
                static_cast<int>(size.w), static_cast<int>(size.h) };
    }

    void Rect::AdjustSizeToFitOtherRect(const Rect& other) {
        float innerAspect = (size.h > 0) ? size.w / size.h : 1.0f;
        float outerAspect = (other.size.h > 0) ? other.size.w / other.size.h : 1.0f;

        float scale;
        if (innerAspect > outerAspect) {
            scale = other.size.w / size.w;
        }
        else {
            scale = other.size.h / size.h;
        }

        size.w *= scale;
        size.h *= scale;

        position.x = other.position.x + (other.size.w - size.w) / 2;
        position.y = other.position.y + (other.size.h - size.h) / 2;
    }

    void Rect::AdjustSizeToCoverOtherRect(const Rect& other) {
        float aspectRatio = (size.h > 0) ? size.w / size.h : 1.0f;
        float targetRatio = (other.size.h > 0) ? other.size.w / other.size.h : 1.0f;

        float newWidth, newHeight;

        if (aspectRatio >= targetRatio) {
            newHeight = other.size.h;
            newWidth = other.size.h * aspectRatio;
        }
        else {
            newWidth = other.size.w;
            newHeight = other.size.w / aspectRatio;
        }

        size.w = newWidth;
        size.h = newHeight;

        position.x = other.position.x + (other.size.w - size.w) / 2;
        position.y = other.position.y + (other.size.h - size.h) / 2;
    }

    Rect Rect::Grow(float left, float top, float right, float bottom) const {
        return Rect(
            position.x - left,
            position.y - top,
            size.w + right,
            size.h + bottom);
    }

    bool Rect::IsIntersect(const Rect& rect) const {
        if (position.x > (rect.position.x + rect.size.w)) {
            return false;
        }
        if ((position.x + size.w) < rect.position.x) {
            return false;
        }
        if (position.y > (rect.position.y + rect.size.h)) {
            return false;
        }
        if ((position.y + size.h) < rect.position.y) {
            return false;
        }

        return true;
    }

    Rect Rect::GetIntersection(const Rect& rect) const {
        Rect newRect = rect;

        if (!IsIntersect(newRect)) {
            return Rect();
        }

        newRect.position = newRect.position.Max(position);

        Vec2 rectEnd = rect.BottomRight();
        Vec2 end = BottomRight();

        newRect.size = rectEnd.Min(end) - newRect.position;

        return newRect;
    }
#pragma endregion

#pragma region Color
    Color::Color(uint8 r, uint8 g, uint8 b, uint8 a) : r(r), g(g), b(b), a(a) {
    }

    Color::Color(const std::string& code) {
        if (code.empty()) {
            return;
        }

        const int currPos = (code[0] == '#') ? 1 : 0;
        const int numOfDigits = code.length() - currPos;

        if (numOfDigits == 3) {
            r = ConvertParseCol4Result(ParseCol4(code, currPos));
            g = ConvertParseCol4Result(ParseCol4(code, currPos + 1));
            b = ConvertParseCol4Result(ParseCol4(code, currPos + 2));
        }
        else if (numOfDigits == 4) {
            r = ConvertParseCol4Result(ParseCol4(code, currPos));
            g = ConvertParseCol4Result(ParseCol4(code, currPos + 1));
            b = ConvertParseCol4Result(ParseCol4(code, currPos + 2));
            a = ConvertParseCol4Result(ParseCol4(code, currPos + 3));
        }
        else if (numOfDigits == 6) {
            r = ParseCol8(code, currPos);
            g = ParseCol8(code, currPos + 2);
            b = ParseCol8(code, currPos + 4);
        }
        else if (numOfDigits == 8) {
            r = ParseCol8(code, currPos);
            g = ParseCol8(code, currPos + 2);
            b = ParseCol8(code, currPos + 4);
            a = ParseCol8(code, currPos + 6);
        }
    }

    Color::Color(uint32 hex) {
        a = (hex & 0xFF);
        hex >>= 8;
        b = (hex & 0xFF);
        hex >>= 8;
        g = (hex & 0xFF);
        hex >>= 8;
        r = (hex & 0xFF);
    }

    bool Color::operator==(const Color& clr) {
        return r == clr.r && g == clr.g && b == clr.b && a == clr.a;
    }

    std::string Color::ToColorCode(bool prefix) const {
        std::stringstream ss;

        if (prefix)
            ss << "#";
        ToHexString(ss, r);
        ToHexString(ss, g);
        ToHexString(ss, b);
        ToHexString(ss, a);

        return ss.str();
    }

    SDL_Color Color::ToSDLColor() const {
        return { r, g, b, a };
    }

    uint8 Color::ParseCol4(const std::string& str, int ofs) const {
        char character = str[ofs];

        if (character >= '0' && character <= '9') {
            return character - '0';
        }
        else if (character >= 'a' && character <= 'f') {
            return character + (10 - 'a');
        }
        else if (character >= 'A' && character <= 'F') {
            return character + (10 - 'A');
        }

        return -1;
    }

    uint8 Color::ParseCol8(const std::string& str, int ofs) const {
        return ParseCol4(str, ofs) * 16 + ParseCol4(str, ofs + 1);
    }

    uint8 Color::ConvertParseCol4Result(uint8 value) const {
        return static_cast<uint8>(lround(value / 15.0f * 255.0f));
    }

    void Color::ToHexString(std::stringstream& ss, uint8 value) const {
        if (value < 10) {
            ss << 0 << value;
        }
        else {
            ss << std::hex << value;
        }
    }

    const Color Color::WHITE{ 255, 255, 255 };
    const Color Color::BLACK{ 0, 0, 0 };
    const Color Color::RED{ 255, 0, 0 };
    const Color Color::GREEN{ 0, 255, 0 };
    const Color Color::BLUE{ 0, 0, 255 };
    const Color Color::YELLOW{ 255, 255, 0 };
    const Color Color::PURPLE{ 255, 0, 255 };
    const Color Color::AQUA{ 0, 255, 255 };
#pragma endregion
}