#include "eviewmode.h"

#include "engine/egameboard.h"
#include "engine/edifficulty.h"

bool eViewModeHelpers::buildingVisible(
        const eViewMode viewMode,
        const eBuilding* const b) {
    if(!b) return false;
    const auto bt = b->type();
    if(eBuilding::sFlatBuilding(bt)) return true;

    switch(viewMode) {
    case eViewMode::defaultView: {
        return true;
    } break;

    case eViewMode::patrolBuilding: {
        return false;
    } break;

    case eViewMode::supplies: {
        return bt == eBuildingType::commonHouse ||
               bt == eBuildingType::eliteHousing;
    } break;

    case eViewMode::water: {
        return bt == eBuildingType::commonHouse ||
               bt == eBuildingType::eliteHousing ||
               bt == eBuildingType::fountain;
    } break;
    case eViewMode::hygiene: {
        return bt == eBuildingType::commonHouse ||
               bt == eBuildingType::eliteHousing ||
               bt == eBuildingType::hospital;
    } break;
    case eViewMode::hazards: {
        auto& board = b->getBoard();
        const auto diff = board.difficulty();
        const int fr = eDifficultyHelpers::fireRisk(diff, bt);
        const int dr = eDifficultyHelpers::damageRisk(diff, bt);
        return (fr || dr) && b->maintenance() < 90;
    } break;
    case eViewMode::unrest: {
        return bt == eBuildingType::commonHouse ||
               bt == eBuildingType::eliteHousing ||
               bt == eBuildingType::watchPost;
    } break;

    case eViewMode::actors: {
        return bt == eBuildingType::commonHouse ||
               bt == eBuildingType::eliteHousing ||
               bt == eBuildingType::dramaSchool ||
               bt == eBuildingType::theater;
    } break;
    case eViewMode::athletes: {
        return bt == eBuildingType::commonHouse ||
               bt == eBuildingType::eliteHousing ||
               bt == eBuildingType::gymnasium ||
               bt == eBuildingType::stadium;
    } break;
    case eViewMode::philosophers: {
        return bt == eBuildingType::commonHouse ||
               bt == eBuildingType::eliteHousing ||
               bt == eBuildingType::college ||
               bt == eBuildingType::podium;
    } break;
    case eViewMode::allCulture: {
        return bt == eBuildingType::commonHouse ||
               bt == eBuildingType::eliteHousing ||
               bt == eBuildingType::dramaSchool ||
               bt == eBuildingType::theater ||
               bt == eBuildingType::college ||
               bt == eBuildingType::podium ||
               bt == eBuildingType::gymnasium ||
               bt == eBuildingType::stadium;
    } break;

    case eViewMode::appeal: {
        return eBuilding::sAestheticsBuilding(bt);
    } break;

    case eViewMode::taxes: {
        return bt == eBuildingType::commonHouse ||
               bt == eBuildingType::eliteHousing;
    } break;
    }

    return false;
}

bool eViewModeHelpers::characterVisible(
        const eViewMode viewMode,
        const eCharacterType c) {
    switch(viewMode) {
    case eViewMode::defaultView: {
        return true;
    } break;

    case eViewMode::patrolBuilding: {
        return true;
    } break;

    case eViewMode::supplies: {
        return c == eCharacterType::waterDistributor ||
               c == eCharacterType::foodVendor ||
               c == eCharacterType::fleeceVendor ||
               c == eCharacterType::oilVendor ||
               c == eCharacterType::wineVendor ||
               c == eCharacterType::armsVendor ||
               c == eCharacterType::horseTrainer;
    } break;

    case eViewMode::water: {
        return c == eCharacterType::waterDistributor;
    } break;
    case eViewMode::hygiene: {
        return c == eCharacterType::healer;
    } break;
    case eViewMode::hazards: {
        return c == eCharacterType::fireFighter;
    } break;
    case eViewMode::unrest: {
        return c == eCharacterType::watchman;
    } break;

    case eViewMode::actors: {
        return c == eCharacterType::actor;
    } break;
    case eViewMode::athletes: {
        return c == eCharacterType::gymnast;
    } break;
    case eViewMode::philosophers: {
        return c == eCharacterType::philosopher;
    } break;
    case eViewMode::allCulture: {
        return c == eCharacterType::actor ||
               c == eCharacterType::gymnast ||
               c == eCharacterType::philosopher;
    } break;

    case eViewMode::appeal: {
        return false;
    } break;

    case eViewMode::taxes: {
        return c == eCharacterType::taxCollector;
    } break;
    }

    return false;
}
