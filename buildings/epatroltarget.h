#ifndef EPATROLTARGET_H
#define EPATROLTARGET_H

#include "epatrolbuilding.h"

class ePatrolTarget : public ePatrolBuilding {
public:
    ePatrolTarget(eGameBoard& board,
                  const eBuildingType fromBuilding,
                  const eBaseTex baseTex,
                  const double overlayX,
                  const double overlayY,
                  const eOverlays overlays,
                  const eCharGenerator& charGen,
                  const eBuildingType type,
                  const int sw, const int sh,
                  const int maxEmployees);

    void timeChanged(const int by);
private:
    void spawnGetActor();

    const eBuildingType mFromBuilding;
    const eCharGenerator mCharGen;

    int mAvailable = -1;

    const int mAvailableWaitTime = 50000;
    const int mSpawnWaitTime = 20000;
    int mSpawnTime = 0;
    stdptr<eCharacter> mChar;
};

#endif // EPATROLTARGET_H
