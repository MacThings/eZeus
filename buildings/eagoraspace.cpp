#include "eagoraspace.h"

#include "textures/egametextures.h"

eAgoraSpace::eAgoraSpace(const stdsptr<eAgoraBase>& agora,
                         eGameBoard& board, const eCityId cid) :
    eBuilding(board, eBuildingType::agoraSpace, 2, 2, cid),
    mAgora(agora) {}

stdsptr<eTexture> eAgoraSpace::getTexture(const eTileSize size) const {
    const int sizeId = static_cast<int>(size);
    const auto& texs = eGameTextures::buildings();
    const auto& coll = texs[sizeId].fAgora;
    return coll.getTexture(seed() % coll.size());
}
