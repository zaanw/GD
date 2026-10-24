#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <cstdint>
#include <cmath>

using namespace geode::prelude;

class $modify(NovaFramePlayLayer, PlayLayer) {
    struct Fields {
        CCLabelBMFont* counter = nullptr;
        std::uint64_t frames = 0;
        double avgFrameSeconds = 0.0;
        int textRefreshCountdown = 0;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        auto* counter = CCLabelBMFont::create("FRAME 0", "bigFont.fnt");
        counter->setID("zaan.nova-frame-counter/display");
        counter->setAnchorPoint({0.f, 1.f});
        counter->setPosition({8.f, CCDirector::sharedDirector()->getWinSize().height - 8.f});
        counter->setScale(.42f);
        counter->setColor({123, 198, 255});
        counter->setOpacity(230);
        this->addChild(counter, 1001);
        m_fields->counter = counter;
        return true;
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        m_fields->frames = 0;
        m_fields->avgFrameSeconds = 0.0;
        m_fields->textRefreshCountdown = 0;
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        auto* mod = Mod::get();
        auto* counter = m_fields->counter;
        if (!counter) return;

        counter->setVisible(mod->getSettingValue<bool>("show-counter"));
        if (!counter->isVisible()) return;

        // Counts PlayLayer simulation update callbacks, not GPU-rendered frames.
        ++m_fields->frames;
        if (dt > .00001f && dt < 1.f) {
            auto& average = m_fields->avgFrameSeconds;
            if (average == 0.0) average = dt;
            else average = average * .92 + static_cast<double>(dt) * .08;
        }

        if (++m_fields->textRefreshCountdown >= 4) {
            m_fields->textRefreshCountdown = 0;
            auto text = fmt::format("FRAME {}", m_fields->frames);
            if (mod->getSettingValue<bool>("show-fps") && m_fields->avgFrameSeconds > 0.0) {
                text += fmt::format(" | {:.0f} FPS*", 1.0 / m_fields->avgFrameSeconds);
            }
            counter->setString(text.c_str());
        }
        counter->setScale(static_cast<float>(mod->getSettingValue<double>("counter-scale")));
    }
};
