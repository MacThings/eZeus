#include "efoodvendor.h"

#include "characters/efoodvendorchar.h"

eFoodVendor::eFoodVendor(const stdsptr<eCommonAgora>& agora,
                         eGameBoard& board) :
    eVendor(board, agora,
            eResourceType::food, eProvide::food,
            &eBuildingTextures::fFoodVendor,
            -2.41, -2.46, &eBuildingTextures::fFoodVendorOverlay,
            0.35, -1.94, &eBuildingTextures::fFoodVendorOverlay2,
            [this]() { return e::make_shared<eFoodVendorChar>(getBoard()); },
            eBuildingType::foodVendor, 2, 2, 4) {}
