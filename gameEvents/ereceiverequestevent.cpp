#include "ereceiverequestevent.h"

#include "engine/egameboard.h"
#include "elanguage.h"
#include "estringhelpers.h"
#include "engine/eeventdata.h"
#include "engine/eevent.h"
#include "emessages.h"

eReceiveRequestEvent::eReceiveRequestEvent(
        eGameBoard& board) :
    eGameEvent(eGameEventType::receiveRequest, board) {
    const auto e1 = eLanguage::text("early");
    mEarlyTrigger = e::make_shared<eEventTrigger>(e1, board);
    const auto e2 = eLanguage::text("comply");
    mComplyTrigger = e::make_shared<eEventTrigger>(e2, board);
    const auto e3 = eLanguage::text("refuse");
    mTooLateTrigger = e::make_shared<eEventTrigger>(e3, board);
    const auto e4 = eLanguage::text("too_late");
    mRefuseTrigger = e::make_shared<eEventTrigger>(e4, board);

    addTrigger(mEarlyTrigger);
    addTrigger(mComplyTrigger);
    addTrigger(mTooLateTrigger);
    addTrigger(mRefuseTrigger);
}

void eReceiveRequestEvent::initialize(
        const int postpone,
        const eResourceType res,
        const int count,
        const stdsptr<eWorldCity> &c) {
    mPostpone = postpone;
    mResource = res;
    mCount = count;
    mCity = c;
}

void eReceiveRequestEvent::trigger() {
    if(!mCity) return;
    auto& board = getBoard();
    eEventData ed;
    ed.fCity = mCity;
    ed.fResourceType = mResource;
    ed.fResourceCount = mCount < 0 ? -mCount : mCount;
    ed.fTime = 6;
    ed.fA0Key = "dispatch_now";
    ed.fA1Key = "postpone";
    ed.fA2Key = "refuse";

    if(mCount <= 0) {
        if(mPostpone > 1 && mPostpone < 5) {
            ed.fType = eMessageEventType::resourceGranted;
            board.event(eEvent::generalRequestTooLate, ed);
            mCity->incAttitude(-5);

            auto& msgs = eMessages::instance;
            auto& reason = msgs.fGeneralRequestTooLateReasonS;
            finished(*mTooLateTrigger, reason);
        } else {
            ed.fType = eMessageEventType::resourceGranted;
            board.event(eEvent::generalRequestComply, ed);
            mCity->incAttitude(10);

            auto& msgs = eMessages::instance;
            auto& reason = msgs.fGeneralRequestComplyReasonS;
            finished(*mComplyTrigger, reason);
        }
        return;
    }

    if(mPostpone > 3) {
        ed.fType = eMessageEventType::resourceGranted;
        board.event(eEvent::generalRequestRefuse, ed);
        mCity->incAttitude(-15);

        auto& msgs = eMessages::instance;
        auto& reason = msgs.fGeneralRequestRefuseReasonS;
        finished(*mRefuseTrigger, reason);
        return;
    }

    const int avCount = board.resourceCount(mResource);
    ed.fSpaceCount = avCount;

    ed.fType = eMessageEventType::requestTributeGranted;
    if(avCount >= mCount) {
        ed.fA0 = [this]() { // dispatch now
            auto& board = getBoard();
            board.takeResource(mResource, mCount);
            const auto e = e::make_shared<eReceiveRequestEvent>(
                        *this);
            e->initialize(mPostpone, mResource, -mCount, mCity);
            const auto date = board.date() + 3*31;
            e->initializeDate(date);
            addConsequence(e);
        };
    }

    if(mPostpone < 4) {
        ed.fA1 = [this]() { // postpone
            const auto& board = getBoard();
            const auto e = e::make_shared<eReceiveRequestEvent>(
                        *this);
            e->initialize(mPostpone + 1, mResource, mCount, mCity);
            const auto date = board.date() + 6*31;
            e->initializeDate(date);
            addConsequence(e);
        };
    }

    ed.fA2 = [this]() { // refuse
        const auto& board = getBoard();
        const auto e = e::make_shared<eReceiveRequestEvent>(
                    *this);
        e->initialize(5, mResource, mCount, mCity);
        const auto date = board.date() + 31;
        e->initializeDate(date);
        addConsequence(e);
    };


    ed.fType = eMessageEventType::generalRequestGranted;
    const auto rel = mCity->relationship();
    if(rel == eWorldCityRelationship::rival) {
        if(mPostpone == 0) { // initial
            board.event(eEvent::generalRequestRivalInitial, ed);
        } else if(mPostpone == 1) { // reminder
            board.event(eEvent::generalRequestRivalReminder, ed);
        } else if(mPostpone == 2) { // overdue
            board.event(eEvent::generalRequestRivalReminder, ed);
        } else if(mPostpone == 3) { // warning
            board.event(eEvent::generalRequestRivalWarning, ed);
        }
    } else {
        if(mPostpone == 0) { // initial
            board.event(eEvent::generalRequestInitial, ed);
        } else if(mPostpone == 1) { // reminder
            board.event(eEvent::generalRequestReminder, ed);
        } else if(mPostpone == 2) { // overdue
            board.event(eEvent::generalRequestReminder, ed);
        } else if(mPostpone == 3) { // warning
            board.event(eEvent::generalRequestWarning, ed);
        }
    }
}

std::string eReceiveRequestEvent::longName() const {
    auto tmpl = eLanguage::text("receive_request");
    const auto resName = eResourceTypeHelpers::typeName(mResource);
    const auto cStr = std::to_string(mCount);
    eStringHelpers::replace(tmpl, "%1", cStr + " " + resName);
    const auto none = eLanguage::text("none");
    const auto ctstr = mCity ? mCity->name() : none;
    eStringHelpers::replace(tmpl, "%2", ctstr);
    return tmpl;
}

void eReceiveRequestEvent::write(eWriteStream& dst) const {
    eGameEvent::write(dst);
    dst << mPostpone;
    dst << mResource;
    dst << mCount;
    dst.writeCity(mCity.get());
}

void eReceiveRequestEvent::read(eReadStream& src) {
    eGameEvent::read(src);
    src >> mPostpone;
    src >> mResource;
    src >> mCount;
    src.readCity(&getBoard(), [this](const stdsptr<eWorldCity>& c) {
        mCity = c;
    });
}

stdsptr<eGameEvent> eReceiveRequestEvent::makeCopy(const std::string& reason) const {
    const auto c = e::make_shared<eReceiveRequestEvent>(getBoard());
    c->initialize(mPostpone, mResource, mCount, mCity);
    c->setReason(reason);
    return c;
}

void eReceiveRequestEvent::setCity(const stdsptr<eWorldCity>& c) {
    mCity = c;
}

void eReceiveRequestEvent::setResourceType(const eResourceType type) {
    mResource = type;
}

void eReceiveRequestEvent::setResourceCount(const int c) {
    mCount = c;
}

void eReceiveRequestEvent::finished(eEventTrigger& t, const eReason& r) {
    const auto& board = getBoard();
    const auto date = board.date();
    auto rFull = r.fFull;
    const auto amount = std::to_string(mCount);
    eStringHelpers::replaceAll(rFull, "[amount]", amount);
    const auto item = eResourceTypeHelpers::typeLongName(mResource);
    eStringHelpers::replaceAll(rFull, "[item]", item);
    t.trigger(*this, date, rFull);
}
