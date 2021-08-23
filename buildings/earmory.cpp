#include "earmory.h"

eArmory::eArmory(eGameBoard& board) :
    eProcessingBuilding(board,
                        &eBuildingTextures::fArmory,
                        -1.75, -3.15,
                        &eBuildingTextures::fArmoryOverlay,
                        eBuildingType::armory, 2, 2,
                        eResourceType::bronze, eResourceType::armor) {

}
