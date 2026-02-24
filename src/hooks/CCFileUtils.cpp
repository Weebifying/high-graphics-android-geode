#include <Geode/Geode.hpp>
#ifdef GEODE_IS_IOS
#include <Geode/modify/CCFileUtils.hpp>

using namespace geode::prelude;

class $modify(CCFileUtils) {
    static void purgeFileUtils() {
        CCFileUtils::purgeFileUtils();
    }
};
#endif