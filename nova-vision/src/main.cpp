#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/binding/GameObject.hpp>
#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>
using namespace geode::prelude;

namespace {
// Collision geometry is collected from objects inserted by the engine.
// Initial version recognizes a conservative set of vanilla spike and solid-block IDs.
// Unknown custom hazards are intentionally NOT treated as safe.
struct Candidate {
    GameObject* object = nullptr;
    bool hazard = false;
};
std::vector<Candidate> seenObjects;

bool isSpike(int id) {
    switch (id) {
        case 8: case 39: case 103: case 392: case 458: case 459:
        case 144: case 145: case 177: case 178: case 179:
            return true;
        default: return false;
    }
}
bool isBlock(int id) {
    switch (id) {
        case 1: case 2: case 3: case 4: case 5: case 6: case 7:
        case 9: case 10: case 11: case 12: case 13: case 14:
            return true;
        default: return false;
    }
}
bool isKnownObstacle(int id) { return isSpike(id) || isBlock(id); }
}

class $modify(VisionPlayLayer, PlayLayer) {
    struct Fields {
        bool pressed = false;
        float lastX = 0.f;
        float velocityX = 0.f;
        float cooldown = 0.f;
        CCLabelBMFont* hud = nullptr;
    };

    bool init(GJGameLevel* level, bool replay, bool dontCreateObjects) {
        seenObjects.clear();
        if (!PlayLayer::init(level, replay, dontCreateObjects)) return false;
        if (m_player1) m_fields->lastX = m_player1->getPositionX();
        auto* hud = CCLabelBMFont::create("VISION: IDLE", "bigFont.fnt");
        hud->setScale(.34f);
        hud->setAnchorPoint({0.f, 1.f});
        hud->setPosition({8.f, CCDirector::sharedDirector()->getWinSize().height - 8.f});
        hud->setColor({130, 213, 255});
        addChild(hud, 800);
        m_fields->hud = hud;
        return true;
    }

    void addObject(GameObject* obj) {
        PlayLayer::addObject(obj);
        if (!obj) return;
        auto id = obj->m_objectID;
        if (isKnownObstacle(id)) seenObjects.push_back({obj, isSpike(id)});
    }

    void setBotInput(bool pressed) {
        if (pressed == m_fields->pressed) return;
        handleButton(pressed, 1, true);
        m_fields->pressed = pressed;
    }

    void resetLevel() {
        setBotInput(false);
        m_fields->cooldown = 0.f;
        m_fields->velocityX = 0.f;
        PlayLayer::resetLevel();
        if (m_player1) m_fields->lastX = m_player1->getPositionX();
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);
        auto& f = m_fields;
        auto* mod = Mod::get();
        bool enabled = mod->getSettingValue<bool>("bot-enabled");
        if (!enabled || !m_player1 || dt <= 0.f || dt > .1f) {
            setBotInput(false);
            if (f.hud) f.hud->setVisible(false);
            return;
        }

        auto* player = m_player1;
        auto pos = player->getPosition();
        f.velocityX = std::max(0.f, (pos.x - f.lastX) / dt);
        f.lastX = pos.x;
        f.cooldown = std::max(0.f, f.cooldown - dt);

        // Prototype: only make decisions for standard cube movement.
        // Never take over controls of other modes.
        if (player->isInBasicMode() && !player->m_isRobot && !player->m_isSpider) {
            auto playerBox = player->getObjectRect();
            float nearest = std::numeric_limits<float>::infinity();
            int scanned = 0;
            bool spike = false;
            const float scanRange = static_cast<float>(mod->getSettingValue<double>("scan-range"));
            for (auto const& entry : seenObjects) {
                if (!entry.object) continue;
                auto rect = entry.object->getObjectRect();
                float distance = rect.getMinX() - playerBox.getMaxX();
                if (distance < -4.f || distance > scanRange) continue;
                // Only objects overlapping the player's vertical route.
                if (rect.getMaxY() < playerBox.getMinY() - 8.f ||
                    rect.getMinY() > playerBox.getMaxY() + 24.f) continue;
                ++scanned;
                if (distance < nearest) { nearest = distance; spike = entry.hazard; }
            }
            // Trigger earlier at higher forward speeds. Cube jump needs a
            // calibrated approach; this simple heuristic is not a path planner.
            float trigger = std::clamp(f.velocityX * .18f, 18.f, 62.f);
            bool jumpNow = std::isfinite(nearest) && nearest < trigger &&
                nearest > -2.f && f.cooldown <= 0.f && player->m_isOnGround;
            if (jumpNow) { setBotInput(true); f.cooldown = .32f; }
            if (f.pressed && f.cooldown < .25f) setBotInput(false);
            if (f.hud) {
                auto text = fmt::format("VISION | nearby {} | nearest {} | {}",
                    scanned, std::isfinite(nearest) ? fmt::format("{:.0f}", nearest) : "--",
                    jumpNow ? "JUMP" : (spike ? "SPIKE" : "SCAN"));
                f.hud->setString(text.c_str());
            }
        } else {
            setBotInput(false);
            if (f.hud) f.hud->setString("VISION | unsupported game mode");
        }
        if (f.hud) f.hud->setVisible(mod->getSettingValue<bool>("show-debug"));
    }
};
