﻿#include "egameevent.h"

#include "engine/egameboard.h"

#include "egodvisitevent.h"
#include "egodattackevent.h"
#include "emonsterattackevent.h"
#include "einvasionevent.h"
#include "epaytributeevent.h"
#include "emakerequestevent.h"
#include "ereceiverequestevent.h"
#include "egifttoevent.h"
#include "egiftfromevent.h"

eGameEvent::eGameEvent(const eGameEventType type,
                       eGameBoard& board) :
    eObject(board), mType(type) {}

stdsptr<eGameEvent> eGameEvent::sCreate(const eGameEventType type,
                                        eGameBoard& board) {
    switch(type) {
    case eGameEventType::godVisit:
        return e::make_shared<eGodVisitEvent>(board);
    case eGameEventType::godAttack:
        return e::make_shared<eGodAttackEvent>(board);
    case eGameEventType::monsterAttack:
        return e::make_shared<eMonsterAttackEvent>(board);
    case eGameEventType::invasion:
        return e::make_shared<eInvasionEvent>(board);
    case eGameEventType::payTribute:
        return e::make_shared<ePayTributeEvent>(board);
    case eGameEventType::makeRequest:
        return e::make_shared<eMakeRequestEvent>(board);
    case eGameEventType::receiveRequest:
        return e::make_shared<eReceiveRequestEvent>(board);
    case eGameEventType::giftTo:
        return e::make_shared<eGiftToEvent>(board);
    case eGameEventType::giftFrom:
        return e::make_shared<eGiftFromEvent>(board);
    }
    return nullptr;
}

void eGameEvent::initializeDate(const eDate& startDate,
                                const int cycleDays,
                                const int nRuns) {
    mStartDate = startDate;
    mPeriodDays = cycleDays;
    mTotNRuns = nRuns;
    mRemNRuns = nRuns;
    mNextDate = startDate;
    rewind();
}

void eGameEvent::addWarning(const int daysBefore,
                            const stdsptr<eGameEvent>& event) {
    const auto startDate = mStartDate - daysBefore;
    event->initializeDate(startDate);
    mWarnings.emplace_back(daysBefore, event);
}

void eGameEvent::addConsequence(
        const stdsptr<eGameEvent>& event) {
    mConsequences.push_back(event);
}

std::string eGameEvent::longDatedName() const {
    const auto dateStr = mStartDate.shortString();
    const auto eventName = longName();
    return dateStr + " " + eventName;
}

void eGameEvent::setStartDate(const eDate& d) {
    mStartDate = d;
    for(const auto& w : mWarnings) {
        w.second->setStartDate(d - w.first);
    }
    rewind();
}

void eGameEvent::setPeriod(const int p) {
    mPeriodDays = p;
    for(const auto& w : mWarnings) {
        w.second->setPeriod(p);
    }
    rewind();
}

void eGameEvent::setRepeat(const int r) {
    mTotNRuns = r;
    for(const auto& w : mWarnings) {
        w.second->setRepeat(r);
    }
    rewind();
}

void eGameEvent::handleNewDate(const eDate& date) {
    for(const auto& w : mWarnings) {
        w.second->handleNewDate(date);
    }
    for(const auto& c : mConsequences) {
        c->handleNewDate(date);
    }
    if(finished()) return;
    if(date > mNextDate) {
        trigger();
        mRemNRuns--;
        mNextDate += mPeriodDays;
    }
}

void eGameEvent::rewind() {
    const auto& board = getBoard();
    const auto date = board.date();
    rewind(date);
}

void eGameEvent::rewind(const eDate& date) {
    for(const auto& w : mWarnings) {
        w.second->rewind(date);
    }
    mConsequences.clear();

    mNextDate = mStartDate;
    mRemNRuns = mTotNRuns;
    if(mPeriodDays <= 0) {
        if(date > mStartDate) mRemNRuns--;
        return;
    }
    while(date > mNextDate) {
        mRemNRuns--;
        if(mRemNRuns <= 0) break;
        mNextDate += mPeriodDays;
    }
}

void eGameEvent::write(eWriteStream& dst) const {
    mNextDate.write(dst);
    mStartDate.write(dst);
    dst << mPeriodDays;
    dst << mTotNRuns;
    dst << mRemNRuns;

    dst << mWarnings.size();
    for(const auto& w : mWarnings) {
        dst << w.first;
        dst << w.second->type();
        w.second->write(dst);
    }

    dst << mConsequences.size();
    for(const auto& c : mConsequences) {
        dst << c->type();
        c->write(dst);
    }
}

void eGameEvent::read(eReadStream& src) {
    mNextDate.read(src);
    mStartDate.read(src);
    src >> mPeriodDays;
    src >> mTotNRuns;
    src >> mRemNRuns;

    int nws;
    src >> nws;
    for(int i = 0; i < nws; i++) {
        int days;
        src >> days;
        eGameEventType type;
        src >> type;
        const auto e = eGameEvent::sCreate(type, getBoard());
        e->read(src);
        mWarnings.emplace_back(eWarning(days, e));
    }

    int ncs;
    src >> ncs;
    for(int i = 0; i < ncs; i++) {
        eGameEventType type;
        src >> type;
        const auto e = eGameEvent::sCreate(type, getBoard());
        e->read(src);
        mConsequences.emplace_back(e);
    }
}
