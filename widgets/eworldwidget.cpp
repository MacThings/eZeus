#include "eworldwidget.h"

#include "eworldmenu.h"
#include "eworldmapwidget.h"
#include "erequestdialog.h"
#include "egiftdialog.h"
#include "emainwindow.h"
#include "egamewidget.h"
#include "egiftsizedialog.h"
#include "evectorhelpers.h"
#include "eframedbutton.h"
#include "elanguage.h"
#include "ecitysettingswidget.h"
#include "efulfilldialog.h"
#include "emessagewidget.h"

#include "gameEvents/eplayerconquestevent.h"
#include "gameEvents/eplayerraidevent.h"
#include "gameEvents/erequestaidevent.h"
#include "gameEvents/erequeststrikeevent.h"

#include "eacceptbutton.h"
#include "ecancelbutton.h"
#include "estringhelpers.h"
#include "widgets/ecitybutton.h"

#include "buildings/eheroshall.h"

void eWorldWidget::initialize() {
    mWM = new eWorldMenu(window());
    const auto requestFunc = [this]() {
        openRequestDialog();
    };
    const auto fulfillFunc = [this]() {
        openFulfillDialog();
    };
    const auto giftFunc = [this]() {
        openGiftDialog();
    };
    const auto raidFunc = [this]() {
        const auto enlistAction = [this](const eEnlistedForces& forces,
                                         const eResourceType r) {
            mBoard->enlistForces(forces);
            const auto e = e::make_shared<ePlayerRaidEvent>(
                               eGameEventBranch::root, mBoard);
            const auto boardDate = mBoard->date();
            const int period = eNumbers::sArmyTravelTime;
            const auto date = boardDate + period;
            e->initializeDate(date, period, 1);
            e->initialize(forces, mCity, r);
            mBoard->addRootGameEvent(e);
            update();
        };
        std::vector<eResourceType> resources;
        resources.push_back(eResourceType::none);
        resources.push_back(eResourceType::drachmas);
        const auto& sells = mCity->sells();
        for(const auto& s : sells) {
            resources.push_back(s.fType);
        }
        openEnlistForcesDialog(enlistAction, {mCity}, resources);
    };
    const auto conquerFunc = [this]() {
        const auto enlistAction = [this](const eEnlistedForces& forces,
                                         const eResourceType r) {
            (void)r;
            mBoard->enlistForces(forces);
            const auto e = e::make_shared<ePlayerConquestEvent>(
                               eGameEventBranch::root, mBoard);
            const auto boardDate = mBoard->date();
            const int period = eNumbers::sArmyTravelTime;
            const auto date = boardDate + period;
            e->initializeDate(date, period, 1);
            e->initialize(forces, mCity);
            mBoard->addRootGameEvent(e);
            update();
        };
        openEnlistForcesDialog(enlistAction, {mCity});
    };
    mWM->initialize(requestFunc, fulfillFunc, giftFunc,
                    raidFunc, conquerFunc);

    mWMW = new eWorldMapWidget(window());
    addWidget(mWMW);

    mWMW->setSelectCityAction([this](const stdsptr<eWorldCity>& ct) {
        mCity = ct;
        const bool editor = mWorldBoard && mWorldBoard->editorMode();
        mSettingsButton->setVisible(mCity && editor);
        mWM->setCity(ct);
    });
    mWMW->setSetTextAction([this](const std::string& text) {
        mWM->setText(text);
    });

    addWidget(mWM);
    mWM->align(eAlignment::right | eAlignment::top);

    const int p = padding();

    mMapButton = new eFramedButton(window());
    mMapButton->setUnderline(false);
    mMapButton->setRenderBg(true);
    mMapButton->setText(eLanguage::text("map"));
    mMapButton->fitContent();
    addWidget(mMapButton);
    mMapButton->setPressAction([this]() {
        if(!mWorldBoard) return;
        auto m = mWorldBoard->map();
        if(m == eWorldMap::poseidon4) {
            m = eWorldMap::greece1;
        } else {
            m = static_cast<eWorldMap>(static_cast<int>(m) + 1);
        }
        mWorldBoard->setMap(m);
        setMap(m);
    });

    const int x = width() - mWM->width() - p - mMapButton->width();
    mMapButton->move(x, p);

    mAddCityButton = new eFramedButton(window());
    mAddCityButton->setUnderline(false);
    mAddCityButton->setRenderBg(true);
    mAddCityButton->setText(eLanguage::text("add_city"));
    mAddCityButton->fitContent();
    addWidget(mAddCityButton);
    mAddCityButton->setPressAction([this]() {
        if(!mWorldBoard) return;
        mWorldBoard->addCity(std::make_shared<eWorldCity>());
        mWMW->updateWidgets();
    });
    const int xx = width() - mWM->width() - p - mAddCityButton->width();
    mAddCityButton->move(xx, mMapButton->y() + mMapButton->height() + p);

    mSettingsButton = new eFramedButton(window());
    mSettingsButton->setUnderline(false);
    mSettingsButton->setRenderBg(true);
    mSettingsButton->setText(eLanguage::text("settings"));
    mSettingsButton->fitContent();
    mSettingsButton->hide();
    addWidget(mSettingsButton);
    mSettingsButton->setPressAction([this]() {
        if(!mCity) return;
        const auto d = new eCitySettingsWidget(window());
        d->initialize(mCity);

        window()->execDialog(d);
        d->align(eAlignment::center);
    });
    const int xxx = width() - mWM->width() - p - mSettingsButton->width();
    mSettingsButton->move(xxx, mAddCityButton->y() + mAddCityButton->height() + p);
}

void eWorldWidget::setBoard(eGameBoard* const board) {
    mBoard = board;
    mWMW->setBoard(board);
    setWorldBoard(mBoard->getWorldBoard());
}

void eWorldWidget::setWorldBoard(eWorldBoard* const board) {
    mWorldBoard = board;
    mWMW->setWorldBoard(board);
    mWM->setWorldBoard(board);
    if(board) setMap(board->map());
    const bool editor = board && board->editorMode();
    mMapButton->setVisible(editor);
    mAddCityButton->setVisible(editor);
    mSettingsButton->setVisible(mCity && editor);
    update();
}

void eWorldWidget::update() {
    mWMW->updateWidgets();
}

void eWorldWidget::openEnlistForcesDialog(
        const eEnlistAction& a,
        const std::vector<stdsptr<eWorldCity>>& exclude,
        const std::vector<eResourceType>& plunderResources) {
    mBoard->requestForces(a, plunderResources, exclude);
}

void eWorldWidget::openRequestDialog() {
    const auto d = new eRequestDialog(window());
    const auto func = [this, d](const eResourceType type) {
        mBoard->request(mCity, type);
        const auto& cts = mWorldBoard->cities();
        for(const auto& ct : cts) {
            if(ct->isCurrentCity()) continue;
            ct->incAttitude(-10);
        }
        mCity->incAttitude(-10);
        mWM->updateRelationshipLabel();
        d->deleteLater();
    };
    const auto requestAid = [this, d]() {
        const auto cids = mBoard->personPlayerCities();
        for(const auto cid : cids) {
            const auto has = mBoard->militaryAid(cid, mCity);
            if(has) return;
        }
        const auto& cts = mWorldBoard->cities();
        for(const auto& ct : cts) {
            if(ct->isCurrentCity()) continue;
            ct->incAttitude(-10);
        }
        mCity->incAttitude(-10);
        mWM->updateRelationshipLabel();

        mBoard->requestAid(mCity);

        d->deleteLater();
    };
    const auto requestStrike = [this, d]() {
        const auto& cts = mWorldBoard->cities();
        std::vector<stdsptr<eWorldCity>> rivals;
        for(const auto& c : cts) {
            if(!c->isRival()) continue;
            rivals.push_back(c);
        }
        if(rivals.empty()) {
            const auto title = eLanguage::zeusText(5, 72); // Nobody to strike
            const auto text = eLanguage::zeusText(5, 73); // There are no cities to strike

            const auto qw = new eMessageWidget(window());
            qw->initialize(title, text);
            openDialog(qw);
            return;
        } else {
            const auto w = new eFramedWidget(window());
            w->setType(eFrameType::message);
            const int p = padding();
            const int width = 40*p;
            const auto cw = new eWidget(window());
            cw->setNoPadding();
            cw->setWidth(width);

            const auto titleLabel = new eLabel(window());
            titleLabel->setHugeFontSize();
            titleLabel->setText(eLanguage::zeusText(5, 74)); // Military strike
            titleLabel->fitContent();
            cw->addWidget(titleLabel);

            auto textBase = eLanguage::zeusText(5, 75);
            eStringHelpers::replace(textBase, "[city_nameA]", mCity->name());
            const auto rival = rivals[0];
            auto text = textBase;
            eStringHelpers::replace(text, "[city_nameB]", rival->name());

            const auto textLabel = new eLabel(window());
            textLabel->setSmallFontSize();
            textLabel->setWrapWidth(width);
            textLabel->setText(text);
            textLabel->fitContent();
            textLabel->setWidth(1.25*textLabel->width());
            cw->addWidget(textLabel);
            const int tly = titleLabel->y() + titleLabel->height();
            textLabel->setY(tly);

            const auto cityButton = new eCityButton(window());
            cityButton->setValidator([](const stdsptr<eWorldCity>& c) {
                return c->isRival();
            });
            cityButton->initialize(mWorldBoard,
                                   [textLabel, textBase](
                                   const stdsptr<eWorldCity>& c) {
                auto text = textBase;
                eStringHelpers::replace(text, "[city_nameB]", c->name());
                textLabel->setText(text);
                textLabel->fitContent();
            });
            cityButton->setCity(rival);
            cw->addWidget(cityButton);
            const int tly2 = textLabel->y() + textLabel->height() + p;
            cityButton->setY(tly2);

            const auto buttons = new eWidget(window());

            const auto accept = new eAcceptButton(window());
            const auto cancel = new eCancelButton(window());
            accept->setPressAction([this, w, rivals, cityButton]() {
                const auto e = e::make_shared<eRequestStrikeEvent>(
                                   eGameEventBranch::root);
                e->setGameBoard(mBoard);
                e->setWorldBoard(mWorldBoard);
                e->setCity(mCity);
                e->setRivalCity(cityButton->city());
                const auto date = mBoard->date() + 30;
                e->initializeDate(date);
                mBoard->addRootGameEvent(e);
                w->deleteLater();
            });
            cancel->setPressAction([w]() {
                w->deleteLater();
            });
            buttons->addWidget(cancel);
            buttons->addWidget(accept);

            buttons->setNoPadding();
            accept->setX(width/5);
            buttons->fitContent();

            cw->addWidget(buttons);
            const int by = cityButton->y() + cityButton->height() + p;
            buttons->setY(by);

            cw->fitContent();
            cw->setHeight(cw->height() + p);
            w->addWidget(cw);
            cw->move(p, p);
            w->resize(cw->width() + 2*p, cw->height() + 2*p);
            titleLabel->align(eAlignment::hcenter);
            cityButton->align(eAlignment::hcenter);
            buttons->align(eAlignment::hcenter);

            openDialog(w);
        }
        d->deleteLater();
    };
    d->initialize(mCity, func, requestAid, requestStrike);
    openDialog(d);
}

void eWorldWidget::openFulfillDialog() {
    const auto d = new eFulfillDialog(window());
    d->initialize(mBoard, mCity);
    openDialog(d);
}

void eWorldWidget::openGiftDialog() {
    const auto d = new eGiftDialog(window());
    const auto func = [this, d](const eResourceType type) {
        const auto dd = new eGiftSizeDialog(window());
        const auto func = [this, d, dd](const eResourceType type,
                                        const int count) {
            mBoard->giftTo(mCity, type, count);
            d->deleteLater();
            dd->deleteLater();
        };
        dd->initialize(type, mCity, func, *mBoard);
        openDialog(dd);
    };
    d->initialize(mCity, func, *mBoard);
    openDialog(d);
}

void eWorldWidget::setMap(const eWorldMap map) {
    mWMW->setMap(map);
    mWMW->align(eAlignment::center);
    mWMW->setX((width() - mWM->width() - mWMW->width())/2);
}

void eWorldWidget::openDialog(eWidget* const d) {
    addWidget(d);
    d->align(eAlignment::vcenter);
    d->setX(mWMW->x() + (mWMW->width() - d->width())/2);
    window()->execDialog(d);
}

void eWorldWidget::openEnlistForcesDialog(
        const eEnlistedForces& enlistable,
        const std::vector<bool>& heroesAbroad,
        const eEnlistAction& action,
        const std::vector<eResourceType>& plunderResources) {

//    {
//        f.fHeroes.push_back(eHeroType::achilles);
//        f.fHeroes.push_back(eHeroType::hercules);
//        f.fHeroes.push_back(eHeroType::bellerophon);

//        for(int i = 0; i < 20; i ++) {
//            const auto b = e::make_shared<eSoldierBanner>(eBannerType::hoplite, *mBoard);
//            b->incCount();
//            const auto n = new stdsptr<eSoldierBanner>(b);
//            f.fSoldiers.push_back(b);
//        }

//        for(int i = 0; i < 4; i ++) {
//            const auto b = e::make_shared<eSoldierBanner>(eBannerType::horseman, *mBoard);
//            b->incCount();
//            const auto n = new stdsptr<eSoldierBanner>(b);
//            f.fSoldiers.push_back(b);
//        }


//        for(int i = 0; i < 2; i ++) {
//            const auto b = e::make_shared<eSoldierBanner>(eBannerType::amazon, *mBoard);
//            b->incCount();
//            const auto n = new stdsptr<eSoldierBanner>(b);
//            f.fSoldiers.push_back(b);
//        }
//    }

    const auto d = new eEnlistForcesDialog(window());
    d->initialize(enlistable, heroesAbroad, action, plunderResources);
    openDialog(d);
}

bool eWorldWidget::keyPressEvent(const eKeyPressEvent& e) {
    const bool edit = mWorldBoard->editorMode();
    if(edit) return true;
    const auto k = e.key();
    if(k == SDL_SCANCODE_ESCAPE) {
        window()->showGame(static_cast<eGameBoard*>(nullptr),
                           eGameWidgetSettings());
    }
    return true;
}
