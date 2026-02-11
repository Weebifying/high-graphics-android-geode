#include "sha1.hpp"
#include "HighGraphics.hpp"

#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <cmath>

HighGraphics* HighGraphics::get() {
    static HighGraphics instance;
    return &instance;
}

int HighGraphics::getNumFiles(fs::path path) {
    int count = 0;

    if (fs::exists(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file()) count++;
        }

        if (fs::exists(path / "icons")) {
            for (const auto& entry : fs::directory_iterator(path / "icons")) {
                if (entry.is_regular_file()) count++;
            }
        }
    }

    return count;
}

std::string HighGraphics::getChecksum(geode::Function<void(int, int, bool)> progressCallback, fs::path path) {
    if (!fs::exists(path)) {
        return "";
    }

    if (fs::is_regular_file(path)) {
        return SHA1::from_file(path.string());
    }

    std::vector<fs::path> files;
    for (auto const& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path());
        }
    }

    int c = 0;
    int guesstimate = 1.386f * files.size() * std::log2(files.size());

    // if only
    std::sort(files.begin(), files.end(), [&](const fs::path& a, const fs::path& b) {
        progressCallback(++c, guesstimate, false);
        auto ra = fs::relative(a, path).generic_string();
        auto rb = fs::relative(b, path).generic_string();
        return ra < rb;
    });


    int total = static_cast<int>(files.size());
    int processed = 0;

    SHA1 sha;
    for (const auto& fp : files) {
        fs::path rel = fs::relative(fp, path);
        std::string relstr = rel.generic_string();

        sha.update(relstr);
        sha.update(std::string(1, '\0'));

        std::ifstream ifs(fp, std::ios::binary);
        if (!ifs) return "";
        sha.update(ifs);

        progressCallback(++processed, total, true);
    }

    return sha.final();
}

$on_mod(Loaded) {
    std::string version = Loader::get()->getGameVersion();
    fs::path path = Mod::get()->getConfigDir();

    log::info("Game version: {}", version);
    log::info("path: {}", path.string());

    HighGraphics* manager = HighGraphics::get();
    manager->m_numFiles = manager->getNumFiles(path / version);
    // manager->m_checksum = manager->getChecksum(path / version);

    if (fs::exists(path / version)) {
        log::debug("Loading high graphics textures");
        // CCFileUtils::get()->addTexturePack({ "weebify.high-textures-new", { (path / version).string() } });
        CCFileUtils::get()->addPriorityPath((path / version).string().c_str()); // fuck textureldr bro
    }
}