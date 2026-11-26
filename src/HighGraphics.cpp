#include "sha1.hpp"
#include "HighGraphics.hpp"

#include <asp/fs/fs.hpp>
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

int HighGraphics::getNumFiles(fs::PathView path) {
    int count = 0;

    if (path.exists()) {
        if (auto it = fs::iterDir(path)) {
            for (const auto& entry : it.unwrap()) {
                if (entry.isErr()) continue;
                if (entry.unwrap().isFile()) count++;
            }
        }

        if ((path / "icons").exists()) {
            if (auto it = fs::iterDir(path / "icons")) {
                for (const auto& entry : it.unwrap()) {
                    if (entry.isErr()) continue;
                    if (entry.unwrap().isFile()) count++;
                }
            }
        }
    }

    return count;
}

std::string HighGraphics::getChecksum(geode::Function<void(int, int, bool)> progressCallback, fs::PathView path) {
    if (!path.exists()) {
        return "";
    }

    if (path.isFile()) {
        return SHA1::from_file(path.string());
    }

    auto it = fs::iterDirRecursive(path);
    if (it.isErr()) {
        return "";
    }

    std::vector<fs::Path> files;
    for (auto const& entry : it.unwrap()) {
        if (entry.isErr()) continue;
        if (entry.unwrap().isFile()) {
            files.push_back(entry.unwrap().path);
        }
    }

    int c = 0;
    int guesstimate = 1.386f * files.size() * std::log2(files.size());

    // if only
    std::sort(files.begin(), files.end(), [&](const fs::Path& a, const fs::Path& b) {
        progressCallback(++c, guesstimate, false);
        auto ra = a.lexicallyRelative(path).value().string();
        auto rb = b.lexicallyRelative(path).value().string();
        return ra < rb;
    });


    int total = static_cast<int>(files.size());
    int processed = 0;

    SHA1 sha;
    for (const auto& fp : files) {
        fs::Path rel = *fp.lexicallyRelative(path);
        std::string relstr = rel.string();

        sha.update(relstr);
        sha.update(std::string(1, '\0'));

        std::ifstream ifs(fp.string(), std::ios::binary);
        if (!ifs) return "";
        sha.update(ifs);

        progressCallback(++processed, total, true);
    }

    return sha.final();
}

$on_mod(Loaded) {
    std::string version = Loader::get()->getGameVersion();
    fs::Path path = Mod::get()->getConfigDir();

    log::info("Game version: {}", version);
    log::info("path: {}", path.string());

    HighGraphics* manager = HighGraphics::get();
    manager->m_numFiles = manager->getNumFiles(path / version);
    // manager->m_checksum = manager->getChecksum(path / version);

    if ((path / version).exists()) {
        log::debug("Loading high graphics textures");
        // CCFileUtils::get()->addTexturePack({ "weebify.high-textures-new", { (path / version).string() } });
        CCFileUtils::get()->addPriorityPath((path / version).string().c_str()); // fuck textureldr bro
    }
}