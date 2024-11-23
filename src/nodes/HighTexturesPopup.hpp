#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
using namespace geode::prelude;

class HighTexturesPopup : public geode::Popup<std::string const&> {
protected:
    bool setup(std::string const& value) override;

public:
    inline static matjson::Value m_links = matjson::makeObject({
        {"2.2074", "https://files.catbox.moe/5gc2ah.zip"}
    });

    bool m_isDownload = false;
    float m_percentage = 0.f;
    CCSprite* m_progressBG;
    CCNode* m_progressBarStencil;
    CCLabelBMFont* m_percentageLabel;
    CCLabelBMFont* m_chatLabel;
    CCMenuItemSpriteExtra* m_downloadBtn;
    CCMenuItemSpriteExtra* m_retryBtn;
    CCMenuItemSpriteExtra* m_applyBtn;

    EventListener<web::WebTask> m_listener;

    static HighTexturesPopup* create(std::string const& text);

    void onDownload(CCObject* sender);
    void onRetry(CCObject* sender);
    void onApplyTextures(CCObject* sender);
    void setPercentage(float percentage, ccColor3B color);
    void startDownload();

    void downloadSucceeded();
    void downloadFailed(std::string reason);
};