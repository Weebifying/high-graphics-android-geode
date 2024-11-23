#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
using namespace geode::prelude;
namespace fs = std::filesystem;
using ExtractTask = Task<Result<>, float>;

class HighTexturesPopup : public geode::Popup<bool> {
protected:
    bool setup(bool zipExists) override;

public:
    inline static matjson::Value m_links = matjson::makeObject({
        {"2.2074", "https://files.catbox.moe/5gc2ah.zip"}
        // {"2.2074", "http://localhost:8000/2.2074.zip"}
    });

    float m_downloadPercentage = 0.f;
    float m_extractPercentage = 0.f;
    std::string m_gameVersion;
    bool m_zipExists;

    CCSprite* m_progressBG;
    CCLabelBMFont* m_chatLabel;
    CCMenuItemSpriteExtra* m_retryBtn;
    CCMenuItemSpriteExtra* m_applyBtn;

    CCNode* m_downloadStencil;
    CCNode* m_extractStencil;
    CCLabelBMFont* m_downloadLabel;
    CCLabelBMFont* m_extractLabel;
    CCMenuItemSpriteExtra* m_downloadBtn;
    CCMenuItemSpriteExtra* m_extractBtn;

    EventListener<web::WebTask> m_downloadListener;
    EventListener<ExtractTask> m_extractListener;

    static HighTexturesPopup* create(bool zipExists);

    void onDownload(CCObject* sender);
    void onExtract(CCObject* sender);
    void onRetry(CCObject* sender);
    void onRestart(CCObject* sender);

    void setDownloadPercentage(float percentage, ccColor3B color);
    void setExtractPercentage(float percentage, ccColor3B color);

    ExtractTask getExtractTask(fs::path file, fs::path path);

    void startDownload();
    void startExtract(fs::path file, fs::path path);

    void downloadSucceeded(fs::path file, fs::path path);
    void downloadFailed(std::string reason);
    void extractSucceeded();
    void extractFailed(std::string reason);
    
};