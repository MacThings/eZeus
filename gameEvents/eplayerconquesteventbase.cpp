#include "eplayerconquesteventbase.h"

#include "engine/egameboard.h"
#include "earmyreturnevent.h"
#include "enumbers.h"

ePlayerConquestEventBase::ePlayerConquestEventBase(
        const eGameEventType type,
        const eGameEventBranch branch,
        eGameBoard& board) :
    eArmyEventBase(type, branch, board) {
    board.addConquest(this);
}

ePlayerConquestEventBase::~ePlayerConquestEventBase() {
    removeConquestEvent();
}

void ePlayerConquestEventBase::addAres() {
    mForces.fAres = true;
}

void ePlayerConquestEventBase::removeConquestEvent() {
    const auto board = gameBoard();
    if(!board) return;
    board->removeConquest(this);
}

void ePlayerConquestEventBase::planArmyReturn() {
    const auto board = gameBoard();
    if(!board) return;
    const auto e = e::make_shared<eArmyReturnEvent>(
                       eGameEventBranch::child, *board);
    const auto boardDate = board->date();
    const int period = eNumbers::sArmyTravelTime;
    const auto date = boardDate + period;
    e->initializeDate(date, period, 1);
    e->initialize(mForces, mCity);
    addConsequence(e);
}
