#include "earmyreturnevent.h"

#include "engine/egameboard.h"
#include "engine/eeventdata.h"
#include "engine/eevent.h"
#include "elanguage.h"
#include "buildings/eheroshall.h"
#include "characters/actions/eheroaction.h"
#include "characters/actions/egodworshippedaction.h"

eArmyReturnEvent::eArmyReturnEvent(
        const eCityId cid,
        const eGameEventBranch branch,
        eGameBoard& board) :
    eArmyEventBase(cid, eGameEventType::armyReturnEvent, branch, board) {}

void eArmyReturnEvent::initialize(
        const eEnlistedForces& forces,
        const stdsptr<eWorldCity>& city) {
    mForces = forces;
    mCity = city;
}

void eArmyReturnEvent::trigger() {
    removeArmyEvent();
    if(!mCity) return;
    const auto board = gameBoard();
    if(!board) return;
    const auto cid = cityId();
    const auto entryPoint = board->entryPoint(cid);

    int wait = 0;

    if(mForces.fAres) {
        const auto as = board->sanctuary(cid, eGodType::ares);
        if(as) {
            as->godComeback();
            if(entryPoint) {
                const auto god = as->spawnGod();
                if(god) {
                    const auto ga = e::make_shared<eGodWorshippedAction>(god);
                    god->setAction(ga);
                    god->changeTile(entryPoint);
                    ga->goBackToSanctuary();
                    wait += 150;
                }
            }
        }
    }

    for(const auto h : mForces.fHeroes) {
        const auto hh = board->heroHall(h.first, h.second);
        if(!hh) continue;
        hh->setHeroOnQuest(false);
        if(!entryPoint) continue;
        const auto hero = hh->spawnHero();
        const auto a = hero->action();
        const auto ha = dynamic_cast<eHeroAction*>(a);
        if(!ha) continue;
        hero->changeTile(entryPoint);
        ha->waitAndGoBackToHall(wait);
        wait += 150;
    }

    for(const auto& s : mForces.fSoldiers) {
        s->killAll();
        s->setBothCityIds(cid);
        s->backFromAbroad(wait);
    }

    for(const auto& a : mForces.fAllies) {
        a->setAbroad(false);
    }

    eEventData ed(cityId());
    ed.fTile = entryPoint;
    ed.fCity = mCity;
    board->event(eEvent::armyReturns, ed);
}

std::string eArmyReturnEvent::longName() const {
    return eLanguage::text("army_returns_event_long_name");
}
