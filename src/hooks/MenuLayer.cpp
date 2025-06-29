#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>
#include "../classes/HighTexturesPopup.hpp"
#include "../HighGraphics.hpp"

using namespace geode::prelude; 
namespace fs = std::filesystem;

class $modify(MenuLayer) {
    static CCScene* scene(bool videoOption) {
        auto scene = MenuLayer::scene(videoOption);

        std::string version = Loader::get()->getGameVersion();
        fs::path path = Mod::get()->getConfigDir();
        HighGraphics* manager = HighGraphics::get();
        manager->m_version = version;

        if (!fs::exists(path / version) || (manager->m_numFiles < manager->m_num[version]) || manager->m_success) {
            if (manager->m_popup == nullptr) {
                manager->m_popup = HighTexturesPopup::create(fs::exists(path / (version + ".zip")));
                manager->m_popup->setID("HighTexturesPopup");
                manager->m_popup->retain();
            }

            manager->m_popup->m_isHidden = false;
            manager->m_popup->setVisible(true);
            manager->m_popup->m_scene = scene;
            manager->m_popup->show();
        } else {
            if (!Mod::get()->getSavedValue<bool>("applied", true)) {
                auto okPopup = FLAlertLayer::create("High Textures", "High textures have been applied successfully!", "OK");
                okPopup->m_scene = scene;
                okPopup->show();
                Mod::get()->setSavedValue("applied", true);
            }
        }
    
        return scene;
    }
};