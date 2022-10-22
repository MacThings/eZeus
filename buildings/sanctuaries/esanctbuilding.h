#ifndef ESANCTBUILDING_H
#define ESANCTBUILDING_H

#include "../ebuilding.h"

struct eSanctCost {
    int fWood;
    int fMarble;
    int fSculpture;

    eSanctCost& operator+=(const eSanctCost& c) {
        fWood += c.fWood;
        fMarble += c.fMarble;
        fSculpture += c.fSculpture;
        return *this;
    }

    eSanctCost& operator-=(const eSanctCost& c) {
        fWood -= c.fWood;
        fMarble -= c.fMarble;
        fSculpture -= c.fSculpture;
        return *this;
    }
};

class eSanctuary;

class eSanctBuilding : public eBuilding {
public:
    eSanctBuilding(const eSanctCost& cost,
                   const int maxProgress,
                   eGameBoard& board,
                   const eBuildingType type,
                   const int sw, const int sh);

    void erase() override;

    bool resourcesAvailable() const;

    bool finished() const;
    int progress() const { return mProgress; }
    bool incProgress();
    void destroy() { mProgress = 0; }

    const eSanctCost& cost() const { return mCost; }

    void setCost(const eSanctCost& c);

    bool workedOn() const { return mWorkedOn; }
    void setWorkedOn(const bool w) { mWorkedOn = w; }

    void setSanctuary(eSanctuary* const s);
    eSanctuary* sanctuary() const { return mSanctuary; }

    void read(eReadStream& src) override;
    void write(eWriteStream& dst) const override;
private:
    const int mMaxProgress;
    eSanctuary* mSanctuary = nullptr;
    eSanctCost mCost{0, 0, 0};
    eSanctCost mStored{0, 0, 0};
    bool mWorkedOn = false;
    int mProgress = mMaxProgress;
};

#endif // ESANCTBUILDING_H
