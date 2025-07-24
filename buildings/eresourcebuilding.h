#ifndef ERESOURCEBUILDING_H
#define ERESOURCEBUILDING_H

#include "ebuilding.h"

enum class eResourceBuildingType {
    oliveTree,
    vine,
    orangeTree
};

class eResourceBuilding : public eBuilding {
public:
    eResourceBuilding(eGameBoard& board,
                      const eResourceBuildingType type,
                      const eCityId cid);

    std::shared_ptr<eTexture> getTexture(const eTileSize size) const override;

    void erase() override;
    void sanctuaryErase();

    static bool sIsResourceBuilding(const eBuildingType type);

    eResourceBuildingType type() const { return mType; }

    bool fullyRipe() const { return mRipe == 5; }
    int ripe() const { return mRipe; }
    int resource() const { return mResource; }
    int takeResource(const int by);
    bool workedOn() const { return mWorkedOn; }
    void workOn();
    bool sanctuary() const { return mSanctuary; }
    void setSanctuary(const bool s);
    void timeChanged(const int by) override;

    void read(eReadStream& src) override;
    void write(eWriteStream& dst) const override;
private:
    const eResourceBuildingType mType;
    const int mRipePeriod;
    bool mSanctuary = false;
    bool mWorkedOn = false;
    int mNextRipe = 0;
    int mRipe = 0; // 0 - 5
    int mResource = 0;
};

#endif // ERESOURCEBUILDING_H
