#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/OptionsLayer.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/CCDirector.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude; 
namespace fs = std::filesystem;


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

class $modify(AltOptionsLayer, OptionsLayer) {
    void customSetup() {
        OptionsLayer::customSetup();

        auto menu = this->getChildByID("main-layer")->getChildByID("options-menu");

        float accountX = menu->getChildByID("account-button")->getPositionX();
        float helpX = menu->getChildByID("how-to-play-button")->getPositionX();
        float optionsY = menu->getChildByID("options-button")->getPositionY();
        CCSize optionsSize = menu->getChildByID("options-button")->getContentSize();

        menu->getChildByID("options-button")->setPositionX(accountX);

        auto videoSprite = ButtonSprite::create("Graphics",130, 0, 1.0, true, "goldFont.fnt", "GJ_button_01.png", 0.0);
        auto videoButton = CCMenuItemSpriteExtra::create(videoSprite, this, menu_selector(AltOptionsLayer::onVideo));
        
        videoButton->setContentSize(optionsSize);
        videoButton->setPosition({helpX, optionsY});
        videoButton->setID("graphics-button");
        menu->addChild(videoButton);
    }

    void onVideo(CCObject* sender) {
        VideoOptionsLayer::create()->show();    
    }
};

bool highLoaded = false;
bool hasWarned = false;

class $modify (MenuLayer) {
    // popup for if you havent applied the high-textures pack
    bool init() {
        if (!MenuLayer::init()) return false;

        for (auto& p : CCFileUtils::sharedFileUtils()->getSearchPaths()) {

            auto path = fs::path(p.c_str()).parent_path();

            if (strcmp(path.parent_path().filename().string().c_str(), "packs") == 0) {

                if (fs::exists(path / "pack.json")) {

                    std::ifstream packjson((path / "pack.json").string());
                    std::string content( (std::istreambuf_iterator<char>(packjson) ),
                                         (std::istreambuf_iterator<char>(        ) )
                    );

                    if (strcmp(matjson::parse(content)["id"].as_string().c_str(), "weebify.high-textures") == 0) {
                        highLoaded = true;
                        break;
                    }
                }
            }
        }


        if (!highLoaded) {
            if (!hasWarned) {
                hasWarned = true;
                geode::Loader::get()->queueInMainThread([] {
                    if (!Mod::get()->getSettingValue<bool>("disable-popup")) {
                        geode::createQuickPopup("Woops!",
                            "It looks like you haven't loaded the <cr>default high graphics textures</c> yet. If you haven't, please download the high textures zip file <cl>using the download button below</c>, <co>unzip it</c> and <cr>load it using Texture Loader</c>. You can disable this popup in the mod's setting page",
                            "CANCEL", "DOWNLOAD",
                            [](auto, bool btn2) {
                                if (btn2) {
                                    geode::utils::web::openLinkInBrowser("https://drive.google.com/drive/folders/1RwM4Nm0ZoFAq0ugGS-YXCA2XPXzGB9oS?usp=sharing");
                                }
                            }
                        );
                    }
                });
            }
        }

        return true;
    }
};