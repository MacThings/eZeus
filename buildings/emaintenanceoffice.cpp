#include "emaintenanceoffice.h"

#include "characters/efirefighter.h"
#include "characters/actions/efirefighteraction.h"


eCharacterAction* gFireFighterActGenerator(
           eCharacter* const c,
           const std::vector<ePatrolGuide>& guides,
           const eAction& failAction,
           const eAction& finishActio) {
    return new eFireFighterAction(c, guides, failAction, finishActio);
}

eMaintenanceOffice::eMaintenanceOffice(eGameBoard& board) :
    ePatrolBuilding(board, &eBuildingTextures::fMaintenanceOffice,
                    -3.85, -4.35,
                    &eBuildingTextures::fMaintenanceOfficeOverlay,
                    [this]() { return new eFireFighter(getBoard()); },
                    gFireFighterActGenerator,
                    eBuildingType::maintenanceOffice, 2, 2)  {

}