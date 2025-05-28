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
    std::string version = Loader::get()->getGameVersion();
    fs::path path = Mod::get()->getConfigDir();

    fs::path c1 = dirs::getModConfigDir();
    fs::path c2 = c1 / Mod::get()->getID();
    fs::path c3 = c1 / "balls.zip";
    log::debug("balls!!! c1 = {}", c1.string());
    log::debug("balls!!! c2 = {}", c2.string());
    log::debug("balls!!! c3 = {}", c3.string());
    log::debug("balls!!! id = {}", Mod::get()->getID() );
    log::debug("balls!!! path = {}", path.string());

    HighGraphics* manager = HighGraphics::get();
    manager->m_numFiles = manager->getNumFiles(path / version);

    if (fs::exists(path / version)) {
        log::debug("Loading high graphics textures");
        // CCFileUtils::get()->addTexturePack({ "weebify.high-textures-new", { (path / version).string() } });
        CCFileUtils::get()->addPriorityPath((path / version).string().c_str()); // fuck textureldr bro
    }
}