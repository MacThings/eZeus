#ifndef EINTERFACETEXTURES_H
#define EINTERFACETEXTURES_H

#include "etexturecollection.h"

class eInterfaceTextures {
public:
    eInterfaceTextures(const int tileW, const int tileH,
                       SDL_Renderer* const renderer);

    void load();

    const int fTileW;
    const int fTileH;
    SDL_Renderer* const fRenderer;


    // normal, hover, selected, disabled
    eTextureCollection fPopulationTexture;
    eTextureCollection fHusbandryTexture;
    eTextureCollection fIndustryTexture;
    eTextureCollection fDistributionTexture;
    eTextureCollection fHygieneSafetyTexture;
    eTextureCollection fAdministrationTexture;
    eTextureCollection fCultureTexture;
    eTextureCollection fMythologyTexture;
    eTextureCollection fMilitaryTexture;
    eTextureCollection fAestheticsTexture;
    eTextureCollection fOverviewTexture;

    // normal, hover, selected
    eTextureCollection fCommonHousingTexture;
    eTextureCollection fEliteHousingTexture;

    eTextureCollection fFoodFarmingTexture;
    eTextureCollection fOtherFarmingTexture; // e.g. wine
    eTextureCollection fAnimalFarmingTexture;
    eTextureCollection fAnimalHuntingTexture;

    eTextureCollection fResourcesTexture; // e.g. wood, marble
    eTextureCollection fProcessingTexture; // e.g. olives
    eTextureCollection fBuildersGuildTexture;

    eTextureCollection fGranaryTexture;
    eTextureCollection fWarehouseTexture;
    eTextureCollection fAgorasTexture;
    eTextureCollection fTradeTexture;

    eTextureCollection fFireFighterTexture;
    eTextureCollection fFountainTexture;
    eTextureCollection fPoliceTexture;
    eTextureCollection fHospitalTexture;

    eTextureCollection fPalaceTexture;
    eTextureCollection fTaxCollectorTexture;
    eTextureCollection fBridgeTexture;

    eTextureCollection fPhilosophyTexture;
    eTextureCollection fGymnasiumTexture;
    eTextureCollection fDramaTexture;
    eTextureCollection fStadiumTexture;

    eTextureCollection fTemplesTexture;
    eTextureCollection fHeroShrinesTexture;

    eTextureCollection fFortificationsTexture;
    eTextureCollection fMilitaryProductionTexture;

    eTextureCollection fBeautificationTexture;
    eTextureCollection fRecreationTexture;
    eTextureCollection fMonumentTexture;

    // normal, hover, selected, disabled
    eTextureCollection fBuildRoadTexture;
    eTextureCollection fClearTexture;
};

#endif // EINTERFACETEXTURES_H
