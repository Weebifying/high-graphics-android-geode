#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
using namespace geode::prelude;
namespace fs = std::filesystem;

class HighTexturesPopup : public geode::Popup {
protected:
    bool init(bool zipExists);

public:
    inline static std::map<std::string, std::vector<std::string>> m_links = {
        // changed catbox link!! (removed geode textures lol)
        {"2.2074", {"https://github.com/Weebifying/gd-textures/releases/download/2.2074/2.2074.zip", "https://files.catbox.moe/buykym.zip", "https://archive.org/download/2.2074/2.2074.zip"}},
        {"2.208", {"https://github.com/Weebifying/gd-textures/releases/download/2.208/2.208.zip", "https://files.catbox.moe/jndvgj.zip", "https://archive.org/download/2.208/2.208.zip"}},
        {"2.2081", {"https://github.com/Weebifying/gd-textures/releases/download/2.2081/2.2081.zip", "https://files.catbox.moe/dn3ce1.zip", "https://archive.org/download/2.2081/2.2081.zip"}}
    };

    float m_downloadPercentage = 0.f;
    float m_extractPercentage = 0.f;
    float m_checksumPercentage = 0.f;
    std::string m_gameVersion;
    bool m_zipExists;
    bool m_isHidden;
    bool m_finished = false;

    CCMenu* m_menu;
    CCSprite* m_progressBG;
    CCLabelBMFont* m_chatLabel;
    CCMenuItemSpriteExtra* m_retryBtn;
    CCMenuItemSpriteExtra* m_restartBtn;
    CCMenuItemSpriteExtra* m_hideBtn;

    CCNode* m_downloadStencil;
    CCNode* m_extractStencil;
    CCNode* m_checksumStencil;
    CCLabelBMFont* m_downloadLabel;
    CCLabelBMFont* m_extractLabel;
    CCLabelBMFont* m_checksumLabel;
    CCMenuItemSpriteExtra* m_downloadBtn;
    CCMenuItemSpriteExtra* m_extractBtn;

    async::TaskHolder<web::WebResponse> m_downloadListener;

    static HighTexturesPopup* create(bool zipExists);
    CCMenuItemSpriteExtra* createButton(const char* text, float width, const char* sprite, std::string id, SEL_MenuHandler selector);
    void keyDown(cocos2d::enumKeyCodes key, double delta) override;

    void onDownload(CCObject* sender);
    void onExtract(CCObject* sender);
    void onRetry(CCObject* sender);
    void onRestart(CCObject* sender);
    void onHide(CCObject* sender);

    void setDownloadPercentage(float percentage, ccColor3B color);
    void setExtractPercentage(float percentage, ccColor3B color);
    void setChecksumPercentage(float percentage, ccColor3B color);

    void startDownload();
    void startExtract(fs::path file, fs::path path);
    void startChecksum(fs::path path);

    void downloadSucceeded(fs::path file, fs::path path);
    void downloadFailed(std::string reason);
    void extractSucceeded(fs::path path);
    void extractFailed(std::string reason);
    void checksumSucceeded();
    void checksumFailed(std::string reason);
    void notifySuccess();
    void notifyFailure(std::string which, std::string reason);

    void handleDownloadFinish(web::WebResponse res, std::shared_ptr<int> num, fs::path path);
    arc::Future<Result<void, std::string>> getExtractTask(fs::path file, fs::path path);
    arc::Future<std::string> getChecksumTask(fs::path path);
};