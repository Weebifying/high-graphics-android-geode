#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include "../HighGraphics.hpp"
using namespace geode::prelude; 
namespace fs = std::filesystem;

class $modify(LoadingLayer) {
    // load saved graphics setting instead of medium graphics on startup
    bool init(bool p0) {
        std::string version = Loader::get()->getGameVersion();
        fs::path path = Mod::get()->getConfigDir();
        HighGraphics* manager = HighGraphics::get();

        // if (fs::exists(path / version) && (manager->m_numFiles >= manager->m_num[version])) {
        if (fs::exists(path / version) && (manager->m_numFiles >= manager->m_num[version])) {
            CCDirector::get()->updateContentScale(static_cast<TextureQuality>(GameManager::sharedState()->m_texQuality));
        }
        
        return LoadingLayer::init(p0);
    }
};