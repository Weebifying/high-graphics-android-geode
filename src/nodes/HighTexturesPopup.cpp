#include "HighTexturesPopup.hpp"
namespace fs = std::filesystem;

bool HighTexturesPopup::setup(std::string const& value) {
    this->setTitle(value);

    CCSize size = m_mainLayer->getContentSize();

    auto chatLabel = CCLabelBMFont::create("Looks like you have yet to download the high graphics textures necessary for this mod. Please download it now for the best experience. (File size: 110MB) ", "chatFont.fnt");
    chatLabel->setPosition({ size.width/2, size.height - 70 });
    chatLabel->setScale(0.9f);
    chatLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    // chatLabel->setLineBreakWithoutSpace(true);
    chatLabel->setWidth(size.width - 10.f);
    chatLabel->setID("chat-label");
    m_mainLayer->addChild(chatLabel);
    m_chatLabel = chatLabel;

    auto menu = CCMenu::create();
    menu->setPosition({ 0, 0 });
    menu->setContentSize(size);
    menu->setID("download-menu");
    m_mainLayer->addChild(menu);

    auto downloadSpr = ButtonSprite::create("Download", 104, true, "bigFont.fnt", "GJ_button_01.png", 30, 0.65f);
    auto downloadBtn = CCMenuItemSpriteExtra::create(
        downloadSpr,
        this,
        menu_selector(HighTexturesPopup::onDownload)
    );
    downloadBtn->setPosition({ size.width/2, 25 });
    downloadBtn->setContentSize({ 104, 30 });
    downloadSpr->setPosition(downloadBtn->getContentSize()/2);
    downloadBtn->setID("download-button");
    menu->addChild(downloadBtn);
    m_downloadBtn = downloadBtn;

    auto retrySpr = ButtonSprite::create("Retry", 104, true, "bigFont.fnt", "GJ_button_01.png", 30, 0.65f);
    auto retryBtn = CCMenuItemSpriteExtra::create(
        retrySpr,
        this,
        menu_selector(HighTexturesPopup::onRetry)
    );
    retryBtn->setVisible(false);
    retryBtn->setPosition({ size.width/2, 25 });
    retryBtn->setContentSize({ 104, 30 });
    retrySpr->setPosition(retryBtn->getContentSize()/2);
    retryBtn->setID("retry-button");
    menu->addChild(retryBtn);
    m_retryBtn = retryBtn;

    auto applySpr = ButtonSprite::create("Apply Textures", 180, true, "bigFont.fnt", "GJ_button_01.png", 30, 0.65f);
    auto applyBtn = CCMenuItemSpriteExtra::create(
        applySpr,
        this,
        menu_selector(HighTexturesPopup::onApplyTextures)
    );
    applyBtn->setVisible(false);
    applyBtn->setPosition({ size.width/2, 25 });
    applyBtn->setContentSize({ 144, 30 });
    applySpr->setPosition(applyBtn->getContentSize()/2);
    applyBtn->setID("apply-button");
    menu->addChild(applyBtn);
    m_applyBtn = applyBtn;

    auto progressBG = CCSprite::create("GJ_progressBar_001.png");
    progressBG->setVisible(false);
    progressBG->setPosition({ size.width/2, 60 });
    progressBG->setScale(0.75f);
    progressBG->setColor({ 0, 0, 0 });
    progressBG->setOpacity(125);
    progressBG->setID("progress-bar-background");
    m_mainLayer->addChild(progressBG);
    m_progressBG = progressBG;

    auto progressBar = CCSprite::create("GJ_progressBar_001.png");
    progressBar->setScale(0.992f);
    progressBar->setScaleY(0.86f);
    progressBar->setPosition(progressBG->getContentSize() / 2);
    progressBar->setColor({ 0, 255, 0 });

    progressBar->setID("progress-bar");

    auto stencil = CCScale9Sprite::create("square02_001.png");
	stencil->setAnchorPoint({ 0, 0.5f });
	stencil->setContentWidth(progressBar->getContentWidth());
    stencil->setContentHeight(100);
    stencil->setScaleX(m_percentage / 100);
    m_progressBarStencil = stencil;

    auto clippingNode = CCClippingNode::create();
    clippingNode->setStencil(stencil);
    clippingNode->setAnchorPoint({ 0, 0 });
	clippingNode->setPosition({ 0, 0 });
	clippingNode->setContentSize(progressBG->getContentSize());
    clippingNode->setID("progress-bar-clipping-node");

	clippingNode->addChild(progressBar);
    progressBG->addChild(clippingNode);

    auto percentageLabel = CCLabelBMFont::create(fmt::format("{:.2f}%", m_percentage).c_str(), "bigFont.fnt");
    percentageLabel->setVisible(false);
    percentageLabel->setPosition({ size.width/2, progressBG->getPositionY() + 20});
    percentageLabel->setScale(0.5f);
    percentageLabel->setID("percentage-label");
    m_mainLayer->addChild(percentageLabel);
    m_percentageLabel = percentageLabel;

    return true;
}

HighTexturesPopup* HighTexturesPopup::create(std::string const& text) {
    auto ret = new HighTexturesPopup();
    if (ret && ret->initAnchored(300.f, 200.f, text)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void HighTexturesPopup::onDownload(CCObject* sender) {
    startDownload();
}

void HighTexturesPopup::onRetry(CCObject* sender) {
    startDownload();
}

void HighTexturesPopup::onApplyTextures(CCObject* sender) {
    std::string version = Mod::get()->getMetadata().getGameVersion().value();
    fs::path path = Mod::get()->getConfigDir();

    CCFileUtils::get()->addTexturePack({ "weebify.high-textures", { (path / version).string() } });
    reloadTextures();
}

void HighTexturesPopup::setPercentage(float percentage, ccColor3B color) {
    m_percentage = percentage;
    m_progressBarStencil->setScaleX(m_percentage / 100);
    m_percentageLabel->setString(fmt::format("{:.2f}%", m_percentage).c_str());
    m_percentageLabel->setColor(color);
}

void HighTexturesPopup::startDownload() {
    m_isDownload = true;
    m_closeBtn->setVisible(false);
    m_progressBG->setVisible(true);
    m_percentageLabel->setVisible(true);
    m_chatLabel->setString("Downloading high graphics textures...");

    m_downloadBtn->setVisible(false);
    m_retryBtn->setVisible(false);
    m_applyBtn->setVisible(false);

    setPercentage(0.f, { 255, 255, 255 });

    std::string version = Mod::get()->getMetadata().getGameVersion().value();
    fs::path path = Mod::get()->getConfigDir();

    m_listener.bind([=] (web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            fs::path file = path / (version + ".zip");
            m_chatLabel->setString("Extracting high graphics textures...");
            if (res->into(file)) {
                if (file::Unzip::intoDir(file, path, true)) {
                    downloadSucceeded();
                } else {
                    downloadFailed("Failed to extract zip file. Are you connected to the internet?");
                }
            } else {
                downloadFailed("Failed to transfer data to zip file. Did you time out? (300 seconds)");
            }
        } else if (web::WebProgress* p = e->getProgress()) {
            setPercentage(p->downloadProgress().value_or(0.f), { 255, 255, 255 });
        } else if (e->isCancelled()) {
            downloadFailed("Download failed.");
        }
    });

    web::WebRequest req = web::WebRequest();
    req.timeout(std::chrono::seconds(300));
    m_listener.setFilter(req.get(m_links[version].asString().unwrap()));
}

void HighTexturesPopup::downloadSucceeded() {
    m_isDownload = false;
    m_chatLabel->setString("High graphics textures have been downloaded successfully!");

    m_downloadBtn->setVisible(false);
    m_retryBtn->setVisible(false);

    m_applyBtn->setVisible(true);

    m_percentage = 100.f;
    setPercentage(m_percentage, { 0, 255, 0 });
}

void HighTexturesPopup::downloadFailed(std::string reason) {
    m_isDownload = false;
    m_closeBtn->setVisible(true);
    m_chatLabel->setString(fmt::format("Error: {}", reason).c_str());

    m_downloadBtn->setVisible(false);
    m_applyBtn->setVisible(false);

    m_retryBtn->setVisible(true);

    setPercentage(m_percentage, { 255, 0, 0 });
}