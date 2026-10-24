#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
using namespace geode::prelude;
namespace {
    constexpr char const* HUD_ID = "gd-nova-hud";
    CCLabelBMFont* createHud() {
        auto label = CCLabelBMFont::create("GD NOVA", "bigFont.fnt");
        label->setID(HUD_ID);
        label->setAnchorPoint({0.f, 1.f});
        label->setColor({124, 192, 255});
        label->setOpacity(210);
        return label;
    }
}
class $modify(NovaMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        auto win = CCDirector::sharedDirector()->getWinSize();
        auto menu = CCMenu::create();
        menu->setID("gd-nova-menu");
        menu->setPosition({win.width - 57.f, win.height - 29.f});
        auto title = CCLabelBMFont::create("NOVA", "goldFont.fnt");
        title->setScale(0.6f);
        auto button = CCMenuItemSpriteExtra::create(title, this, menu_selector(NovaMenuLayer::onNova));
        button->setID("gd-nova-open");
        menu->addChild(button);
        addChild(menu, 100);
        return true;
    }
    void onNova(CCObject*) {
        FLAlertLayer::create(
            "<cy>GD Nova</c> <cg>v0.1</c>",
            "<cb>Midnight utilities</c>\n\nConfigure the HUD in Geode settings.",
            "OK"
        )->show();
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
        schedule(schedule_selector(NovaPlayLayer::tickNova), 0.15f);
        return true;
    }
    void tickNova(float) {
        auto hud = m_fields->hud;
        if (!hud) return;
        auto mod = Mod::get();
        hud->setVisible(mod->getSettingValue<bool>("show-hud"));
        if (!hud->isVisible()) return;
        auto msg = std::string("GD NOVA");
        if (mod->getSettingValue<bool>("show-attempts"))
            msg += fmt::format(" | Attempt {}", m_attempts);
        if (mod->getSettingValue<bool>("show-progress"))
            msg += fmt::format(" | {:.1f}%", getCurrentPercent());
        hud->setString(msg.c_str());
        hud->setScale(static_cast<float>(mod->getSettingValue<double>("hud-scale")));
    }
};
