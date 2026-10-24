#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

namespace {
constexpr int NOVA_PANEL_TAG = 871220;
constexpr char const* HUD_ID = "gd-nova-hud";

CCLabelBMFont* label(char const* text, float scale, ccColor3B color = {225, 238, 255}) {
    auto* node = CCLabelBMFont::create(text, "bigFont.fnt");
    node->setScale(scale);
    node->setColor(color);
    return node;
}

CCLabelBMFont* createHud() {
    auto* hud = label("GD NOVA", .5f, {124, 192, 255});
    hud->setID(HUD_ID);
    hud->setAnchorPoint({0.f, 1.f});
    hud->setOpacity(220);
    return hud;
}
}

class $modify(NovaMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        auto win = CCDirector::sharedDirector()->getWinSize();
        auto* menu = CCMenu::create();
        menu->setID("gd-nova-menu");
        menu->setPosition({win.width - 57.f, win.height - 29.f});
        auto* title = label("NOVA", .6f, {104, 184, 255});
        auto* button = CCMenuItemSpriteExtra::create(title, this, menu_selector(NovaMenuLayer::onNova));
        button->setID("gd-nova-open");
        menu->addChild(button);
        addChild(menu, 100);
        return true;
    }

    void addOption(CCMenu* menu, char const* name, char const* key, float y,
                   SEL_MenuHandler callback) {
        bool enabled = Mod::get()->getSettingValue<bool>(key);
        auto text = fmt::format("{}   [{}]", name, enabled ? "ON" : "OFF");
        auto* item = CCMenuItemSpriteExtra::create(
            label(text.c_str(), .45f, enabled ? ccColor3B{109, 226, 188} : ccColor3B{255, 147, 160}),
            this, callback
        );
        item->setPosition({0.f, y});
        menu->addChild(item);
    }

    void onNova(CCObject*) {
        if (getChildByTag(NOVA_PANEL_TAG)) return;
        auto win = CCDirector::sharedDirector()->getWinSize();
        auto* overlay = CCLayerColor::create({9, 13, 30, 235}, win.width, win.height);
        overlay->setTag(NOVA_PANEL_TAG);
        overlay->setZOrder(200);
        addChild(overlay, 200);
        auto center = CCPoint{win.width / 2.f, win.height / 2.f};

        auto* heading = label("GD NOVA", .85f, {116, 180, 255});
        heading->setPosition({center.x, center.y + 108.f});
        overlay->addChild(heading);
        auto* hint = label("MIDNIGHT  /  VISUAL SETTINGS", .32f, {135, 156, 203});
        hint->setPosition({center.x, center.y + 82.f});
        overlay->addChild(hint);

        auto* menu = CCMenu::create();
        menu->setPosition(center);
        overlay->addChild(menu);

        addOption(menu, "HUD", "show-hud", 47.f, menu_selector(NovaMenuLayer::toggleHud));
        addOption(menu, "ATTEMPTS", "show-attempts", 16.f, menu_selector(NovaMenuLayer::toggleAttempts));
        addOption(menu, "PROGRESS", "show-progress", -15.f, menu_selector(NovaMenuLayer::toggleProgress));

        auto* scaleText = label(fmt::format("HUD SCALE: {:.2f}", Mod::get()->getSettingValue<double>("hud-scale")).c_str(), .4f, {215, 224, 248});
        scaleText->setPosition({0.f, -52.f});
        menu->addChild(scaleText);

        auto* minus = CCMenuItemSpriteExtra::create(label("-", .6f), this, menu_selector(NovaMenuLayer::scaleDown));
        minus->setPosition({-105.f, -52.f});
        menu->addChild(minus);
        auto* plus = CCMenuItemSpriteExtra::create(label("+", .6f), this, menu_selector(NovaMenuLayer::scaleUp));
        plus->setPosition({105.f, -52.f});
        menu->addChild(plus);

        auto* close = CCMenuItemSpriteExtra::create(label("CLOSE", .48f, {255, 169, 169}), this, menu_selector(NovaMenuLayer::onCloseNova));
        close->setPosition({0.f, -88.f});
        menu->addChild(close);
    }

    void redraw() {
        if (auto* panel = getChildByTag(NOVA_PANEL_TAG)) panel->removeFromParentAndCleanup(true);
        onNova(nullptr);
    }
    void flip(char const* key) {
        auto* mod = Mod::get();
        mod->setSettingValue<bool>(key, !mod->getSettingValue<bool>(key));
        redraw();
    }
    void toggleHud(CCObject*) { flip("show-hud"); }
    void toggleAttempts(CCObject*) { flip("show-attempts"); }
    void toggleProgress(CCObject*) { flip("show-progress"); }
    void scaleBy(double change) {
        auto* mod = Mod::get();
        auto value = mod->getSettingValue<double>("hud-scale");
        mod->setSettingValue<double>("hud-scale", std::max(.25, std::min(.90, value + change)));
        redraw();
    }
    void scaleDown(CCObject*) { scaleBy(-.05); }
    void scaleUp(CCObject*) { scaleBy(.05); }
    void onCloseNova(CCObject*) {
        if (auto* panel = getChildByTag(NOVA_PANEL_TAG)) panel->removeFromParentAndCleanup(true);
    }
};

class $modify(NovaPlayLayer, PlayLayer) {
    struct Fields { CCLabelBMFont* hud = nullptr; };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        auto win = CCDirector::sharedDirector()->getWinSize();
        m_fields->hud = createHud();
        m_fields->hud->setPosition({8.f, win.height - 7.f});
        addChild(m_fields->hud, 1000);
        schedule(schedule_selector(NovaPlayLayer::tickNova), .15f);
        return true;
    }

    void tickNova(float) {
        auto* hud = m_fields->hud;
        if (!hud) return;
        auto* mod = Mod::get();
        hud->setVisible(mod->getSettingValue<bool>("show-hud"));
        if (!hud->isVisible()) return;
        auto message = std::string("GD NOVA");
        if (mod->getSettingValue<bool>("show-attempts"))
            message += fmt::format(" | Attempt {}", m_attempts);
        if (mod->getSettingValue<bool>("show-progress"))
            message += fmt::format(" | {:.1f}%", getCurrentPercent());
        hud->setString(message.c_str());
        hud->setScale(static_cast<float>(mod->getSettingValue<double>("hud-scale")));
    }
};
