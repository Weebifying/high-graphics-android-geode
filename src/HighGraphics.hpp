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
        {"2.2074", 2827},
        {"2.208", 2822},
        {"2.2081", 2822}
    };
    std::map<std::string, std::string> m_checksums = {
        {"2.2074", "abb0d92b4294528d2996def8344b9c21538044a4"},
        {"2.208", "d098cff446863822b9d192811e2961d3920a42d6"},
        {"2.2081", "cd7a43e0b96b4a41477eab217659ac8f9fc9c91d"}
    };
    bool m_success = false;
    int m_numFiles;
    std::string m_checksum;

    static HighGraphics* get();
    int getNumFiles(fs::path path);
    std::string getChecksum(geode::Function<void(int, int, bool)> progressCallback, fs::path path);

private:
    HighGraphics() = default;
};