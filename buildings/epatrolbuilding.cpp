﻿#include "epatrolbuilding.h"

#include "characters/echaracter.h"
#include "characters/actions/epatrolaction.h"
#include "textures/egametextures.h"

ePatrolBuilding::ePatrolBuilding(eGameBoard& board,
                                 const eBaseTex baseTex,
                                 const double overlayX,
                                 const double overlayY,
                                 const eOverlays overlays,
                                 const eCharGenerator& charGen,
                                 const eActGenerator& actGen,
                                 const eBuildingType type,
                                 const int sw, const int sh,
                                 const int maxEmployees,
                                 const eCityId cid) :
    ePatrolBuildingBase(board, charGen, actGen, type,
                        sw, sh, maxEmployees, cid),
    mBaseTex(baseTex), mOverlays(overlays),
    mOverlayX(overlayX), mOverlayY(overlayY) {
    setOverlayEnabledFunc([this]() {
        return enabled() && spawnPatrolers();
    });
}

ePatrolBuilding::ePatrolBuilding(eGameBoard& board,
                                 const eBaseTex baseTex,
                                 const double overlayX,
                                 const double overlayY,
                                 const eOverlays overlays,
                                 const eCharGenerator& charGen,
                                 const eBuildingType type,
                                 const int sw, const int sh,
                                 const int maxEmployees,
                                 const eCityId cid) :
    ePatrolBuilding(board, baseTex, overlayX, overlayY,
                    overlays, charGen, sDefaultActGenerator,
                    type, sw, sh, maxEmployees, cid) {

}

std::shared_ptr<eTexture> ePatrolBuilding::getTexture(const eTileSize size) const {
    const int sizeId = static_cast<int>(size);
    const auto& blds = eGameTextures::buildings();
    return blds[sizeId].*mBaseTex;
}

std::vector<eOverlay> ePatrolBuilding::getOverlays(const eTileSize size) const {
    if(!mOverlays) return {};
    const int sizeId = static_cast<int>(size);
    const auto& blds = eGameTextures::buildings();
    const auto& coll = blds[sizeId].*mOverlays;
    const int frame = std::round(mOverlaySpeed * textureTime());
    const int texId = frame % coll.size();
    eOverlay o;
    o.fTex = coll.getTexture(texId);
    o.fX = mOverlayX;
    o.fY = mOverlayY;
    return std::vector<eOverlay>({o});
}

void ePatrolBuilding::setOverlaySpeed(const double s) {
    mOverlaySpeed = s;
}
