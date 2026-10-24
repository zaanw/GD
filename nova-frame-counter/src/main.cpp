#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <cstdint>
#include <string>

using namespace geode::prelude;

// Nova Frame Counter v0.2
// Input markers and action counts are real observed events.
// Exact frame windows are NOT inferred from click timings.
// Future macro-based testing is needed to measure them accurately.

class $modify(NovaFramePlayLayer, PlayLayer) {
    struct Fields {
        CCLabelBMFont* hud = nullptr;
        cocos2d::CCNode* markerLayer = nullptr;
        std::uint64_t ticks = 0;
        int presses = 0;
        int releases = 0;
        bool p1Held = false;
        double averageDt = 0.0;
        int refreshCounter = 0;
    };

    bool init(GJGameLevel* level, bool replay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, replay, dontCreateObjects)) return false;
        auto win = CCDirector::sharedDirector()->getWinSize();
        auto* hud = CCLabelBMFont::create("NOVA | 0 CLICKS", "bigFont.fnt");
        hud->setID("zaan.nova-frame-counter/hud");
        hud->setAnchorPoint({0.f, 1.f});
        hud->setPosition({8.f, win.height - 8.f});
        hud->setScale(.42f);
        hud->setColor({135, 195, 255});
        addChild(hud, 1001);
        m_fields->hud = hud;

        auto* layer = CCNode::create();
        layer->setID("zaan.nova-frame-counter/markers");
        addChild(layer, 1000);
        m_fields->markerLayer = layer;
        return true;
    }

    void showInputMarker(bool pressed) {
        if (!Mod::get()->getSettingValue<bool>("show-markers") || !m_player1)
            return;
        auto win = CCDirector::sharedDirector()->getWinSize();
        // The position is intentionally a screen HUD position: no false
        // assumption that world and screen coordinates are identical.
        auto* marker = CCLabelBMFont::create(
            pressed ? "DOWN" : "UP", "bigFont.fnt"
        );
        marker->setColor(pressed ? ccColor3B{99, 227, 176}
                                 : ccColor3B{142, 200, 255});
        marker->setScale(.35f);
        marker->setPosition({win.width * .5f, win.height * .72f});
        m_fields->markerLayer->addChild(marker);
        marker->runAction(CCSequence::create(
            CCFadeOut::create(.5f),
            CCCallFunc::create(marker, callfunc_selector(CCNode::removeFromParent)),
            nullptr
        ));
    }

    void handleButton(bool down, int button, bool player1) {
        if (player1 && button == 1 && down != m_fields->p1Held) {
            m_fields->p1Held = down;
            if (down) ++m_fields->presses;
            else ++m_fields->releases;
            showInputMarker(down);
        }
        PlayLayer::handleButton(down, button, player1);
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        m_fields->ticks = 0;
        m_fields->presses = 0;
        m_fields->releases = 0;
        m_fields->p1Held = false;
        m_fields->averageDt = 0.0;
        m_fields->refreshCounter = 0;
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);
        ++m_fields->ticks;
        if (dt > .00001f && dt < .5f) {
            auto& average = m_fields->averageDt;
            average = average == 0.0 ? dt : average * .92 + dt * .08;
        }

        auto* hud = m_fields->hud;
        if (!hud) return;
        auto* mod = Mod::get();
        hud->setVisible(mod->getSettingValue<bool>("show-counter"));
        if (!hud->isVisible()) return;
        hud->setScale(static_cast<float>(mod->getSettingValue<double>("counter-scale")));
        if (++m_fields->refreshCounter < 4) return;
        m_fields->refreshCounter = 0;

        auto text = fmt::format("NOVA | CLICKS {} | RELEASES {}",
            m_fields->presses, m_fields->releases);
        if (mod->getSettingValue<bool>("show-ticks")) {
            text += fmt::format(" | TICKS {}", m_fields->ticks);
        }
        if (mod->getSettingValue<bool>("show-fps") && m_fields->averageDt > .0) {
            text += fmt::format(" | ~{:.0f} UPS", 1.0 / m_fields->averageDt);
        }
        hud->setString(text.c_str());
    }
};
