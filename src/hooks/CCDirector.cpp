#include <Geode/Geode.hpp>
#include <Geode/modify/CCDirector.hpp>
using namespace geode::prelude;

class $modify(CCDirector) {
    // force the game to use high graphics
    void updateContentScale(TextureQuality p0) {
        CCDirector::updateContentScale(p0);
        if (p0 == TextureQuality::kTextureQualityHigh) {
            this->m_fContentScaleFactor = 4.f;
            this->m_eTextureQuality = TextureQuality::kTextureQualityHigh;
        }
    }
};