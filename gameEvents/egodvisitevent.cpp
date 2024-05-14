#include "egodvisitevent.h"

#include "engine/egameboard.h"
#include "engine/eevent.h"
#include "engine/eeventdata.h"
#include "characters/actions/egodvisitaction.h"
#include "elanguage.h"

eGodVisitEvent::eGodVisitEvent(eGameBoard& board) :
    eGameEvent(eGameEventType::godVisit, board) {}

void eGodVisitEvent::setTypes(const std::vector<eGodType>& types) {
    mTypes = types;
    const int nTypes = mTypes.size();
    if(mNextId >= nTypes) mNextId = 0;
}

void eGodVisitEvent::setRandom(const bool r) {
    mRandom = r;
}

void eGodVisitEvent::trigger() {
    if(mTypes.empty()) return;
    auto& board = getBoard();
    int tid;
    const int nTypes = mTypes.size();
    if(mRandom) {
        tid = rand() % mTypes.size();
    } else {
        tid = mNextId;
        if(++mNextId >= nTypes) mNextId = 0;
    }
    const auto t = mTypes.at(tid);
    const auto god = eGod::sCreateGod(t, board);

    const auto a = e::make_shared<eGodVisitAction>(god.get());
    god->setAction(a);
    god->setAttitude(eGodAttitude::friendly);
    a->increment(1);
    eEventData ed;
    ed.fChar = god.get();
    ed.fTile = god->tile();
    ed.fGod = t;
    board.event(eEvent::godVisit, ed);
}

std::string eGodVisitEvent::longName() const {
    return eLanguage::text("god_visit");
}

void eGodVisitEvent::write(eWriteStream& dst) const {
    eGameEvent::write(dst);
    dst << mTypes.size();
    for(const auto t : mTypes) {
        dst << t;
    }
    dst << mRandom;
    dst << mNextId;
}

void eGodVisitEvent::read(eReadStream& src) {
    eGameEvent::read(src);
    int n;
    src >> n;
    for(int i = 0; i < n; i++) {
        eGodType t;
        src >> t;
        mTypes.push_back(t);
    }
    src >> mRandom;
    src >> mNextId;
}

stdsptr<eGameEvent> eGodVisitEvent::makeCopy(const std::string& reason) const {
    const auto c = e::make_shared<eGodVisitEvent>(getBoard());
    c->mTypes = mTypes;
    c->mNextId = mNextId;
    c->mRandom = mRandom;
    c->setReason(reason);
    return c;
}
