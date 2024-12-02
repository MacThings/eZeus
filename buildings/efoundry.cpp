#include "efoundry.h"

#include "characters/ebronzeminer.h"
#include "textures/egametextures.h"

eFoundry::eFoundry(eGameBoard& board, const eCityId cid) :
    eResourceCollectBuilding(board,
                             &eBuildingTextures::fFoundry,
                             -3.80, -3.78,
                             &eBuildingTextures::fFoundryOverlay,
                             2, 1.0, -2.0,
                             [this]() { return e::make_shared<eBronzeMiner>(getBoard()); },
                             eBuildingType::foundry,
                             eHasResourceObject::sCreate(eHasResourceObjectType::copper),
                             2, 2, 15, eResourceType::bronze, cid) {
    eGameTextures::loadFoundry();
    setRawCountCollect(4);
}
