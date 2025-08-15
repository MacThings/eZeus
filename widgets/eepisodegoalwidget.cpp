#include "eepisodegoalwidget.h"

#include "engine/eepisodegoal.h"
#include "elabel.h"

#include "evaluebutton.h"
#include "egodbutton.h"
#include "epyramidbutton.h"
#include "echoosebutton.h"
#include "emainwindow.h"
#include "ecitybutton.h"
#include "edatebutton.h"
#include "eresourcebutton.h"
#include "buildings/esmallhouse.h"
#include "buildings/eelitehousing.h"
#include "buildings/pyramids/epyramid.h"
#include "elanguage.h"
#include "elabeledwidget.h"
#include "engine/egameboard.h"

void eEpisodeGoalWidget::initialize(const stdsptr<eEpisodeGoal>& e,
                                    eGameBoard* const board) {
    setType(eFrameType::message);

    const auto cont = new eWidget(window());
    cont->setNoPadding();
    addWidget(cont);
    const int p = padding();

    const auto textL = new eLabel(window());
    textL->setSmallFontSize();
    textL->setSmallPadding();
    cont->addWidget(textL);

    const auto updateText = [textL, e, board]() {
        textL->setText(e->text(false, false, *board));
        textL->fitContent();
        textL->align(eAlignment::hcenter);
    };

    updateText();

    const auto detailsW = new eWidget(window());
    detailsW->setNoPadding();

    switch(e->fType) {
    case eEpisodeGoalType::population:
    case eEpisodeGoalType::treasury:
    case eEpisodeGoalType::hippodrome: {
        const auto b = new eValueButton(window());
        b->initialize(0, 99999);
        b->setValue(e->fRequiredCount);
        b->setValueChangeAction([e, updateText](const int c) {
            e->fRequiredCount = c;
            updateText();
        });
        detailsW->addWidget(b);
    } break;
    case eEpisodeGoalType::sanctuary: {
        const auto g = new eGodButton(window());
        g->initialize([e, updateText](const eGodType god) {
            e->fEnumInt1 = static_cast<int>(god);
            updateText();
        });
        const auto t = static_cast<eGodType>(e->fEnumInt1);
        g->setType(t);
        detailsW->addWidget(g);
    } break;
    case eEpisodeGoalType::pyramid: {
        const auto g = new eGodButton(window());

        const auto b = new eValueButton(window());

        const auto p = new ePyramidButton(window());
        p->initialize([g, b, e, updateText](const eBuildingType type) {
            const bool toGod = ePyramid::sIsToGod(type);
            if(toGod) {
                g->setType(ePyramid::sGod(type));
                g->show();
            } else {
                g->hide();
            }
            const bool isNone = type == eBuildingType::none;
            if(!isNone) e->fRequiredCount = 100;
            else e->fRequiredCount = 1;
            b->setVisible(isNone);
            b->setValue(e->fRequiredCount);
            e->fEnumInt1 = static_cast<int>(type);
        });
        detailsW->addWidget(p);
        auto t = static_cast<eBuildingType>(e->fEnumInt1);
        p->setType(t);

        g->initialize([e, updateText](const eGodType god) {
            auto t = static_cast<eBuildingType>(e->fEnumInt1);
            t = ePyramid::sSwitchGod(t, god);
            e->fEnumInt1 = static_cast<int>(t);
            updateText();
        });
        const bool toGod = ePyramid::sIsToGod(t);
        if(toGod) g->setType(ePyramid::sGod(t));
        g->setVisible(toGod);
        detailsW->addWidget(g);

        b->initialize(0, 9);
        b->setValue(e->fRequiredCount);
        b->setValueChangeAction([e, updateText](const int c) {
            e->fRequiredCount = c;
            updateText();
        });
        b->setVisible(t == eBuildingType::none);
        detailsW->addWidget(b);
    } break;
    case eEpisodeGoalType::support: {
        const auto type = new eFramedButton(window());
        type->setUnderline(false);
        type->setPressAction([this, e, type, updateText]() {
            const auto choose = new eChooseButton(window());
            const std::vector<eBannerType> types{
                eBannerType::rockThrower,
                eBannerType::hoplite,
                eBannerType::horseman,
                eBannerType::trireme
            };
            std::vector<std::string> names;
            for(const auto t : types) {
                names.push_back(eSoldierBanner::sName(t, false));
            }
            const auto act = [e, type, updateText, types](const int val) {
                const auto t = types[val];
                e->fEnumInt1 = static_cast<int>(t);
                updateText();
                type->setText(eSoldierBanner::sName(t, false));
            };
            choose->initialize(8, names, act);

            window()->execDialog(choose);
            choose->align(eAlignment::center);
        });
        const auto t = static_cast<eBannerType>(e->fEnumInt1);
        type->setText(eSoldierBanner::sName(t, false));
        type->fitContent();
        detailsW->addWidget(type);

        const auto b = new eValueButton(window());
        b->initialize(0, 99999);
        b->setValue(e->fRequiredCount);
        b->setValueChangeAction([e, updateText](const int c) {
            e->fRequiredCount = c;
            updateText();
        });
        detailsW->addWidget(b);
    } break;
    case eEpisodeGoalType::quest: {
        const auto b = new eValueButton(window());
        b->initialize(0, 9);
        b->setValue(e->fRequiredCount);
        b->setValueChangeAction([e, updateText](const int c) {
            e->fRequiredCount = c;
            updateText();
        });
        detailsW->addWidget(b);
    } break;
    case eEpisodeGoalType::yearlyProduction: {
        const auto resButton = new eResourceButton(window());
        resButton->initialize([e, updateText](
                              const eResourceType r) {
            e->fEnumInt1 = static_cast<int>(r);
            updateText();
        });
        auto res = static_cast<eResourceType>(e->fEnumInt1);
        if(res == eResourceType::none) {
            res = eResourceType::marble;
            e->fEnumInt1 = static_cast<int>(res);
        }
        resButton->setResource(res);
        detailsW->addWidget(resButton);

        const auto b = new eValueButton(window());
        b->initialize(0, 999);
        b->setValue(e->fRequiredCount);
        b->setValueChangeAction([e, updateText](const int c) {
            e->fRequiredCount = c;
            updateText();
        });
        detailsW->addWidget(b);
    } break;
    case eEpisodeGoalType::slay: {
        const auto b = new eValueButton(window());
        b->initialize(0, 9);
        b->setValue(e->fRequiredCount);
        b->setValueChangeAction([e, updateText](const int c) {
            e->fRequiredCount = c;
            updateText();
        });
        detailsW->addWidget(b);
    } break;
    case eEpisodeGoalType::yearlyProfit: {
        const auto b = new eValueButton(window());
        b->initialize(0, 99999);
        b->setValue(e->fRequiredCount);
        b->setValueChangeAction([e, updateText](const int c) {
            e->fRequiredCount = c;
            updateText();
        });
        detailsW->addWidget(b);
    } break;
    case eEpisodeGoalType::rule: {
        const auto cityButton = new eCityButton(window());
        cityButton->initialize(&board->world(), [e, updateText](const stdsptr<eWorldCity>& c){
            e->fEnumInt1 = static_cast<int>(c->cityId());
            updateText();
        });
        const auto& world = board->world();
        const auto cid = static_cast<eCityId>(e->fEnumInt1);
        const auto city = world.cityWithId(cid);
        cityButton->setCity(city);
        detailsW->addWidget(cityButton);
    } break;
    case eEpisodeGoalType::housing: {
        const auto type = new eFramedButton(window());
        type->setUnderline(false);
        const auto nameGetter = [e]() {
            std::string name;
            const auto t = e->fEnumInt1 == 0 ? eBuildingType::commonHouse :
                                               eBuildingType::eliteHousing;
            switch(t) {
            case eBuildingType::commonHouse:
                return eSmallHouse::sName(e->fEnumInt2);
            case eBuildingType::eliteHousing:
                return eEliteHousing::sName(e->fEnumInt2);
            default:
                break;
            }
            return std::string();
        };
        type->setText(nameGetter());
        type->fitContent();
        type->setPressAction([this, e, updateText, type, nameGetter]() {
            const auto choose = new eChooseButton(window());
            std::vector<std::string> names;
            for(int i = 0; i < 7; i++) {
                names.push_back(eSmallHouse::sName(i));
            }
            for(int i = 0; i < 5; i++) {
                names.push_back(eEliteHousing::sName(i));
            }
            const auto act = [e, updateText, type, nameGetter](const int val) {
                e->fEnumInt1 = val > 6 ? 1 : 0;
                e->fEnumInt2 = val > 6 ? (val - 6) : val;
                {
                    const auto name = nameGetter();
                    type->setText(name);
                }
                updateText();
            };
            choose->initialize(7, names, act);

            window()->execDialog(choose);
            choose->align(eAlignment::center);
        });
        detailsW->addWidget(type);

        const auto b = new eValueButton(window());
        b->initialize(0, 99999);
        b->setValue(e->fRequiredCount);
        b->setValueChangeAction([e, updateText](const int c) {
            e->fRequiredCount = c;
            updateText();
        });
        detailsW->addWidget(b);
    } break;
    case eEpisodeGoalType::setAsideGoods: {
        const auto resButton = new eResourceButton(window());
        resButton->initialize([e, updateText](
                              const eResourceType r) {
            e->fEnumInt1 = static_cast<int>(r);
            updateText();
        });
        auto res = static_cast<eResourceType>(e->fEnumInt1);
        if(res == eResourceType::none) {
            res = eResourceType::marble;
            e->fEnumInt1 = static_cast<int>(res);
        }
        resButton->setResource(res);
        detailsW->addWidget(resButton);

        const auto b = new eValueButton(window());
        b->initialize(0, 99999);
        b->setValue(e->fRequiredCount);
        b->setValueChangeAction([e, updateText](const int c) {
            e->fRequiredCount = c;
            updateText();
        });
        detailsW->addWidget(b);
    } break;
    case eEpisodeGoalType::surviveUntil:
    case eEpisodeGoalType::completeBefore: {
        const auto dateW = new eWidget(window());
        dateW->setNoPadding();

        const auto yearsButtonL = new eLabeledWidget(window());
        const auto yearsButton = new eValueButton(window());
        yearsButton->setValueChangeAction([e, yearsButton](const int y) {
            e->fRequiredCount = y;
            yearsButton->setText("+" + yearsButton->text());
        });
        yearsButton->initialize(0, 99999);
        yearsButton->setValue(e->fRequiredCount);
        yearsButton->setText("+" + yearsButton->text());
        yearsButtonL->setup(eLanguage::text("years:"), yearsButton);
        dateW->addWidget(yearsButtonL);

        const auto monthssButtonL = new eLabeledWidget(window());
        const auto monthsButton = new eValueButton(window());
        monthsButton->setValueChangeAction([e, monthsButton](const int y) {
            e->fEnumInt2 = y;
            monthsButton->setText("+" + monthsButton->text());
        });
        monthsButton->initialize(0, 99999);
        monthsButton->setValue(e->fEnumInt2);
        monthsButton->setText("+" + monthsButton->text());
        monthssButtonL->setup(eLanguage::text("months:"), monthsButton);
        dateW->addWidget(monthssButtonL);

        const auto daysButtonL = new eLabeledWidget(window());
        const auto daysButton = new eValueButton(window());
        daysButton->setValueChangeAction([e, daysButton](const int y) {
            e->fEnumInt1 = y;
            daysButton->setText("+" + daysButton->text());
        });
        daysButton->initialize(0, 99999);
        daysButton->setValue(e->fEnumInt1);
        daysButton->setText("+" + daysButton->text());
        daysButtonL->setup(eLanguage::text("days:"), daysButton);
        dateW->addWidget(daysButtonL);
        dateW->stackVertically(p);
        dateW->fitContent();
        detailsW->addWidget(dateW);
    } break;
    case eEpisodeGoalType::tradingPartners: {
        const auto b = new eValueButton(window());
        b->initialize(0, 99);
        b->setValue(e->fRequiredCount);
        b->setValueChangeAction([e, updateText](const int c) {
            e->fRequiredCount = c;
            updateText();
        });
        detailsW->addWidget(b);
    } break;
    }

    detailsW->stackVertically(p);
    detailsW->fitContent();
    cont->addWidget(detailsW);

    cont->stackVertically();
    cont->fitContent();

    cont->align(eAlignment::center);
    updateText();
}

