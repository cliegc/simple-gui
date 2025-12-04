#include "ui_loader/ui_file_parser.hpp"
#include "logger.hpp"


namespace SimpleGui::UILoader {
    void UIFileParser::Parse(const std::string &filePath) const {
        m_loadResult->m_cmps.clear();
        m_loadResult->m_cbGroups.clear();
        m_loadResult->m_root = nullptr;

        tinyxml2::XMLDocument doc;
        if (const auto id = doc.LoadFile(filePath.c_str());
            id != tinyxml2::XMLError::XML_SUCCESS) {
            SG_ERROR("UILoader-UIFileParser: Failed to load file {}", filePath);
            return;
        }

        tinyxml2::XMLElement* root = doc.RootElement();
        if (!root) {
            SG_ERROR("UILoader-UIFileParser: The XML document is invalid");
            return;
        }

        for (tinyxml2::XMLElement* elem = root->FirstChildElement("root");
             elem != nullptr;
             elem = elem->NextSiblingElement()) {
            // 获取元素名
            // 获取属性
        }
    }

    std::shared_ptr<LoadResult> UIFileParser::Load() const {
        return m_loadResult;
    }
}
