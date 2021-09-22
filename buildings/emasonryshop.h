#ifndef EMASONRYSHOP_H
#define EMASONRYSHOP_H

#include "eresourcecollectbuilding.h"

class eMasonryShop : public eResourceCollectBuilding {
public:
    eMasonryShop(eGameBoard& board);

    std::vector<eOverlay> getOverlays(const eTileSize size) const;

    void timeChanged(const int by);

    int add(const eResourceType type, const int count) {
        if(type == eResourceType::marble && count > 0) {
            disableSpawn();
            mRawCount = 8;
            return 8;
        }
        return eResourceBuildingBase::add(type, count);
    }
private:
    int mRawCount{0};

    int mProcessWaitTime = 10000;
    int mProcessTime = mProcessWaitTime;
};

#endif // EMASONRYSHOP_H