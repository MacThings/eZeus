#ifndef EPALACE_H
#define EPALACE_H

#include "ebuilding.h"

class ePalaceTile;

class ePalace : public eBuilding {
public:
    ePalace(eGameBoard& board, const bool r,
            const eCityId cid);
    ~ePalace();

    void erase() override;

    std::shared_ptr<eTexture>
    getTexture(const eTileSize) const override { return nullptr; }

    eTextureSpace
    getTextureSpace(const int tx, const int ty,
                    const eTileSize size) const override;

    std::vector<eOverlay>
    getOverlays(const eTileSize size) const override;

    void addTile(ePalaceTile* const tile);

    const std::vector<ePalaceTile*>& tiles() const { return mTiles; }

    bool rotated() const { return mRotated; }

    void read(eReadStream& src) override;
    void write(eWriteStream& dst) const override;
private:
    const bool mRotated;
    std::vector<ePalaceTile*> mTiles;
};

#endif // EPALACE_H
