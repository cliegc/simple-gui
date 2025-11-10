#include <string>
#include "math.hpp"
#include "style.hpp"
#include "component/progress_bar.hpp"
#include "component/common/types.hpp"


namespace SimpleGui {
    class TypeConverter final {
    public:
        ~TypeConverter() = default;

        static int ToInt(const std::string& str) {
            try {
                return std::stoi(str);
            }
            catch (const std::invalid_argument&) {
                return 0;
            }
        }

        static float ToFloat(const std::string& str) {
            try {
                return std::stof(str.c_str());
            }
            catch (const std::invalid_argument&) {
                return 0.f;
            }
        }

        static bool ToBool(const std::string& str) {
            return str == "true" || str == "1" || str == "True";
        }

        static Color ToColor(const std::string& str) {
            auto temp = str;
            temp.erase(std::ranges::remove(temp, ' ').begin(), temp.end());
            if (temp[0] == '#')
                return Color(temp);

            std::vector<uint8> color;
            constexpr char delimiter = ',';
            size_t start = 0;
            size_t end = temp.find(delimiter);
            int index = 0;

            while (end != std::string::npos) {
                auto clrStr = temp.substr(start, end - start);
                color[index++] = static_cast<uint8>(ToInt(clrStr));

                start = end + 1;
                end = temp.find(delimiter, start);
            }

            color[index] = static_cast<uint8>(ToInt(temp.substr(start)));

            if (color.size() >= 4) return Color(color[0], color[1], color[2], color[3]);
            if (color.size() == 3) return Color(color[0], color[1], color[2]);
            if (color.size() == 2) return Color(color[0], color[1], 0);
            return Color(color[0], 0, 0);
        }

        static ThemeColorFlags ToThemeColorFlags(const std::string& str) {

        }

        static ProgressFillMode ToProgressFillMode(const std::string& str) {

        }

        static Direction ToDirection(const std::string& str) {
            if (str == "horizontal") return Direction::Horizontal;
            return Direction::Vertical;
        }

        static TextureStretchMode ToTextureStretchMode(const std::string& str) {

        }

        static TextureScaleMode ToTextureScaleMode(const std::string& str) {

        }

    private:
        static std::unordered_map<std::string, ThemeColorFlags> s_themeColorFlagsMap;

        static void RegisterThemeColorFlags(const std::string& str, ThemeColorFlags flag) {
            s_themeColorFlagsMap.emplace(str, flag);
        }

        TypeConverter() {

        }
    };
}