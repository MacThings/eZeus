#ifndef ETEMPLEBUILDING_H
#define ETEMPLEBUILDING_H

#include "esanctbuilding.h"

class eTempleBuilding : public eSanctBuilding {
public:
    eTempleBuilding(eGameBoard& board, const eCityId cid);
    eTempleBuilding(const int id, eGameBoard& board,
                    const eCityId cid);

    std::shared_ptr<eTexture>
    getTexture(const eTileSize size) const override;

    std::vector<eOverlay>
    getOverlays(const eTileSize size) const override;

    void read(eReadStream& src) override;
    void write(eWriteStream& dst) const override;
private:
    bool hasNeighbour() const;

    int rotatedId() const;

    mutable bool mUpdateHasNeighbour = true;
    mutable bool mHasNeighbour = false;
    int mId;
};

#endif // ETEMPLEBUILDING_H
