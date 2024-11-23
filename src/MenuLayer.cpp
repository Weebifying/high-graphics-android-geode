#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>
#include "classes/HighTexturesPopup.hpp"

using namespace geode::prelude; 
namespace fs = std::filesystem;

class $modify(MenuLayer) {
    static void onModify(auto& self) {
        (void) self.setHookPriority("MenuLayer::init", INT_MIN + 1);
    }
    
    bool init() {
        if (!MenuLayer::init()) return false;

        std::string version = Mod::get()->getMetadata().getGameVersion().value();
        fs::path path = Mod::get()->getConfigDir();

        if (!fs::exists(path / version)) {
            auto popup = HighTexturesPopup::create(fs::exists(path / (version + ".zip")));
            popup->m_scene = this;
            popup->show();
        } else {
            if (!Mod::get()->getSavedValue<bool>("applied", false)) {
                auto okPopup = FLAlertLayer::create("High Textures", "High textures have been applied successfully!", "OK");
                okPopup->m_scene = this;
                okPopup->show();
                Mod::get()->setSavedValue("applied", true);
            }
        }

        return true;
    }
};

$on_mod(Loaded) {
    std::string version = Mod::get()->getMetadata().getGameVersion().value();
    fs::path path = Mod::get()->getConfigDir();

    if (fs::exists(path / version)) {
        log::debug("Loading high graphics textures");
        CCFileUtils::get()->addTexturePack({ "weebify.high-textures", { (path / version).string() } });
    }
}