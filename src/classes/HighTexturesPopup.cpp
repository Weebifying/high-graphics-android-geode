#include "HighTexturesPopup.hpp"
#include "../HighGraphics.hpp"
using namespace geode::utils::file;

CCMenuItemSpriteExtra* HighTexturesPopup::createButton(const char* text, float width, const char* sprite, std::string id, SEL_MenuHandler selector) {
    CCSize size = m_mainLayer->getContentSize();

    auto spr = ButtonSprite::create(text, width, true, "bigFont.fnt", sprite, 30, 0.65f);
    auto btn = CCMenuItemSpriteExtra::create(spr, this, selector);
    btn->setVisible(false);
    btn->setPosition({ size.width/2, 25 });
    btn->setContentSize({ width, 30 });
    spr->setPosition(btn->getContentSize()/2);
    btn->setID(id);
    m_menu->addChild(btn);

    return btn;
}

void HighTexturesPopup::keyDown(cocos2d::enumKeyCodes key, double delta) {
    if (key == cocos2d::enumKeyCodes::KEY_Escape) return;
    if (m_finished) {
        if (key == cocos2d::enumKeyCodes::KEY_Space) {
            Mod::get()->setSavedValue("applied", false);
            game::restart(true);
            return;
        }
    }
    Popup::keyDown(key, delta);
}

bool HighTexturesPopup::init(bool zipExists) {
    if (!geode::Popup::init(360.f, 200.f)) {
        return false;
    }

    this->setTitle("High Textures");

    CCSize size = m_mainLayer->getContentSize();
    m_gameVersion = Loader::get()->getGameVersion();

    auto chatLabel = CCLabelBMFont::create("Looks like you have yet to download the high graphics textures necessary for this mod. Please download it now for the best experience. (File size: 107MB)", "chatFont.fnt");
    chatLabel->setPosition({ size.width/2, size.height - 70 });
    chatLabel->setScale(0.9f);
    chatLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    chatLabel->setWidth(size.width - 10.f);
    chatLabel->setID("chat-label");
    m_mainLayer->addChild(chatLabel);
    m_chatLabel = chatLabel;

    auto menu = CCMenu::create();
    menu->setPosition({ 0, 0 });
    menu->setContentSize(size);
    menu->setID("buttons-menu");
    m_mainLayer->addChild(menu);
    m_menu = menu;

    m_downloadBtn = createButton("Download", 104, "GJ_button_01.png", "download-button", menu_selector(HighTexturesPopup::onDownload));
    m_extractBtn = createButton("Extract", 104, "GJ_button_02.png", "extract-button", menu_selector(HighTexturesPopup::onExtract));
    m_retryBtn = createButton("Retry", 104, "GJ_button_04.png", "retry-button", menu_selector(HighTexturesPopup::onRetry));
    m_restartBtn = createButton("Restart", 104, "GJ_button_01.png", "restart-button", menu_selector(HighTexturesPopup::onRestart));
    m_hideBtn = createButton("Run in Background", 180, "GJ_button_03.png", "hide-button", menu_selector(HighTexturesPopup::onHide));

    auto progressBG = CCSprite::create("GJ_progressBar_001.png");
    progressBG->setVisible(false);
    progressBG->setPosition({ size.width/2, 60 });
    progressBG->setScale(0.75f);
    progressBG->setColor({ 0, 0, 0 });
    progressBG->setOpacity(125);
    progressBG->setID("progress-bar-background");
    m_mainLayer->addChild(progressBG);
    m_progressBG = progressBG;

    auto downloadBar = CCSprite::create("GJ_progressBar_001.png");
    downloadBar->setScale(0.992f);
    downloadBar->setScaleY(0.86f);
    downloadBar->setPosition(progressBG->getContentSize() / 2);
    downloadBar->setColor({ 0, 255, 0 });
    downloadBar->setID("download-bar");

    auto downloadStencil = CCScale9Sprite::create("square02_001.png");
	downloadStencil->setAnchorPoint({ 0, 0.5f });
	downloadStencil->setContentWidth(downloadBar->getContentWidth());
    downloadStencil->setContentHeight(100);
    downloadStencil->setScaleX(m_downloadPercentage / 100);
    m_downloadStencil = downloadStencil;

    auto downloadClipNode = CCClippingNode::create();
    downloadClipNode->setStencil(downloadStencil);
    downloadClipNode->setAnchorPoint({ 0, 0 });
	downloadClipNode->setPosition({ 0, 0 });
	downloadClipNode->setContentSize(progressBG->getContentSize());
    downloadClipNode->setID("download-bar-clipping-node");

	downloadClipNode->addChild(downloadBar);
    progressBG->addChild(downloadClipNode, 1);

    auto extractBar = CCSprite::create("GJ_progressBar_001.png");
    extractBar->setScale(0.992f);
    extractBar->setScaleY(0.86f);
    extractBar->setPosition(progressBG->getContentSize() / 2);
    extractBar->setColor({ 0, 255, 255 });
    extractBar->setID("extract-bar");

    auto extractStencil = CCScale9Sprite::create("square02_001.png");
	extractStencil->setAnchorPoint({ 0, 0.5f });
	extractStencil->setContentWidth(extractBar->getContentWidth());
    extractStencil->setContentHeight(100);
    extractStencil->setScaleX(m_extractPercentage / 100);
    m_extractStencil = extractStencil;

    auto extractClipNode = CCClippingNode::create();
    extractClipNode->setStencil(extractStencil);
    extractClipNode->setAnchorPoint({ 0, 0 });
	extractClipNode->setPosition({ 0, 0 });
	extractClipNode->setContentSize(progressBG->getContentSize());
    extractClipNode->setID("extract-bar-clipping-node");

	extractClipNode->addChild(extractBar);
    progressBG->addChild(extractClipNode, 2);

    auto checksumBar = CCSprite::create("GJ_progressBar_001.png");
    checksumBar->setScale(0.992f);
    checksumBar->setScaleY(0.86f);
    checksumBar->setPosition(progressBG->getContentSize() / 2);
    checksumBar->setColor({ 255, 255, 0 });
    checksumBar->setID("checksum-bar");

    auto checksumStencil = CCScale9Sprite::create("square02_001.png");
    checksumStencil->setAnchorPoint({ 0, 0.5f });
    checksumStencil->setContentWidth(checksumBar->getContentWidth());
    checksumStencil->setContentHeight(100);
    checksumStencil->setScaleX(m_checksumPercentage / 100);
    m_checksumStencil = checksumStencil;

    auto checksumClipNode = CCClippingNode::create();
    checksumClipNode->setStencil(checksumStencil);
    checksumClipNode->setAnchorPoint({ 0, 0 });
    checksumClipNode->setPosition({ 0, 0 });
    checksumClipNode->setContentSize(progressBG->getContentSize());
    checksumClipNode->setID("checksum-bar-clipping-node");

    checksumClipNode->addChild(checksumBar);
    progressBG->addChild(checksumClipNode, 3);


    auto downloadLabel = CCLabelBMFont::create(fmt::format("{:.2f}%", m_downloadPercentage).c_str(), "bigFont.fnt");
    downloadLabel->setVisible(false);
    downloadLabel->setPosition({ 180, progressBG->getPositionY() + 20});
    downloadLabel->setScale(0.5f);
    downloadLabel->setID("download-label");
    m_mainLayer->addChild(downloadLabel);
    m_downloadLabel = downloadLabel;

    auto extractLabel = CCLabelBMFont::create(fmt::format("{:.2f}%", m_extractPercentage).c_str(), "bigFont.fnt");
    extractLabel->setVisible(false);
    extractLabel->setPosition({ 230, progressBG->getPositionY() + 20});
    extractLabel->setScale(0.5f);
    extractLabel->setID("extract-label");
    m_mainLayer->addChild(extractLabel);
    m_extractLabel = extractLabel;

    auto checksumLabel = CCLabelBMFont::create(fmt::format("{:.2f}%", m_checksumPercentage).c_str(), "bigFont.fnt");
    checksumLabel->setVisible(false);
    checksumLabel->setPosition({ 270, progressBG->getPositionY() + 20});
    checksumLabel->setScale(0.5f);
    checksumLabel->setID("checksum-label");
    m_mainLayer->addChild(checksumLabel);
    m_checksumLabel = checksumLabel;

    if (zipExists) {
        m_extractBtn->setVisible(true);
        chatLabel->setString("Looks like you have downloaded the high textures zip file, but yet to extract it. Please extract it now for the best experience.");
        setDownloadPercentage(100.f, { 0, 255, 0 });
        extractLabel->setPositionX(180);
    } else {
        m_downloadBtn->setVisible(true);
    }

    m_closeBtn->setVisible(false);

    return true;
}

HighTexturesPopup* HighTexturesPopup::create(bool zipExists) {
    auto ret = new HighTexturesPopup();
    if (ret && ret->init(zipExists)) {
        ret->m_zipExists = zipExists;
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void HighTexturesPopup::onDownload(CCObject* sender) {
    startDownload();
}

void HighTexturesPopup::onExtract(CCObject* sender) {
    fs::path path = Mod::get()->getConfigDir();
    startExtract(path / (m_gameVersion + ".zip"), path);
}

void HighTexturesPopup::onRetry(CCObject* sender) {
    fs::path path = Mod::get()->getConfigDir();
    m_zipExists = fs::exists(path / (m_gameVersion + ".zip"));

    if (m_zipExists) startExtract(path / (m_gameVersion + ".zip"), path);
    else startDownload();
}

void HighTexturesPopup::onRestart(CCObject* sender) {
    Mod::get()->setSavedValue("applied", false);
    game::restart(true);
}

void HighTexturesPopup::onHide(CCObject* sender) {
    m_isHidden = true;
    this->setVisible(false);
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
}

void HighTexturesPopup::setDownloadPercentage(float percentage, ccColor3B color) {
    m_downloadPercentage = percentage;
    m_downloadStencil->setScaleX(m_downloadPercentage / 100);
    m_downloadLabel->setString(fmt::format("{:.2f}%", m_downloadPercentage).c_str());
    m_downloadLabel->setColor(color);
}

void HighTexturesPopup::setExtractPercentage(float percentage, ccColor3B color) {
    m_extractPercentage = percentage;
    m_extractStencil->setScaleX(m_extractPercentage / 100);
    m_extractLabel->setString(fmt::format("{:.2f}%", m_extractPercentage).c_str());
    m_extractLabel->setColor(color);
}

void HighTexturesPopup::setChecksumPercentage(float percentage, ccColor3B color) {
    m_checksumPercentage = percentage;
    m_checksumStencil->setScaleX(m_checksumPercentage / 100);
    m_checksumLabel->setString(fmt::format("{:.2f}%", m_checksumPercentage).c_str());
    m_checksumLabel->setColor(color);
}

void HighTexturesPopup::handleDownloadFinish(web::WebResponse res, std::shared_ptr<int> num, fs::path path) {
    if (res.ok()) {
        fs::path file = path / (m_gameVersion + ".zip");
        if (res.into(file).isOk()) {
            log::debug("Downloaded high graphics textures to {}", file.string());
            downloadSucceeded(file, path);
        } else {
            log::debug("Failed to transfer data to zip file at {}", file.string());
            downloadFailed("Failed to transfer data to zip file.");
        }
    } else {
        (*num)++;
        // auto n = *num;
        auto n = 1;
        if ((*num) < m_links[m_gameVersion].size()) {
            log::debug("Failed to download from primary link: {}. Trying backup link: {}", m_links[m_gameVersion][n - 1], m_links[m_gameVersion][n]);
            m_chatLabel->setString(fmt::format("Failed to download from primary link. Trying backup link {}...\n({}) ", n, m_links[m_gameVersion][n]).c_str());
            setDownloadPercentage(0.f, { 255, 255, 255 });

            web::WebRequest backupReq = web::WebRequest();
            backupReq.timeout(std::chrono::seconds(900));
            m_downloadListener.spawn(
                backupReq.get(m_links[m_gameVersion][n]),
                [this, num, path] (web::WebResponse res) {
                    handleDownloadFinish(res, num, path);
                }
            );

        } else {
            log::debug("Failed to download from all links.");
            downloadFailed("Failed to download file. Did you time out? (15 minutes)");
        }
    }
}

void HighTexturesPopup::startDownload() {
    auto num = std::make_shared<int>(0);

    m_closeBtn->setVisible(false);
    m_progressBG->setVisible(true);

    m_downloadLabel->setOpacity(0);
    m_downloadLabel->setVisible(true);
    m_downloadLabel->runAction(CCFadeIn::create(0.5f));

    m_chatLabel->setString(fmt::format("Downloading high graphics textures...\n({}) ", m_links[m_gameVersion][0]).c_str());

    m_downloadBtn->setVisible(false);
    m_extractBtn->setVisible(false);
    m_retryBtn->setVisible(false);
    m_restartBtn->setVisible(false);
    m_hideBtn->setVisible(true);

    setDownloadPercentage(0.f, { 255, 255, 255 });

    fs::path path = Mod::get()->getConfigDir();

    web::WebRequest req = web::WebRequest();
    req.timeout(std::chrono::seconds(900));

    req.onProgress([this] (web::WebProgress p) {
        setDownloadPercentage(p.downloadProgress().value_or(0.f), { 255, 255, 255 });
    });

    log::debug("Starting downloading from link: {}", m_links[m_gameVersion][0]);
    m_downloadListener.spawn(
        req.get(m_links[m_gameVersion][0]),
        [this, num, path] (web::WebResponse res) {
        handleDownloadFinish(res, num, path);
        }
    );
}

arc::Future<Result<void, std::string>> HighTexturesPopup::getExtractTask(fs::path file, fs::path path) {
    co_return file::Unzip::intoDir(
        [this] (auto num, auto total) {
            geode::queueInMainThread([this, num, total] {
                this->setExtractPercentage(num / (float)total * 100, { 255, 255, 255 });
            });
        },
        file, path, true
    );
}

void HighTexturesPopup::startExtract(fs::path file, fs::path path) {
    m_closeBtn->setVisible(false);
    m_progressBG->setVisible(true);

    m_downloadLabel->runAction(CCMoveBy::create(0.5f, { -50, 0 }));

    m_extractLabel->setOpacity(0);
    m_extractLabel->setVisible(true);
    m_extractLabel->runAction(CCFadeIn::create(0.5f));
    
    m_chatLabel->setString("Extracting high graphics textures. This might take a while...");

    m_downloadBtn->setVisible(false);
    m_extractBtn->setVisible(false);
    m_retryBtn->setVisible(false);
    m_restartBtn->setVisible(false);
    m_hideBtn->setVisible(true);

    setExtractPercentage(0.f, { 255, 255, 255 });

    async::spawn(
        this->getExtractTask(file, path),
        [this, path] (Result<void, std::string> res) {
            fs::path extractedPath = path / m_gameVersion;
            if (fs::exists(extractedPath)) {
                log::debug("Extracted high graphics textures to {}", extractedPath.string());
                extractSucceeded(extractedPath);
            } else {
                log::debug("Failed to extract high graphics textures to {}", extractedPath.string());
                extractFailed("Cannot find folder");
            }
        }
    );
}

arc::Future<std::string> HighTexturesPopup::getChecksumTask(fs::path path) {
    co_return HighGraphics::get()->getChecksum(
        [this] (auto num, auto total, auto isCheck) {
            geode::queueInMainThread([this, num, total, isCheck] {
                this->setChecksumPercentage(isCheck ? 10 + num / (float)total * 90 : num / (float)total * 10, { 255, 255, 255 });
            });
        }, path
    );
}

void HighTexturesPopup::startChecksum(fs::path path) {
    m_closeBtn->setVisible(false);
    m_progressBG->setVisible(true);

    m_downloadLabel->runAction(CCMoveBy::create(0.5f, { -40, 0 }));
    m_extractLabel->runAction(CCMoveBy::create(0.5f, { -50, 0 }));

    m_checksumLabel->setOpacity(0);
    m_checksumLabel->setVisible(true);
    m_checksumLabel->runAction(CCFadeIn::create(0.5f));

    m_chatLabel->setString("Verifying extracted files' integrity...");

    m_downloadBtn->setVisible(false);
    m_extractBtn->setVisible(false);
    m_retryBtn->setVisible(false);
    m_restartBtn->setVisible(false);
    m_hideBtn->setVisible(true);

    setChecksumPercentage(0.f, { 255, 255, 255 });

    async::spawn(
        this->getChecksumTask(path),
        [this] (std::string res) {
            std::string expectedChecksum = HighGraphics::get()->m_checksums[m_gameVersion];
            if (res == expectedChecksum) {
                log::debug("Checksum verified: {}", res);
                checksumSucceeded();
            } else {
                log::debug("Checksum mismatch: expected {}, got {}", expectedChecksum, res);
                checksumFailed("Checksum mismatch");
            }
        }
    );
}

void HighTexturesPopup::downloadSucceeded(fs::path file, fs::path path) {
    m_downloadPercentage = 100.f;
    setDownloadPercentage(m_downloadPercentage, { 0, 255, 0 });

    startExtract(file, path);
}

void HighTexturesPopup::downloadFailed(std::string reason) {
    notifyFailure("Download", reason);
    
    m_closeBtn->setVisible(true);
    m_chatLabel->setString(fmt::format("Error downloading: {}", reason).c_str());

    m_downloadBtn->setVisible(false);
    m_extractBtn->setVisible(false);
    m_restartBtn->setVisible(false);
    m_hideBtn->setVisible(false);

    m_retryBtn->setVisible(true);

    setDownloadPercentage(m_downloadPercentage, { 255, 0, 0 });
}

void HighTexturesPopup::extractSucceeded(fs::path path) {
    // m_extractPercentage = 100.f;
    // setExtractPercentage(m_extractPercentage, { 0, 255, 0 });

    // startChecksum(path);
    notifySuccess();

    m_finished = true;
    HighGraphics::get()->m_success = true;

    m_chatLabel->setString("High graphics textures have been installed successfully! Please restart the game to finish this process.");

    m_downloadBtn->setVisible(false);
    m_extractBtn->setVisible(false);
    m_retryBtn->setVisible(false);
    m_hideBtn->setVisible(false);
    m_restartBtn->setVisible(true);

    m_extractPercentage = 100.f;
    setExtractPercentage(m_extractPercentage, { 0, 255, 0 });
}

void HighTexturesPopup::extractFailed(std::string reason) {
    notifyFailure("Extraction", reason);

    m_closeBtn->setVisible(true);
    m_chatLabel->setString(fmt::format("Error extracting: {}", reason).c_str());

    m_downloadBtn->setVisible(false);
    m_extractBtn->setVisible(false);
    m_restartBtn->setVisible(false);
    m_hideBtn->setVisible(false);

    m_retryBtn->setVisible(true);

    setExtractPercentage(m_extractPercentage, { 255, 0, 0 });
}

void HighTexturesPopup::checksumSucceeded() {
    notifySuccess();

    m_finished = true;
    HighGraphics::get()->m_success = true;

    m_chatLabel->setString("High graphics textures have been installed successfully! Please restart the game to finish this process.");

    m_downloadBtn->setVisible(false);
    m_extractBtn->setVisible(false);
    m_retryBtn->setVisible(false);
    m_hideBtn->setVisible(false);

    m_restartBtn->setVisible(true);

    m_checksumPercentage = 100.f;
    setChecksumPercentage(m_checksumPercentage, { 0, 255, 0 });
}

void HighTexturesPopup::checksumFailed(std::string reason) {
    notifyFailure("Checksum", reason);

    m_closeBtn->setVisible(true);
    m_chatLabel->setString(fmt::format("Error verifying files: {}", reason).c_str());

    m_downloadBtn->setVisible(false);
    m_extractBtn->setVisible(false);
    m_restartBtn->setVisible(false);
    m_hideBtn->setVisible(false);

    m_retryBtn->setVisible(true);

    setChecksumPercentage(m_checksumPercentage, { 255, 0, 0 });
}

void HighTexturesPopup::notifySuccess() {
    auto notif = Notification::create("High graphics textures have been installed successfully!", NotificationIcon::Success, 5.f);
    notif->show();
}

void HighTexturesPopup::notifyFailure(std::string which, std::string reason) {
    auto notif = Notification::create(fmt::format("{} error: {}", which, reason).c_str(), NotificationIcon::Error, 5.f);
    notif->show();
}