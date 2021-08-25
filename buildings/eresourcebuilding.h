#ifndef ERESOURCEBUILDING_H
#define ERESOURCEBUILDING_H

#include "ebuilding.h"

enum class eResourceBuildingType {
    oliveTree,
    vine
};

class eResourceBuilding : public eBuilding {
public:
    eResourceBuilding(eGameBoard& board,
                      const eResourceBuildingType type);

    eTexture getTexture(const eTileSize size) const;

    eResourceBuildingType type() const { return mType; }

    bool fullyRipe() const { return mRipe == 5; }
    int ripe() const { return mRipe; }
    int resource() const { return mResource; }
    int takeResource(const int by);
    void timeChanged();
private:
    const eResourceBuildingType mType;
    int mNextRipe = 0;
    int mRipe = 0; // 0 - 5
    int mResource = 0;
};

#endif // ERESOURCEBUILDING_H
