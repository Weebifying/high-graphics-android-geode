#include "HighGraphics.hpp"

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

$on_mod(Loaded) {
    std::string version = Mod::get()->getMetadata().getGameVersion().value();
    fs::path path = Mod::get()->getConfigDir();

    HighGraphics* manager = HighGraphics::get();
    manager->m_numFiles = manager->getNumFiles(path / version);

    if (fs::exists(path / version)) {
        log::debug("Loading high graphics textures");
        // CCFileUtils::get()->addTexturePack({ "weebify.high-textures-new", { (path / version).string() } });
        CCFileUtils::get()->addPriorityPath((path / version).string().c_str()); // fuck textureldr bro
    }
}