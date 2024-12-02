#include "epalacetile.h"

#include "textures/egametextures.h"
#include "epalace.h"

ePalaceTile::ePalaceTile(eGameBoard& board,
                         const bool other,
                         const eCityId cid) :
    eBuilding(board, eBuildingType::palaceTile, 1, 1, cid),
    mOther(other) {
    eGameTextures::loadPalaceTiles();
}

void ePalaceTile::erase() {
    mPalace->erase();
}

std::shared_ptr<eTexture> ePalaceTile::getTexture(const eTileSize size) const {
    const int sizeId = static_cast<int>(size);
    const auto& texs = eGameTextures::buildings();
    const auto& coll = texs[sizeId].fPalaceTiles;
    const int id = mOther ? 1 : 0;
    return coll.getTexture(id);
}

int ePalaceTile::provide(const eProvide p, const int n) {
    return mPalace->provide(p, n);
}

void ePalaceTile::setPalace(ePalace* const palace) {
    mPalace = palace;
}
