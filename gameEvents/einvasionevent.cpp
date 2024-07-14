#include "einvasionevent.h"

#include "engine/egameboard.h"
#include "engine/eevent.h"
#include "engine/eeventdata.h"
#include "einvasionhandler.h"
#include "elanguage.h"
#include "estringhelpers.h"
#include "einvasionwarningevent.h"
#include "audio/emusic.h"

#include <algorithm>

eInvasionEvent::eInvasionEvent(const eGameEventBranch branch) :
    eGameEvent(eGameEventType::invasion, branch) {}

eInvasionEvent::~eInvasionEvent() {
    const auto board = gameBoard();
    if(board) board->removeInvasion(this);
}

void eInvasionEvent::pointerCreated() {
    const auto warnTypes = {
        eInvasionWarningType::warning36,
        eInvasionWarningType::warning24,
        eInvasionWarningType::warning12,
        eInvasionWarningType::warning6,
        eInvasionWarningType::warning1
    };
    for(const auto w : warnTypes) {
        int months;
        switch(w) {
        case eInvasionWarningType::warning36:
            months = 36;
            break;
        case eInvasionWarningType::warning24:
            months = 24;
            break;
        case eInvasionWarningType::warning12:
            months = 12;
            break;
        case eInvasionWarningType::warning6:
            months = 6;
            break;
        case eInvasionWarningType::warning1:
            months = 1;
            break;
        }
        const int daysBefore = 31*months;
        const auto e = e::make_shared<eInvasionWarningEvent>(
                           eGameEventBranch::child);
        e->initialize(w, mCity);
        addWarning(daysBefore, e);
    }
}

void eInvasionEvent::initialize(const stdsptr<eWorldCity>& city,
                                const int infantry,
                                const int cavalry,
                                const int archers) {
    setCity(city);

    mInfantry = infantry;
    mCavalry = cavalry;
    mArchers = archers;
}

void eInvasionEvent::trigger() {
    const auto board = gameBoard();
    if(!board) return;
    board->removeInvasion(this);
    eEventData ed;
    ed.fCity = mCity;
    ed.fType = eMessageEventType::invasion;
    const int bribe = bribeCost();
    ed.fBribe = bribe;
    ed.fReason = reason();

    const auto city = mCity;
    ed.fA0 = [board, city]() { // surrender
        eEventData ed;
        ed.fCity = city;
        board->event(eEvent::invasionDefeat, ed);
        board->updateMusic();
    };
    if(board->drachmas() >= bribe) { // bribe
        ed.fA1 = [board, bribe, city]() {
            board->incDrachmas(-bribe);
            eEventData ed;
            ed.fCity = city;
            board->event(eEvent::invasionBribed, ed);
            board->updateMusic();
        };
    }

    int infantry = 0;
    int cavalry = 0;
    int archers = 0;

    if(mHardcoded) {
        infantry = mInfantry;
        cavalry = mCavalry;
        archers = mArchers;
    } else {
        const int troops = std::max(12, 2*city->troops()/3);
        const auto n = city->nationality();
        switch(n) {
        case eNationality::greek: {
            infantry = std::ceil(0.6*troops);
            cavalry = std::ceil(0.2*troops);
            archers = std::ceil(0.2*troops);
        } break;
        case eNationality::trojan: {
            infantry = std::ceil(0.5*troops);
            cavalry = std::ceil(0.3*troops);
            archers = std::ceil(0.2*troops);
        } break;
        case eNationality::persian: {
            infantry = std::ceil(0.3*troops);
            cavalry = std::ceil(0.3*troops);
            archers = std::ceil(0.4*troops);
        } break;
        case eNationality::centaur: {
            infantry = 0;
            cavalry = std::ceil(0.5*troops);
            archers = std::ceil(0.5*troops);
        } break;
        case eNationality::amazon: {
            infantry = std::ceil(0.75*troops);
            cavalry = 0;
            archers = std::ceil(0.25*troops);
        } break;

        case eNationality::egyptian: {
            infantry = std::ceil(0.5*troops);
            cavalry = std::ceil(0.2*troops);
            archers = std::ceil(0.3*troops);
        } break;
        case eNationality::mayan: {
            infantry = std::ceil(0.25*troops);
            cavalry = 0;
            archers = std::ceil(0.75*troops);
        } break;
        case eNationality::phoenician: {
            infantry = 0;
            cavalry = std::ceil(0.3*troops);
            archers = std::ceil(0.7*troops);
        } break;
        case eNationality::oceanid: {
            infantry = std::ceil(0.5*troops);
            cavalry = 0;
            archers = std::ceil(0.5*troops);
        } break;
        case eNationality::atlantean: {
            infantry = std::ceil(0.4*troops);
            cavalry = std::ceil(0.3*troops);
            archers = std::ceil(0.3*troops);
        } break;
        }
    }

    const auto tile = board->landInvasionTile(mInvasionPoint);
    ed.fTile = tile;
    ed.fA2 = [board, tile, city, infantry, cavalry, archers]() { // fight
        if(!tile) return;
        const auto eh = new eInvasionHandler(*board, city);
        eh->initialize(tile, infantry, cavalry, archers);
    };
    board->event(eEvent::invasion, ed);
    eMusic::playRandomBattleMusic();
}

std::string eInvasionEvent::longName() const {
    auto tmpl = eLanguage::text("invasion_by");
    const auto none = eLanguage::text("none");
    const auto cstr = mCity ? mCity->name() : none;
    eStringHelpers::replace(tmpl, "%1", cstr);
    return tmpl;
}

void eInvasionEvent::write(eWriteStream& dst) const {
    eGameEvent::write(dst);
    dst.writeCity(mCity.get());

    dst << mHardcoded;

    dst << mInfantry;
    dst << mCavalry;
    dst << mArchers;

    dst << mInvasionPoint;

    dst << mWarned;
    mFirstWarning.write(dst);
}

void eInvasionEvent::read(eReadStream& src) {
    eGameEvent::read(src);
    src.readCity(gameBoard(), [this](const stdsptr<eWorldCity>& c) {
        mCity = c;
    });

    src >> mHardcoded;

    src >> mInfantry;
    src >> mCavalry;
    src >> mArchers;

    src >> mInvasionPoint;

    src >> mWarned;
    mFirstWarning.read(src);
}

void eInvasionEvent::setCity(const stdsptr<eWorldCity>& c) {
    mCity = c;
    const auto& ws = warnings();
    for(const auto& w : ws) {
        const auto& ws = w.second;
        const auto iw = static_cast<eInvasionWarningEvent*>(ws.get());
        iw->setCity(c);
    }
}

void eInvasionEvent::setFirstWarning(const eDate& w) {
    mFirstWarning = w;
    mWarned = true;
    const auto board = gameBoard();
    if(!board) return;
    board->addInvasion(this);
}

int eInvasionEvent::bribeCost() const {
    const auto board = gameBoard();
    if(!board) return 0;
    const auto diff = board->difficulty();
    const int rt = eDifficultyHelpers::soliderBribe(
                       diff, eCharacterType::rockThrower);
    const int ht = eDifficultyHelpers::soliderBribe(
                       diff, eCharacterType::hoplite);
    const int hm = eDifficultyHelpers::soliderBribe(
                       diff, eCharacterType::horseman);
    const int bribe = rt*mArchers + ht*mInfantry + hm*mCavalry;
    return bribe;
}
