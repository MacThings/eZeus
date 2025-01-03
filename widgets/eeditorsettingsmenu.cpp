#include "eeditorsettingsmenu.h"

#include "eframedbutton.h"
#include "elanguage.h"

#include "characters/gods/egod.h"
#include "egodselectionwidget.h"
#include "emonsterselectionwidget.h"

#include "emainwindow.h"

#include "widgets/eeventselectionwidget.h"
#include "widgets/eepisodegoalselectionwidget.h"
#include "gameEvents/egameevent.h"
#include "widgets/echeckablebutton.h"
#include "widgets/edatebutton.h"
#include "widgets/evaluebutton.h"

#include "engine/ecampaign.h"
#include "evectorhelpers.h"

void eEditorSettingsMenu::initialize(const bool first,
                                     eCampaign* const c,
                                     eEpisode* const ep,
                                     eWorldBoard* const board) {
    setType(eFrameType::message);

    if(first) {
        const auto dateButt = new eDateButton(window());
        dateButt->initialize();
        dateButt->setDate(c->date());
        dateButt->setUnderline(false);
        dateButt->fitContent();
        dateButt->setDateChangeAction([c](const eDate& date) {
            c->setDate(date);
        });
        addWidget(dateButt);
        dateButt->align(eAlignment::hcenter);

        const auto fundsButt = new eValueButton(window());
        fundsButt->initialize(0, 99999);
        const int f = c->initialFunds(ePlayerId::player0);
        const auto fStr = std::to_string(f);
        fundsButt->setValue(f);
        fundsButt->setText(eLanguage::zeusText(44, 39) + " " + fStr);
        fundsButt->setUnderline(false);
        fundsButt->fitContent();
        fundsButt->setValueChangeAction([c, fundsButt](const int funds) {
            const auto fStr = std::to_string(funds);
            fundsButt->setText(eLanguage::zeusText(44, 39) + " " + fStr);
            c->setInitialFunds(ePlayerId::player0, funds);
        });
        addWidget(fundsButt);
        fundsButt->align(eAlignment::hcenter);

        const auto pricesAct = [this, c]() {
            const auto priceMenu = new eFramedWidget(window());
            priceMenu->setType(eFrameType::message);
            priceMenu->resize(width(), height());

            eWidget* column = nullptr;
            int i = 1;
            const auto addW = [&](eWidget* const w) {
                if(i > 5 || !column) {
                    if(column) {
                        column->stackVertically();
                        column->fitContent();
                        column->align(eAlignment::vcenter);
                    }
                    column = new eWidget(window());
                    priceMenu->addWidget(column);
                    i = 0;
                } else {
                    i++;
                }
                column->addWidget(w);
            };

            const auto res = resolution();
            const auto uiScale = res.uiScale();
            auto& prices = c->prices();
            std::map<eResourceType, eValueButton*> buttons;
            for(const auto& p : prices) {
                const auto w = new eWidget(window());
                w->setNoPadding();

                const auto type = p.first;
                const auto icon = eResourceTypeHelpers::icon(uiScale, type);
                const auto iconLabel = new eLabel(window());
                iconLabel->setTexture(icon);
                iconLabel->fitContent();
                w->addWidget(iconLabel);

                const auto priceButt = new eValueButton(window());
                buttons[type] = priceButt;
                priceButt->initialize(0, 99999);
                const int f = p.second;
                const auto fStr = std::to_string(f);
                priceButt->setValue(f);
                priceButt->setText(fStr);
                priceButt->setUnderline(false);
                priceButt->fitContent();
                priceButt->setValueChangeAction([c, type](const int price) {
                    auto& prices = c->prices();
                    prices[type] = price;
                });
                w->addWidget(priceButt);
                w->stackHorizontally();
                w->fitContent();

                addW(w);
            }

            if(column) {
                const auto resetButt = new eFramedButton(window());
                resetButt->setUnderline(false);
                resetButt->setText(eLanguage::zeusText(44, 214));
                resetButt->fitContent();
                resetButt->setPressAction([c, buttons]() {
                    auto& prices = c->prices();
                    for(auto& p : prices) {
                        const auto type = p.first;
                        const int dprice = eResourceTypeHelpers::defaultPrice(type);
                        prices[type] = dprice;
                        const auto butt = buttons.find(type);
                        if(butt != buttons.end()) {
                            butt->second->setValue(dprice);
                        }
                    }
                });
                column->addWidget(resetButt);

                column->stackVertically();
                column->fitContent();
                column->align(eAlignment::vcenter);
            }

            window()->execDialog(priceMenu);
            priceMenu->align(eAlignment::center);
            priceMenu->layoutHorizontally();
        };

        const auto pricesButt = new eFramedButton(window());
        pricesButt->setUnderline(false);
        pricesButt->setText(eLanguage::zeusText(54, 9));
        pricesButt->fitContent();
        pricesButt->setPressAction(pricesAct);
        addWidget(pricesButt);
        pricesButt->align(eAlignment::hcenter);
    }

    const auto mythologyAct = [this, ep]() {
        const auto mythMenu = new eFramedWidget(window());
        mythMenu->setType(eFrameType::message);
        mythMenu->resize(width(), height());

        const auto friendGodsAct = [this, ep]() {
            const auto choose = new eGodSelectionWidget(window());
            const auto act = [ep](const std::vector<eGodType>& godNs) {
                ep->fFriendlyGodsDelete = godNs;
            };

            const auto& iniG = ep->fFriendlyGodsDelete;
            choose->resize(width(), height());
            choose->initialize(act, iniG);

            window()->execDialog(choose);
            choose->align(eAlignment::center);
        };

        const auto friendGodsButt = new eFramedButton(window());
        friendGodsButt->setUnderline(false);
        friendGodsButt->setText(eLanguage::zeusText(44, 174));
        friendGodsButt->fitContent();
        friendGodsButt->setPressAction(friendGodsAct);
        mythMenu->addWidget(friendGodsButt);
        friendGodsButt->align(eAlignment::hcenter);

//        const auto hostileGodsAct = [this, ep]() {
//            const auto choose = new eGodSelectionWidget(window());
//            const auto act = [ep](const std::vector<eGodType>& godNs) {
//                ep->fHostileGods = godNs;
//            };

//            const auto& iniG = ep->fHostileGods;
//            choose->resize(width(), height());
//            choose->initialize(act, iniG);

//            window()->execDialog(choose);
//            choose->align(eAlignment::center);
//        };

//        const auto hostileGodsButt = new eFramedButton(window());
//        hostileGodsButt->setUnderline(false);
//        hostileGodsButt->setText(eLanguage::zeusText(44, 173));
//        hostileGodsButt->fitContent();
//        hostileGodsButt->setPressAction(hostileGodsAct);
//        mythMenu->addWidget(hostileGodsButt);
//        hostileGodsButt->align(eAlignment::hcenter);

//        const auto monstersAct = [this, ep]() {
//            const auto choose = new eMonsterSelectionWidget(window());
//            const auto act = [ep](const std::vector<eMonsterType>& monsters) {
//                ep->setHostileMonsters(monsters);
//            };

//            const auto& iniM = ep->hostileMonsters();
//            choose->resize(width(), height());
//            choose->initialize(act, iniM);

//            window()->execDialog(choose);
//            choose->align(eAlignment::center);
//        };

//        const auto monstersButt = new eFramedButton(window());
//        monstersButt->setUnderline(false);
//        monstersButt->setText(eLanguage::zeusText(44, 175));
//        monstersButt->fitContent();
//        monstersButt->setPressAction(monstersAct);
//        mythMenu->addWidget(monstersButt);
//        monstersButt->align(eAlignment::hcenter);

        window()->execDialog(mythMenu);
        mythMenu->align(eAlignment::center);
        mythMenu->layoutVertically();
    };

    const auto mythButt = new eFramedButton(window());
    mythButt->setUnderline(false);
    mythButt->setText(eLanguage::zeusText(44, 162));
    mythButt->fitContent();
    mythButt->setPressAction(mythologyAct);
    addWidget(mythButt);
    mythButt->align(eAlignment::hcenter);

    const auto eventsAct = [this, ep]() {
        const auto choose = new eEventSelectionWidget(
                                eGameEventBranch::root,
                                window());

        choose->resize(width(), height());
        const auto get = [ep]() {
            return ep->fEvents;
        };
        const auto add = [ep](const stdsptr<eGameEvent>& e) {
            e->setWorldBoard(ep->fWorldBoard);
            ep->fEvents.push_back(e);
        };
        const auto remove = [ep](const stdsptr<eGameEvent>& e) {
            eVectorHelpers::remove(ep->fEvents, e);
        };
        choose->initialize(get, add, remove);

        window()->execDialog(choose);
        choose->align(eAlignment::center);
    };

    const auto eventsButt = new eFramedButton(window());
    eventsButt->setUnderline(false);
    eventsButt->setText(eLanguage::zeusText(44, 94));
    eventsButt->fitContent();
    eventsButt->setPressAction(eventsAct);
    addWidget(eventsButt);
    eventsButt->align(eAlignment::hcenter);

    const auto goalsAct = [this, ep, board]() {
        const auto choose = new eEpisodeGoalSelectionWidget(
                                window());

        choose->resize(width(), height());
        const auto get = [ep]() {
            return ep->fGoals;
        };
        const auto add = [ep](const stdsptr<eEpisodeGoal>& e) {
            ep->fGoals.push_back(e);
        };
        const auto remove = [ep](const stdsptr<eEpisodeGoal>& e) {
            eVectorHelpers::remove(ep->fGoals, e);
        };
        choose->initialize(get, add, remove, board);

        window()->execDialog(choose);
        choose->align(eAlignment::center);
    };

    const auto goalsButt = new eFramedButton(window());
    goalsButt->setUnderline(false);
    goalsButt->setText(eLanguage::zeusText(44, 45));
    goalsButt->fitContent();
    goalsButt->setPressAction(goalsAct);
    addWidget(goalsButt);
    goalsButt->align(eAlignment::hcenter);

    const auto buildingsAct = [this, ep]() {
        const auto buildMenu = new eFramedWidget(window());
        buildMenu->setType(eFrameType::message);
        buildMenu->resize(width(), height());

        const std::vector<eBuildingType> bv {
            eBuildingType::eliteHousing,

            eBuildingType::wheatFarm,
            eBuildingType::carrotsFarm,
            eBuildingType::onionsFarm,

            eBuildingType::vine,
            eBuildingType::oliveTree,
            eBuildingType::orangeTree,

            eBuildingType::dairy,
            eBuildingType::cardingShed,

            eBuildingType::fishery,
            eBuildingType::urchinQuay,
            eBuildingType::huntingLodge,
            eBuildingType::corral,

            eBuildingType::mint,
            eBuildingType::foundry,
            eBuildingType::timberMill,
            eBuildingType::masonryShop,

            eBuildingType::winery,
            eBuildingType::olivePress,
            eBuildingType::sculptureStudio,

            eBuildingType::armory,

            eBuildingType::horseRanch,
            eBuildingType::chariotFactory,
        };

        int w = 0;
        std::vector<eCheckableButton*> buttons;
        for(const auto& type : bv) {
            const auto bb = new eCheckableButton(window());
            bb->setSmallFontSize();
            bb->setSmallPadding();
            bb->setText(eBuilding::sNameForBuilding(type));
            bb->fitContent();
            w = std::max(w, bb->width());
            bb->setChecked(ep->availableBuilding(type));
            bb->setCheckAction([type, ep](const bool b) {
                if(b) {
                    ep->fAvailableBuildingsDelete.allow(type);
                } else {
                    ep->fAvailableBuildingsDelete.disallow(type);
                }
            });
            buttons.push_back(bb);
            buildMenu->addWidget(bb);
        }
        const int p = padding();
        int x = 2*p;
        int y = 2*p;
        for(const auto b : buttons) {
            b->setWidth(w);
            const int bh = b->height();
            if(y + bh + 2*p > buildMenu->height()) {
                x += w;
                y = 2*p;
            }
            b->move(x, y);
            y += bh + p;
        }

        window()->execDialog(buildMenu);
        buildMenu->align(eAlignment::center);
    };

    const auto buildingsButt = new eFramedButton(window());
    buildingsButt->setUnderline(false);
    buildingsButt->setText(eLanguage::zeusText(44, 44));
    buildingsButt->fitContent();
    buildingsButt->setPressAction(buildingsAct);
    addWidget(buildingsButt);
    buildingsButt->align(eAlignment::hcenter);

    layoutVertically();
}
