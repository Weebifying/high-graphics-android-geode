#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/OptionsLayer.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/CCDirector.hpp>

using namespace geode::prelude;

class $modify(LoadingLayer) {
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

class $modify(CCDirector) {
    void updateContentScale(TextureQuality p0) {
        CCDirector::updateContentScale(p0);
        if (p0 == TextureQuality::kTextureQualityHigh) {
            this->m_fContentScaleFactor = 4.f;
            this->m_eTextureQuality = TextureQuality::kTextureQualityHigh;
        }
    }
};

class $modify(AltOptionsLayer, OptionsLayer) {
    void customSetup() {
        OptionsLayer::customSetup();

        // get the menu containing the buttons
        CCArrayExt<CCNode*> children = typeinfo_cast<CCLayer*>(this->getChildren()->objectAtIndex(0))->getChildren();
        CCMenu* menu = nullptr;

        for (auto* child: children) {
            if (child->getChildrenCount() == 6 || child->getChildrenCount() == 7) {
                menu = typeinfo_cast<CCMenu*>(child);
                break;
            }
        }

        float accountX;
        float helpX;
        float optionsY;
        CCSize optionsSize;

        // extremely cursed bc i cant be bothered to do nodeids
        CCArrayExt<CCMenuItemSpriteExtra*> buttons = menu->getChildren();
        for (auto* button: buttons) {
            auto sprite = typeinfo_cast<ButtonSprite*>(button->getChildren()->objectAtIndex(0));

            if (strcmp(getChildOfType<CCLabelBMFont>(sprite, 0)->getString(), "Account") == 0) {
                accountX = button->getPositionX();
            } else if (strcmp(getChildOfType<CCLabelBMFont>(sprite, 0)->getString(), "How To Play") == 0)  {
                helpX = button->getPositionX();
            } else if (strcmp(getChildOfType<CCLabelBMFont>(sprite, 0)->getString(), "Options") == 0) {
                button->setPositionX(accountX);
                optionsY = button->getPositionY();
                optionsSize = button->getContentSize();
            }
        }

        auto videoSprite = ButtonSprite::create("Graphics",130, 0, 1.0, true, "goldFont.fnt", "GJ_button_01.png", 0.0);
        auto videoButton = CCMenuItemSpriteExtra::create(videoSprite, this, menu_selector(AltOptionsLayer::onVideo));
        
        videoButton->setContentSize(optionsSize);
        videoButton->setPosition({helpX, optionsY});
        menu->addChild(videoButton);
    }

    void onVideo(CCObject* sender) {
        VideoOptionsLayer::create()->show();
    }
};
