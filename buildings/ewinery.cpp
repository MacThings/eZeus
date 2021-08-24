#include "ewinery.h"

eWinery::eWinery(eGameBoard& board) :
    eProcessingBuilding(board,
                        &eBuildingTextures::fWinery,
                        -2.2, -3.25,
                        &eBuildingTextures::fWineryOverlay,
                        eBuildingType::winery, 2, 2,
                        eResourceType::grapes,
                        eResourceType::wine, 1, 2) {

}