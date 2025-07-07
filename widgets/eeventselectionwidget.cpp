#include "eeventselectionwidget.h"

#include "emainwindow.h"

#include "eventwidgets/eeventwidgetbase.h"
#include "gameEvents/egameevent.h"
#include "elanguage.h"

#include "widgets/echoosebutton.h"

#include "gameEvents/egodattackevent.h"
#include "gameEvents/einvasionevent.h"
#include "gameEvents/egiftfromevent.h"
#include "gameEvents/emakerequestevent.h"
#include "gameEvents/ereceiverequestevent.h"

#include "engine/egameboard.h"

eEventSelectionWidget::eEventSelectionWidget(
        const eGameEventBranch branch,
        eMainWindow* const window) :
    eScrollButtonsList(window),
    mBranch(branch) {}

void eEventSelectionWidget::initialize(
        const eCityId cid,
        const eEventsGetter& get,
        const eEventAdder& add,
        const eEventRemover& remove,
        eGameBoard& board) {
    eScrollButtonsList::initialize();

    const auto iniEs = get();
    for(const auto& e : iniEs) {
        const auto eStr = e->longDatedName();
        addButton(eStr);
    }

    const auto editEvent = [this](const stdsptr<eGameEvent>& e) {
        const auto settings = new eEventWidgetBase(window());
        settings->resize(width(), height());
        settings->initialize(e);

        window()->execDialog(settings);
        settings->align(eAlignment::center);
    };

    setButtonPressEvent([get, editEvent](const int id) {
        const auto es = get();
        const auto& e = es[id];
        editEvent(e);
    });

    const auto boardPtr = &board;

    setButtonCreateEvent([this, cid, add, editEvent, boardPtr]() {
        const std::vector<eGameEventType> types = {
//            eGameEventType::godVisit,
            eGameEventType::godAttack,
            eGameEventType::monsterUnleashed,
            eGameEventType::monsterInvasion,
            eGameEventType::invasion,
//            eGameEventType::payTribute,
//            eGameEventType::makeRequest,
            eGameEventType::receiveRequest,
//            eGameEventType::giftTo,
            eGameEventType::giftFrom,
            eGameEventType::godQuest,
            eGameEventType::militaryChange,
            eGameEventType::economicChange,
            eGameEventType::troopsRequest,
            eGameEventType::godDisaster,
            eGameEventType::rivalArmyAway,
            eGameEventType::earthquake,
            eGameEventType::cityBecomes,
            eGameEventType::tradeShutdowns,
            eGameEventType::tradeOpensUp,
            eGameEventType::supplyChange,
            eGameEventType::demandChange,
            eGameEventType::priceChange,
            eGameEventType::wageChange
        };
        const std::vector<std::string> labels = {
//            eLanguage::text("god_visit"),
            eLanguage::text("god_attack"),
            eLanguage::text("monster_unleash_long_name"),
            eLanguage::text("monster_invasion_long_name"),
            eLanguage::text("invasion"),
//            eLanguage::text("pay_tribute"),
//            eLanguage::text("make_request"),
            eLanguage::text("receive_request"),
//            eLanguage::text("gift_to"),
            eLanguage::text("gift_from"),
            eLanguage::text("god_quest"),
            eLanguage::text("military_change_long_name"),
            eLanguage::text("economic_change_long_name"),
            eLanguage::text("troops_request_short_name"),
            eLanguage::text("god_disaster_long_name"),
            eLanguage::text("rival_army_away_long_name"),
            eLanguage::text("earthquake_long_name"),
            eLanguage::text("city_becomes"),
            eLanguage::text("trade_shutdown_short_name"),
            eLanguage::text("trade_openup_short_name"),
            eLanguage::text("supply_change_short_name"),
            eLanguage::text("demand_change_short_name"),
            eLanguage::text("price_change_short_name"),
            eLanguage::text("wage_change")
        };
        const auto echoose = new eChooseButton(window());
        const auto act = [this, cid, add, types, labels, editEvent, boardPtr](const int val) {
            const auto type = types[val];
            const auto e = eGameEvent::sCreate(cid, type, mBranch, *boardPtr);

            if(e) {
                e->setIsEpisodeEvent(true);
                const auto boardDate = eDate(1, eMonth::january, -1500);
                const int period = 150;
                const auto date = boardDate + period;
                e->initializeDate(date, period, 1);
                add(e);
                editEvent(e);
                addButton(e->longDatedName());
            }
        };
        echoose->initialize(10, labels, act);

        window()->execDialog(echoose);
        echoose->align(eAlignment::center);
    });

    setButtonRemoveEvent([get, remove](const int id) {
        const auto es = get();
        const auto& e = es[id];
        remove(e);
    });
}
