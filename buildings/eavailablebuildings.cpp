#include "eavailablebuildings.h"

#include "fileIO/ereadstream.h"
#include "fileIO/ewritestream.h"

#include "buildings/ebuilding.h"

#include <algorithm>

void eAvailableBuildings::read(eReadStream& src) {
    src >> fEliteHousing;

    src >> fWheatFarm;
    src >> fCarrotsFarm;
    src >> fOnionFarm;

    src >> fVine;
    src >> fOliveTree;
    src >> fOrangeTree;

    src >> fDairy;
    src >> fCardingShed;

    src >> fFishery;
    src >> fUrchinQuay;
    src >> fHuntingLodge;

    src >> fMint;
    src >> fFoundry;
    src >> fTimberMill;
    src >> fMasonryShop;

    src >> fRefinery;
    src >> fBlackMarbleWorkshop;

    src >> fWinery;
    src >> fOlivePress;
    src >> fSculptureStudio;

    src >> fArmory;

    src >> fHorseRanch;
    src >> fChariotFactory;

    src >> fTriremeWharf;

    src >> fModestPyramid;
    src >> fPyramid;
    src >> fGreatPyramid;
    src >> fMajesticPyramid;

    src >> fSmallMonumentToTheSky;
    src >> fMonumentToTheSky;
    src >> fGrandMonumentTotTheSky;

    src >> fMinorShrine;
    src >> fShrine;
    src >> fMajorShrine;

    src >> fPyramidToThePantheon;
    src >> fAltarOfOlympus;
    src >> fTempleOfOlympus;
    src >> fObservatoryKosmika;
    src >> fMuseumAtlantika;

    src >> fAphroditeSanctuary;
    src >> fApolloSanctuary;
    src >> fAresSanctuary;
    src >> fArtemisSanctuary;
    src >> fAthenaSanctuary;
    src >> fAtlasSanctuary;
    src >> fDemeterSanctuary;
    src >> fDionysusSanctuary;
    src >> fHadesSanctuary;
    src >> fHephaestusSanctuary;
    src >> fHeraSanctuary;
    src >> fHermesSanctuary;
    src >> fPoseidonSanctuary;
    src >> fZeusSanctuary;

    src >> fAchillesHall;
    src >> fAtalantaHall;
    src >> fBellerophonHall;
    src >> fHerculesHall;
    src >> fJasonHall;
    src >> fOdysseusHall;
    src >> fPerseusHall;
    src >> fTheseusHall;

    src >> fPopulationMonument;
    src >> fVictoryMonument;
    src >> fColonyMonument;
    src >> fAthleteMonument;
    src >> fConquestMonument;
    src >> fHappinessMonument;
    src >> fHeroicMonument;
    src >> fDiplomacyMonument;
    src >> fScholarMonument;

    const int iMax = static_cast<int>(eGodType::zeus) + 1;
    for(int i = 0; i < iMax; i++) {
        const auto t = static_cast<eGodType>(i);
        src >> mGodMonuments[t];
    }
}

void eAvailableBuildings::write(eWriteStream& dst) const {
    dst << fEliteHousing;

    dst << fWheatFarm;
    dst << fCarrotsFarm;
    dst << fOnionFarm;

    dst << fVine;
    dst << fOliveTree;
    dst << fOrangeTree;

    dst << fDairy;
    dst << fCardingShed;

    dst << fFishery;
    dst << fUrchinQuay;
    dst << fHuntingLodge;

    dst << fMint;
    dst << fFoundry;
    dst << fTimberMill;
    dst << fMasonryShop;

    dst << fRefinery;
    dst << fBlackMarbleWorkshop;

    dst << fWinery;
    dst << fOlivePress;
    dst << fSculptureStudio;

    dst << fArmory;

    dst << fHorseRanch;
    dst << fChariotFactory;

    dst << fTriremeWharf;

    dst << fModestPyramid;
    dst << fPyramid;
    dst << fGreatPyramid;
    dst << fMajesticPyramid;

    dst << fSmallMonumentToTheSky;
    dst << fMonumentToTheSky;
    dst << fGrandMonumentTotTheSky;

    dst << fMinorShrine;
    dst << fShrine;
    dst << fMajorShrine;

    dst << fPyramidToThePantheon;
    dst << fAltarOfOlympus;
    dst << fTempleOfOlympus;
    dst << fObservatoryKosmika;
    dst << fMuseumAtlantika;

    dst << fAphroditeSanctuary;
    dst << fApolloSanctuary;
    dst << fAresSanctuary;
    dst << fArtemisSanctuary;
    dst << fAthenaSanctuary;
    dst << fAtlasSanctuary;
    dst << fDemeterSanctuary;
    dst << fDionysusSanctuary;
    dst << fHadesSanctuary;
    dst << fHephaestusSanctuary;
    dst << fHeraSanctuary;
    dst << fHermesSanctuary;
    dst << fPoseidonSanctuary;
    dst << fZeusSanctuary;

    dst << fAchillesHall;
    dst << fAtalantaHall;
    dst << fBellerophonHall;
    dst << fHerculesHall;
    dst << fJasonHall;
    dst << fOdysseusHall;
    dst << fPerseusHall;
    dst << fTheseusHall;

    dst << fPopulationMonument;
    dst << fVictoryMonument;
    dst << fColonyMonument;
    dst << fAthleteMonument;
    dst << fConquestMonument;
    dst << fHappinessMonument;
    dst << fHeroicMonument;
    dst << fDiplomacyMonument;
    dst << fScholarMonument;

    const int iMax = static_cast<int>(eGodType::zeus) + 1;
    for(int i = 0; i < iMax; i++) {
        const auto t = static_cast<eGodType>(i);
        dst << mGodMonuments.at(t);
    }
}

bool eAvailableBuildings::available(
        const eBuildingType type,
        const int id) const {
    switch(type) {
    case eBuildingType::eliteHousing:
        return fEliteHousing;

    case eBuildingType::wheatFarm:
        return fWheatFarm;
    case eBuildingType::carrotsFarm:
        return fCarrotsFarm;
    case eBuildingType::onionsFarm:
        return fOnionFarm;

    case eBuildingType::growersLodge: {
        const bool s = fAthenaSanctuary == eAvailable::built ||
                       fDionysusSanctuary == eAvailable::built;
        if(s) return true;
        return fVine || fOliveTree;
    } break;
    case eBuildingType::vine:
        return fVine;
    case eBuildingType::oliveTree:
        return fOliveTree;
    case eBuildingType::orangeTendersLodge: {
        const bool s = fHeraSanctuary == eAvailable::built;
        if(s) return true;
        return fOrangeTree;
    } break;
    case eBuildingType::orangeTree:
        return fOrangeTree;

    case eBuildingType::dairy:
    case eBuildingType::goat:
        return fDairy;
    case eBuildingType::cardingShed:
    case eBuildingType::sheep:
        return fCardingShed;

    case eBuildingType::fishery:
        return fFishery;
    case eBuildingType::urchinQuay:
        return fUrchinQuay;
    case eBuildingType::huntingLodge:
        return fHuntingLodge;
    case eBuildingType::corral:
    case eBuildingType::cattle:
        return fCorral;

    case eBuildingType::mint: {
        const bool s = fHadesSanctuary == eAvailable::built;
        if(s) return true;
        return fMint;
    } break;
    case eBuildingType::foundry: {
        const bool s = fHephaestusSanctuary == eAvailable::built;
        if(s) return true;
        return fFoundry;
    } break;
    case eBuildingType::timberMill:
        return fTimberMill;
    case eBuildingType::masonryShop:
        return fMasonryShop;
    case eBuildingType::refinery:
        return fRefinery;
    case eBuildingType::blackMarbleWorkshop:
        return fBlackMarbleWorkshop;

    case eBuildingType::winery: {
        const bool s = fDionysusSanctuary == eAvailable::built;
        if(s) return true;
        return fWinery;
    } break;
    case eBuildingType::olivePress: {
        const bool s = fAthenaSanctuary == eAvailable::built;
        if(s) return true;
        return fOlivePress;
    } break;
    case eBuildingType::sculptureStudio:
        return fSculptureStudio;

    case eBuildingType::armory:
        return fArmory;

    case eBuildingType::horseRanch:
        return fHorseRanch;
    case eBuildingType::chariotFactory:
        return fChariotFactory;

    case eBuildingType::triremeWharf:
        return fTriremeWharf;

    case eBuildingType::modestPyramid:
        return fModestPyramid == eAvailable::available;
    case eBuildingType::pyramid:
        return fPyramid == eAvailable::available;
    case eBuildingType::greatPyramid:
        return fGreatPyramid == eAvailable::available;
    case eBuildingType::majesticPyramid:
        return fMajesticPyramid == eAvailable::available;

    case eBuildingType::smallMonumentToTheSky:
        return fSmallMonumentToTheSky == eAvailable::available;
    case eBuildingType::monumentToTheSky:
        return fMonumentToTheSky == eAvailable::available;
    case eBuildingType::grandMonumentToTheSky:
        return fGrandMonumentTotTheSky == eAvailable::available;

    case eBuildingType::minorShrine:
        return fMinorShrine == eAvailable::available;
    case eBuildingType::shrine:
        return fShrine == eAvailable::available;
    case eBuildingType::majorShrine:
        return fMajorShrine == eAvailable::available;

    case eBuildingType::pyramidOfThePantheon:
        return fPyramidToThePantheon == eAvailable::available;
    case eBuildingType::altarOfOlympus:
        return fAltarOfOlympus == eAvailable::available;
    case eBuildingType::templeOfOlympus:
        return fTempleOfOlympus == eAvailable::available;
    case eBuildingType::observatoryKosmika:
        return fObservatoryKosmika == eAvailable::available;
    case eBuildingType::museumAtlantika:
        return fMuseumAtlantika == eAvailable::available;

    case eBuildingType::templeAphrodite:
        return fAphroditeSanctuary == eAvailable::available;
    case eBuildingType::templeApollo:
        return fApolloSanctuary == eAvailable::available;
    case eBuildingType::templeAres:
        return fAresSanctuary == eAvailable::available;
    case eBuildingType::templeArtemis:
        return fArtemisSanctuary == eAvailable::available;
    case eBuildingType::templeAthena:
        return fAthenaSanctuary == eAvailable::available;
    case eBuildingType::templeAtlas:
        return fAtlasSanctuary == eAvailable::available;
    case eBuildingType::templeDemeter:
        return fDemeterSanctuary == eAvailable::available;
    case eBuildingType::templeDionysus:
        return fDionysusSanctuary == eAvailable::available;
    case eBuildingType::templeHades:
        return fHadesSanctuary == eAvailable::available;
    case eBuildingType::templeHephaestus:
        return fHephaestusSanctuary == eAvailable::available;
    case eBuildingType::templeHera:
        return fHeraSanctuary == eAvailable::available;
    case eBuildingType::templeHermes:
        return fHermesSanctuary == eAvailable::available;
    case eBuildingType::templePoseidon:
        return fPoseidonSanctuary == eAvailable::available;
    case eBuildingType::templeZeus:
        return fZeusSanctuary == eAvailable::available;

    case eBuildingType::achillesHall:
        return fAchillesHall == eAvailable::available;
    case eBuildingType::atalantaHall:
        return fAtalantaHall == eAvailable::available;
    case eBuildingType::bellerophonHall:
        return fBellerophonHall == eAvailable::available;
    case eBuildingType::herculesHall:
        return fHerculesHall == eAvailable::available;
    case eBuildingType::jasonHall:
        return fJasonHall == eAvailable::available;
    case eBuildingType::odysseusHall:
        return fOdysseusHall == eAvailable::available;
    case eBuildingType::perseusHall:
        return fPerseusHall == eAvailable::available;
    case eBuildingType::theseusHall:
        return fTheseusHall == eAvailable::available;

    case eBuildingType::commemorative:
        if(id == 0) return fPopulationMonument;
        if(id == 1) return fVictoryMonument;
        if(id == 2) return fColonyMonument;
        if(id == 3) return fAthleteMonument;
        if(id == 4) return fConquestMonument;
        if(id == 5) return fHappinessMonument;
        if(id == 6) return fHeroicMonument;
        if(id == 7) return fDiplomacyMonument;
        if(id == 8) return fScholarMonument;
        break;
    case eBuildingType::godMonument: {
        const auto t = static_cast<eGodType>(id);
        return mGodMonuments.at(t);
    } break;
    default:
        return true;
    }
    return true;
}

void eAvailableBuildings::built(
        const eBuildingType type, const int id) {
    switch(type) {
    case eBuildingType::commemorative: {
        int* c = nullptr;
        if(id == 0) c = &fPopulationMonument;
        else if(id == 1) c = &fVictoryMonument;
        else if(id == 2) c = &fColonyMonument;
        else if(id == 3) c = &fAthleteMonument;
        else if(id == 4) c = &fConquestMonument;
        else if(id == 5) c = &fHappinessMonument;
        else if(id == 6) c = &fHeroicMonument;
        else if(id == 7) c = &fDiplomacyMonument;
        else if(id == 8) c = &fScholarMonument;
        if(!c) return;
        *c = std::max(0, *c - 1);
    } break;
    case eBuildingType::godMonument: {
        const auto t = static_cast<eGodType>(id);
        int& c = mGodMonuments[t];
        c = std::max(0, c - 1);
    } break;

    case eBuildingType::modestPyramid:
        fModestPyramid = eAvailable::built; break;
    case eBuildingType::pyramid:
        fPyramid = eAvailable::built; break;
    case eBuildingType::greatPyramid:
        fGreatPyramid = eAvailable::built; break;
    case eBuildingType::majesticPyramid:
        fMajesticPyramid = eAvailable::built; break;

    case eBuildingType::smallMonumentToTheSky:
        fSmallMonumentToTheSky = eAvailable::built; break;
    case eBuildingType::monumentToTheSky:
        fMonumentToTheSky = eAvailable::built; break;
    case eBuildingType::grandMonumentToTheSky:
        fGrandMonumentTotTheSky = eAvailable::built; break;

    case eBuildingType::minorShrine:
        fMinorShrine = eAvailable::built; break;
    case eBuildingType::shrine:
        fShrine = eAvailable::built; break;
    case eBuildingType::majorShrine:
        fMajorShrine = eAvailable::built; break;

    case eBuildingType::pyramidOfThePantheon:
        fPyramidToThePantheon = eAvailable::built; break;
    case eBuildingType::altarOfOlympus:
        fAltarOfOlympus = eAvailable::built; break;
    case eBuildingType::templeOfOlympus:
        fTempleOfOlympus = eAvailable::built; break;
    case eBuildingType::observatoryKosmika:
        fObservatoryKosmika = eAvailable::built; break;
    case eBuildingType::museumAtlantika:
        fMuseumAtlantika = eAvailable::built; break;

    case eBuildingType::templeAphrodite:
        fAphroditeSanctuary = eAvailable::built; break;
    case eBuildingType::templeApollo:
        fApolloSanctuary = eAvailable::built; break;
    case eBuildingType::templeAres:
        fAresSanctuary = eAvailable::built; break;
    case eBuildingType::templeArtemis:
        fArtemisSanctuary = eAvailable::built; break;
    case eBuildingType::templeAthena:
        fAthenaSanctuary = eAvailable::built; break;
    case eBuildingType::templeAtlas:
        fAtlasSanctuary = eAvailable::built; break;
    case eBuildingType::templeDemeter:
        fDemeterSanctuary = eAvailable::built; break;
    case eBuildingType::templeDionysus:
        fDionysusSanctuary = eAvailable::built; break;
    case eBuildingType::templeHades:
        fHadesSanctuary = eAvailable::built; break;
    case eBuildingType::templeHephaestus:
        fHephaestusSanctuary = eAvailable::built; break;
    case eBuildingType::templeHera:
        fHeraSanctuary = eAvailable::built; break;
    case eBuildingType::templeHermes:
        fHermesSanctuary = eAvailable::built; break;
    case eBuildingType::templePoseidon:
        fPoseidonSanctuary = eAvailable::built; break;
    case eBuildingType::templeZeus:
        fZeusSanctuary = eAvailable::built; break;

    case eBuildingType::achillesHall:
        fAchillesHall = eAvailable::built; break;
    case eBuildingType::atalantaHall:
        fAtalantaHall = eAvailable::built; break;
    case eBuildingType::bellerophonHall:
        fBellerophonHall = eAvailable::built; break;
    case eBuildingType::herculesHall:
        fHerculesHall = eAvailable::built; break;
    case eBuildingType::jasonHall:
        fJasonHall = eAvailable::built; break;
    case eBuildingType::odysseusHall:
        fOdysseusHall = eAvailable::built; break;
    case eBuildingType::perseusHall:
        fPerseusHall = eAvailable::built; break;
    case eBuildingType::theseusHall:
        fTheseusHall = eAvailable::built; break;

    default:
        break;
    }
}

void eAvailableBuildings::destroyed(
        const eBuildingType type, const int id) {
    (void)id;
    const auto a = availablePtr(type);
    if(!a) return;
    if(*a == eAvailable::built) *a = eAvailable::available;
}

void eAvailableBuildings::allow(
        const eBuildingType type, const int id) {
    switch(type) {
    case eBuildingType::commemorative: {
        int* c = nullptr;
        if(id == 0) c = &fPopulationMonument;
        else if(id == 1) c = &fVictoryMonument;
        else if(id == 2) c = &fColonyMonument;
        else if(id == 3) c = &fAthleteMonument;
        else if(id == 4) c = &fConquestMonument;
        else if(id == 5) c = &fHappinessMonument;
        else if(id == 6) c = &fHeroicMonument;
        else if(id == 7) c = &fDiplomacyMonument;
        else if(id == 8) c = &fScholarMonument;
        if(!c) return;
        (*c)++;
    } break;

    case eBuildingType::godMonument: {
        const auto t = static_cast<eGodType>(id);
        int& c = mGodMonuments[t];
        c++;
    } break;

    default: {
        const auto a = availablePtr(type);
        if(a) {
            if(*a == eAvailable::built) return;
            *a = eAvailable::available;
            return;
        }
        const auto aa = allowedPtr(type);
        if(aa) {
            *aa = true;
            return;
        }
    } break;
    }
}

void eAvailableBuildings::disallow(
        const eBuildingType type, const int id) {
    (void)id;
    const auto a = availablePtr(type);
    if(a) {
        *a = eAvailable::notAvailable;
        return;
    }
    const auto aa = allowedPtr(type);
    if(aa) {
        *aa = false;
        return;
    }
}

void eAvailableBuildings::startEpisode(const eAvailableBuildings& o) {
    startEpisode(o, &eAvailableBuildings::fEliteHousing);

    startEpisode(o, &eAvailableBuildings::fWheatFarm);
    startEpisode(o, &eAvailableBuildings::fCarrotsFarm);
    startEpisode(o, &eAvailableBuildings::fOnionFarm);

    startEpisode(o, &eAvailableBuildings::fVine);
    startEpisode(o, &eAvailableBuildings::fOliveTree);
    startEpisode(o, &eAvailableBuildings::fOrangeTree);

    startEpisode(o, &eAvailableBuildings::fDairy);
    startEpisode(o, &eAvailableBuildings::fCardingShed);

    startEpisode(o, &eAvailableBuildings::fFishery);
    startEpisode(o, &eAvailableBuildings::fUrchinQuay);
    startEpisode(o, &eAvailableBuildings::fHuntingLodge);
    startEpisode(o, &eAvailableBuildings::fCorral);

    startEpisode(o, &eAvailableBuildings::fMint);
    startEpisode(o, &eAvailableBuildings::fFoundry);
    startEpisode(o, &eAvailableBuildings::fTimberMill);
    startEpisode(o, &eAvailableBuildings::fMasonryShop);
    startEpisode(o, &eAvailableBuildings::fRefinery);
    startEpisode(o, &eAvailableBuildings::fBlackMarbleWorkshop);

    startEpisode(o, &eAvailableBuildings::fWinery);
    startEpisode(o, &eAvailableBuildings::fOlivePress);
    startEpisode(o, &eAvailableBuildings::fSculptureStudio);

    startEpisode(o, &eAvailableBuildings::fArmory);

    startEpisode(o, &eAvailableBuildings::fHorseRanch);
    startEpisode(o, &eAvailableBuildings::fChariotFactory);

    startEpisode(o, &eAvailableBuildings::fTriremeWharf);

    startEpisode(o, &eAvailableBuildings::fModestPyramid);
    startEpisode(o, &eAvailableBuildings::fPyramid);
    startEpisode(o, &eAvailableBuildings::fGreatPyramid);
    startEpisode(o, &eAvailableBuildings::fMajesticPyramid);

    startEpisode(o, &eAvailableBuildings::fSmallMonumentToTheSky);
    startEpisode(o, &eAvailableBuildings::fMonumentToTheSky);
    startEpisode(o, &eAvailableBuildings::fGrandMonumentTotTheSky);

    startEpisode(o, &eAvailableBuildings::fMinorShrine);
    startEpisode(o, &eAvailableBuildings::fShrine);
    startEpisode(o, &eAvailableBuildings::fMajorShrine);

    startEpisode(o, &eAvailableBuildings::fPyramidToThePantheon);
    startEpisode(o, &eAvailableBuildings::fAltarOfOlympus);
    startEpisode(o, &eAvailableBuildings::fTempleOfOlympus);
    startEpisode(o, &eAvailableBuildings::fObservatoryKosmika);
    startEpisode(o, &eAvailableBuildings::fMuseumAtlantika);

    startEpisode(o, &eAvailableBuildings::fAphroditeSanctuary);
    startEpisode(o, &eAvailableBuildings::fApolloSanctuary);
    startEpisode(o, &eAvailableBuildings::fAresSanctuary);
    startEpisode(o, &eAvailableBuildings::fArtemisSanctuary);
    startEpisode(o, &eAvailableBuildings::fAthenaSanctuary);
    startEpisode(o, &eAvailableBuildings::fAtlasSanctuary);
    startEpisode(o, &eAvailableBuildings::fDemeterSanctuary);
    startEpisode(o, &eAvailableBuildings::fDionysusSanctuary);
    startEpisode(o, &eAvailableBuildings::fHadesSanctuary);
    startEpisode(o, &eAvailableBuildings::fHephaestusSanctuary);
    startEpisode(o, &eAvailableBuildings::fHeraSanctuary);
    startEpisode(o, &eAvailableBuildings::fHermesSanctuary);
    startEpisode(o, &eAvailableBuildings::fPoseidonSanctuary);
    startEpisode(o, &eAvailableBuildings::fZeusSanctuary);
}

void eAvailableBuildings::startEpisode(const eAvailableBuildings& o,
                               bool eAvailableBuildings::*ptr) {
    this->*ptr = o.*ptr;
}

void eAvailableBuildings::startEpisode(const eAvailableBuildings& o,
                                       eAvailable eAvailableBuildings::*ptr) {
    if(this->*ptr == eAvailable::built) return;
    this->*ptr = o.*ptr;
}

bool* eAvailableBuildings::allowedPtr(const eBuildingType type) {
    switch(type) {
    case eBuildingType::eliteHousing:
        return &fEliteHousing;

    case eBuildingType::wheatFarm:
        return &fWheatFarm;
    case eBuildingType::carrotsFarm:
        return &fCarrotsFarm;
    case eBuildingType::onionsFarm:
        return &fOnionFarm;

    case eBuildingType::vine:
        return &fVine;
    case eBuildingType::oliveTree:
        return &fOliveTree;
    case eBuildingType::orangeTree:
        return &fOrangeTree;

    case eBuildingType::dairy:
        return &fDairy;
    case eBuildingType::cardingShed:
        return &fCardingShed;

    case eBuildingType::fishery:
        return &fFishery;
    case eBuildingType::urchinQuay:
        return &fUrchinQuay;
    case eBuildingType::huntingLodge:
        return &fHuntingLodge;
    case eBuildingType::corral:
        return &fCorral;

    case eBuildingType::mint:
        return &fMint;
    case eBuildingType::foundry:
        return &fFoundry;
    case eBuildingType::timberMill:
        return &fTimberMill;
    case eBuildingType::masonryShop:
        return &fMasonryShop;
    case eBuildingType::refinery:
        return &fRefinery;
    case eBuildingType::blackMarbleWorkshop:
        return &fBlackMarbleWorkshop;

    case eBuildingType::winery:
        return &fWinery;
    case eBuildingType::olivePress:
        return &fOlivePress;
    case eBuildingType::sculptureStudio:
        return &fSculptureStudio;

    case eBuildingType::armory:
        return &fArmory;

    case eBuildingType::horseRanch:
        return &fHorseRanch;
    case eBuildingType::chariotFactory:
        return &fChariotFactory;

    case eBuildingType::triremeWharf:
        return &fTriremeWharf;

    default:
        return nullptr;
    }
    return nullptr;
}

eAvailable* eAvailableBuildings::availablePtr(const eBuildingType type) {
    eAvailable* a = nullptr;

    switch(type) {
    case eBuildingType::modestPyramid:
        a = &fModestPyramid; break;
    case eBuildingType::pyramid:
        a = &fPyramid; break;
    case eBuildingType::greatPyramid:
        a = &fGreatPyramid; break;
    case eBuildingType::majesticPyramid:
        a = &fMajesticPyramid; break;

    case eBuildingType::smallMonumentToTheSky:
        a = &fSmallMonumentToTheSky; break;
    case eBuildingType::monumentToTheSky:
        a = &fMonumentToTheSky; break;
    case eBuildingType::grandMonumentToTheSky:
        a = &fGrandMonumentTotTheSky; break;

    case eBuildingType::minorShrine:
        a = &fMinorShrine; break;
    case eBuildingType::shrine:
        a = &fShrine; break;
    case eBuildingType::majorShrine:
        a = &fMajorShrine; break;

    case eBuildingType::pyramidOfThePantheon:
        a = &fPyramidToThePantheon; break;
    case eBuildingType::altarOfOlympus:
        a = &fAltarOfOlympus; break;
    case eBuildingType::templeOfOlympus:
        a = &fTempleOfOlympus; break;
    case eBuildingType::observatoryKosmika:
        a = &fObservatoryKosmika; break;
    case eBuildingType::museumAtlantika:
        a = &fMuseumAtlantika; break;

    case eBuildingType::templeAphrodite:
        a = &fAphroditeSanctuary; break;
    case eBuildingType::templeApollo:
        a = &fApolloSanctuary; break;
    case eBuildingType::templeAres:
        a = &fAresSanctuary; break;
    case eBuildingType::templeArtemis:
        a = &fArtemisSanctuary; break;
    case eBuildingType::templeAthena:
        a = &fAthenaSanctuary; break;
    case eBuildingType::templeAtlas:
        a = &fAtlasSanctuary; break;
    case eBuildingType::templeDemeter:
        a = &fDemeterSanctuary; break;
    case eBuildingType::templeDionysus:
        a = &fDionysusSanctuary; break;
    case eBuildingType::templeHades:
        a = &fHadesSanctuary; break;
    case eBuildingType::templeHephaestus:
        a = &fHephaestusSanctuary; break;
    case eBuildingType::templeHera:
        a = &fHeraSanctuary; break;
    case eBuildingType::templeHermes:
        a = &fHermesSanctuary; break;
    case eBuildingType::templePoseidon:
        a = &fPoseidonSanctuary; break;
    case eBuildingType::templeZeus:
        a = &fZeusSanctuary; break;

    case eBuildingType::achillesHall:
        a = &fAchillesHall; break;
    case eBuildingType::atalantaHall:
        a = &fAtalantaHall; break;
    case eBuildingType::bellerophonHall:
        a = &fBellerophonHall; break;
    case eBuildingType::herculesHall:
        a = &fHerculesHall; break;
    case eBuildingType::jasonHall:
        a = &fJasonHall; break;
    case eBuildingType::odysseusHall:
        a = &fOdysseusHall; break;
    case eBuildingType::perseusHall:
        a = &fPerseusHall; break;
    case eBuildingType::theseusHall:
        a = &fTheseusHall; break;

    default:
        break;
    }
    return a;
}
