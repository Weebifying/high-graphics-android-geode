#include <Geode/Geode.hpp>
#include <Geode/modify/OptionsLayer.hpp>
using namespace geode::prelude;

class $modify(AltOptionsLayer, OptionsLayer) {
    void customSetup() {
        OptionsLayer::customSetup();

        auto menu = this->m_mainLayer->getChildByID("options-menu");

        menu->getChildByID("options-button")->setPositionX(menu->getChildByID("account-button")->getPositionX());
        if (!menu->getChildByID("graphics-button")) {
            auto videoButton = CCMenuItemSpriteExtra::create(
                ButtonSprite::create("Graphics", 130, 0, 1.0, true, "goldFont.fnt", "GJ_button_01.png", 0.0),
                this,
                menu_selector(AltOptionsLayer::onAltVideo)
            );
            videoButton->setContentSize(menu->getChildByID("options-button")->getContentSize());
            videoButton->setPosition({menu->getChildByID("how-to-play-button")->getPositionX(), menu->getChildByID("options-button")->getPositionY()});
            videoButton->setID("graphics-button");
            menu->addChild(videoButton);
        }
    }

    void onAltVideo(CCObject* sender) {
        VideoOptionsLayer::create()->show();
    }
};