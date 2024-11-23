#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>
using namespace geode::prelude; 

class $modify(LoadingLayer) {
    // load saved graphics setting instead of medium graphics on startup
    bool init(bool p0) {
        if (GameManager::sharedState()->m_texQuality == 1) {
                CCDirector::get()->updateContentScale(TextureQuality::kTextureQualityLow);
        }
        if (GameManager::sharedState()->m_texQuality == 3) {
            CCDirector::get()->updateContentScale(TextureQuality::kTextureQualityHigh);
        }
        
        return LoadingLayer::init(p0);
    }
};