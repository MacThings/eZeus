#ifndef EINVASIONEVENT_H
#define EINVASIONEVENT_H

#include "egameevent.h"

#include "engine/eworldcity.h"
#include "characters/eenlistedforces.h"

class eInvasionHandler;
class ePlayerConquestEvent;

class eInvasionEvent : public eGameEvent {
public:
    eInvasionEvent(const eCityId cid,
                   const eGameEventBranch branch,
                   eGameBoard& board);
    ~eInvasionEvent();

    void pointerCreated() override;

    void initialize(const stdsptr<eWorldCity>& city,
                    const int infantry,
                    const int cavalry,
                    const int archers);
    void initialize(const stdsptr<eWorldCity>& city,
                    const eEnlistedForces& forces,
                    ePlayerConquestEvent* const conquestEvent);

    void trigger() override;
    std::string longName() const override;

    void write(eWriteStream& dst) const override;
    void read(eReadStream& src) override;

    bool finished() const override;

    const stdsptr<eWorldCity>& city() const { return mCity; }
    void setCity(const stdsptr<eWorldCity>& c);

    int infantry() const { return mInfantry; }
    void setInfantry(const int i) { mInfantry = i; }
    int cavalry() const { return mCavalry; }
    void setCavalry(const int c) { mCavalry = c; }
    int archers() const { return mArchers; }
    void setArchers(const int a) { mArchers = a; }

    int invasionPoint() const { return mInvasionPoint; }
    void setInvasionPoint(const int p) { mInvasionPoint = p; }

    void setFirstWarning(const eDate& w);
    eDate firstWarning() const { return mFirstWarning; }
    bool warned() const { return mWarned; }

    bool hardcoded() const { return mHardcoded; }
    void setHardcoded(const bool h) { mHardcoded = h; }

    bool activeInvasions() const;
    void addInvasionHandler(eInvasionHandler* const i);
    void removeInvasionHandler(eInvasionHandler* const i);

    bool nearestSoldier(const int fromX, const int fromY,
                        int& toX,int& toY) const;
    void defeated();

    void updateWarnings();
private:
    int bribeCost() const;

    stdsptr<eWorldCity> mCity;
    std::vector<eInvasionHandler*> mHandlers;

    bool mHardcoded = true;

    int mInfantry = 10;
    int mCavalry = 10;
    int mArchers = 10;

    stdptr<ePlayerConquestEvent> mConquestEvent;
    eEnlistedForces mForces;

    int mInvasionPoint = 0;

    bool mWarned = false;
    eDate mFirstWarning;
};

#endif // EINVASIONEVENT_H
