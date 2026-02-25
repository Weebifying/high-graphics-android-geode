#include <Geode/Geode.hpp>
#ifdef GEODE_IS_IOS
#include <Geode/modify/CCFileUtils.hpp>

using namespace geode::prelude;

class $modify(CCFileUtils) {
    // When the texture quality is changed, the game needs to purge the CCFileUtils cache to load the new textures.
    // For some inexplicable reason, the cache isn't purged when this function isn't hooked, even though the function is called in the game's code.
    // This is a workaround to ensure the cache is purged when changing graphics settings.
    static void purgeFileUtils() {
        CCFileUtils::purgeFileUtils();
    }
};
#endif