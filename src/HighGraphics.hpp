#pragma once

#include <Geode/Geode.hpp>
#include "classes/HighTexturesPopup.hpp"
using namespace geode::prelude;
namespace fs = std::filesystem;

class HighGraphics {
public:
    HighTexturesPopup* m_popup = nullptr;
    std::string m_version;
    std::map<std::string, int> m_num = {
        {"2.2074", 2827}
    };
    bool m_success = false;
    int m_numFiles;

    static HighGraphics* get();
    int getNumFiles(fs::path path);

private:
    HighGraphics() = default;
};