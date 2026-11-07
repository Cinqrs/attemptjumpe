#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class AttemptPopup : public Popup<> {
protected:
    TextInput* m_input = nullptr;

    bool setup() override {
        this->setTitle("Attempt Jumper");

        m_input = TextInput::create(150.f, "Attempt", "bigFont.fnt");
        m_input->setFilter("0123456789");
        m_input->setMaxCharCount(9);
        m_input->setPosition(m_mainLayer->getContentSize() / 2 + ccp(0, 10));
        m_mainLayer->addChild(m_input);

        auto btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Set"), this,
            menu_selector(AttemptPopup::onSet)
        );
        m_buttonMenu->addChildAtPosition(btn, Anchor::Bottom, {0, 25});
        return true;
    }

    void onSet(CCObject*) {
        auto str = m_input->getString();
        if (str.empty()) return;

        int value = numFromString<int>(str).unwrapOr(1);
        if (value < 1) value = 1;

        if (auto pl = PlayLayer::get()) {
            // resetLevel yeni attempt başlatınca sayaç +1 olur, o yüzden value-1
            pl->m_level->m_attempts = value - 1;
            pl->m_attempts = value - 1;
            pl->resetLevel();
        }
        this->onClose(nullptr);
    }

public:
    static AttemptPopup* create() {
        auto ret = new AttemptPopup();
        if (ret && ret->initAnchored(240.f, 140.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

class $modify(AttemptPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto menu = CCMenu::create();
        auto btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Attempts", "bigFont.fnt", "GJ_button_01.png", .6f),
            this, menu_selector(AttemptPauseLayer::onAttemptJumper)
        );
        menu->addChild(btn);

        auto win = CCDirector::get()->getWinSize();
        menu->setPosition({win.width - 70.f, 30.f});
        this->addChild(menu);
    }

    void onAttemptJumper(CCObject*) {
        AttemptPopup::create()->show();
    }
};
