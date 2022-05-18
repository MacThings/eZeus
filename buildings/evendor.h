#ifndef EVENDOR_H
#define EVENDOR_H

#include "epatrolbuilding.h"

class eVendor : public ePatrolBuilding {
public:
    eVendor(eGameBoard& board,
            const eResourceType resType,
            const eProvide provType,
            const eBaseTex& baseTex,
            const double overlayX,
            const double overlayY,
            const eBaseTex& overlayTex,
            const eCharGenerator& charGen,
            const eBuildingType type,
            const int sw, const int sh,
            const int maxEmployees);
    ~eVendor();

    std::vector<eOverlay> getOverlays(const eTileSize size) const;

    int add(const eResourceType type, const int count);
    int take(const eResourceType type, const int count);

    int count(const eResourceType type) const;
    int spaceLeft(const eResourceType type) const;

    void timeChanged(const int by);

    stdsptr<eCharacter> vendorGenerator();
private:
    const int mMaxResource = 8;
    const eResourceType mResType;
    const eProvide mProvType;
    const eCharGenerator mCharGen;
    const double mOverlayX;
    const double mOverlayY;
    const eBaseTex mOverlayTex;

    stdsptr<eCharacter> mVendor;

    int mResource{0};

    stdsptr<eCartTransporter> mCart;
};

#endif // EVENDOR_H
