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

        auto menu = this->m_mainLayer->getChildByID("options-menu");

        menu->getChildByID("options-button")->setPositionX(menu->getChildByID("account-button")->getPositionX());
        if (!menu->getChildByID("graphics-button")) {
            auto videoButton = CCMenuItemSpriteExtra::create(
                ButtonSprite::create("Graphics", 130, 0, 1.0, true, "goldFont.fnt", "GJ_button_01.png", 0.0),
                this,
                menu_selector(AltOptionsLayer::onVideo)
            );
            videoButton->setContentSize(menu->getChildByID("options-button")->getContentSize());
            videoButton->setPosition({menu->getChildByID("how-to-play-button")->getPositionX(), menu->getChildByID("options-button")->getPositionY()});
            videoButton->setID("graphics-button");
            menu->addChild(videoButton);
        }
    }

    void onVideo(CCObject* sender) {
        VideoOptionsLayer::create()->show();    
    }
};

auto links = matjson::makeObject({
    {"2.2074", "https://files.catbox.moe/5gc2ah.zip"}
});

class $modify(MenuLayer) {
    struct Fields {
        EventListener<web::WebTask> m_listener;
    };

    void applyTexturePack(fs::path path) {
        CCFileUtils::get()->addTexturePack({ "weebify.high-textures", { path.string() } });
    }

    bool init() {
        if (!MenuLayer::init()) return false;

        std::string version = Mod::get()->getMetadata().getGameVersion().value();
        fs::path path = Mod::get()->getConfigDir();
        if (!fs::exists(path / version)) {
            m_fields->m_listener.bind([=] (web::WebTask::Event* e) {
                if (web::WebResponse* res = e->getValue()) {
                    fs::path file = path / (version + ".zip");
                    if (res->into(file)) {
                        if (file::Unzip::intoDir(file, path, false)) {
                            applyTexturePack(path / version);
                        } else {
                            log::error("failed to extract zip data");
                        }
                    } else {
                        log::error("failed to transfer data to file");
                    }
                } else if (web::WebProgress* p = e->getProgress()) {
                    log::warn("progress: {}", p->downloadProgress().value_or(0.f));
                } else if (e->isCancelled()) {
                    log::error("download failed...");
                }
            });

            web::WebRequest req = web::WebRequest();
            req.timeout(std::chrono::seconds(300));
            m_fields->m_listener.setFilter(req.get(links[version].asString().unwrap()));
        } else {
            applyTexturePack(path / version);
        }
        

        return true;
    }
};