#ifndef EBUILDINGTEXTURES_H
#define EBUILDINGTEXTURES_H

#include "etexturecollection.h"

class eBuildingTextures {
public:
    eBuildingTextures(const int tileW, const int tileH,
                     SDL_Renderer* const renderer);

    void load();

    const int fTileW;
    const int fTileH;
    SDL_Renderer* const fRenderer;

    std::shared_ptr<eTexture> fHouseSpace;
    std::vector<eTextureCollection> fCommonHouse;


    std::shared_ptr<eTexture> fGymnasium;
    eTextureCollection fGymnasiumOverlay;

    std::shared_ptr<eTexture> fCollege;
    eTextureCollection fCollegeOverlay;

    std::shared_ptr<eTexture> fDramaSchool;
    eTextureCollection fDramaSchoolOverlay;

    std::shared_ptr<eTexture> fPodium;
    eTextureCollection fPodiumOverlay;

    std::shared_ptr<eTexture> fTheater;
    eTextureCollection fTheaterOverlay;

    std::shared_ptr<eTexture> fStadium1H;
    std::shared_ptr<eTexture> fStadium2H;

    std::shared_ptr<eTexture> fStadium1W;
    std::shared_ptr<eTexture> fStadium2W;

    eTextureCollection fStadiumOverlay1;
    eTextureCollection fStadiumOverlay2;
    eTextureCollection fStadiumOverlay3;
    eTextureCollection fStadiumOverlay4W;
    eTextureCollection fStadiumOverlay4H;
    eTextureCollection fStadiumOverlay5W;
    eTextureCollection fStadiumOverlay5H;

    eTextureCollection fStadiumAudiance1W;
    eTextureCollection fStadiumAudiance2W;

    eTextureCollection fStadiumAudiance1H;
    eTextureCollection fStadiumAudiance2H;

    std::shared_ptr<eTexture> fPalace1H;
    std::shared_ptr<eTexture> fPalace2H;

    std::shared_ptr<eTexture> fPalace1W;
    std::shared_ptr<eTexture> fPalace2W;

    eTextureCollection fPalaceTiles;

    std::shared_ptr<eTexture> fFountain;
    eTextureCollection fFountainOverlay;

    std::shared_ptr<eTexture> fHospital;
    eTextureCollection fHospitalOverlay;


    eTextureCollection fOliveTree;
    eTextureCollection fVine;

    std::shared_ptr<eTexture> fPlantation;
    eTextureCollection fWheat;
    eTextureCollection fCarrots;
    eTextureCollection fOnions;

    std::shared_ptr<eTexture> fHuntingLodge;
    eTextureCollection fHuntingLodgeOverlay;

    eTextureCollection fFishery;
    eTextureCollection fUrchinQuay;

    std::shared_ptr<eTexture> fCardingShed;
    eTextureCollection fCardingShedOverlay;

    std::shared_ptr<eTexture> fDairy;
    eTextureCollection fDairyOverlay;

    std::shared_ptr<eTexture> fGrowersLodge;
    eTextureCollection fGrowersLodgeOverlay;

    std::shared_ptr<eTexture> fTimberMill;
    eTextureCollection fTimberMillOverlay;

    std::shared_ptr<eTexture> fMasonryShop;
    eTextureCollection fMasonryShopStones;

    std::shared_ptr<eTexture> fMint;
    eTextureCollection fMintOverlay;

    std::shared_ptr<eTexture> fFoundry;
    eTextureCollection fFoundryOverlay;

    std::shared_ptr<eTexture> fArtisansGuild;
    eTextureCollection fArtisansGuildOverlay;

    std::shared_ptr<eTexture> fOlivePress;
    eTextureCollection fOlivePressOverlay;

    std::shared_ptr<eTexture> fWinery;
    eTextureCollection fWineryOverlay;

    std::shared_ptr<eTexture> fSculptureStudio;
    eTextureCollection fSculptureStudioOverlay;


    eTextureCollection fTriremeWharf;

    std::shared_ptr<eTexture> fHorseRanch;
    eTextureCollection fHorseRanchOverlay;
    std::shared_ptr<eTexture> fHorseRanchEnclosure;

    std::shared_ptr<eTexture> fArmory;
    eTextureCollection fArmoryOverlay;

    std::shared_ptr<eTexture> fMaintenanceOffice;
    eTextureCollection fMaintenanceOfficeOverlay;

    std::shared_ptr<eTexture> fTaxOffice;
    eTextureCollection fTaxOfficeOverlay;

    std::shared_ptr<eTexture> fWatchPost;
    eTextureCollection fWatchPostOverlay;

    eTextureCollection fBeachRoad;
    eTextureCollection fRoad;

    eTextureCollection fPatrolGuides;

    eTextureCollection fWaitingMeat;
    eTextureCollection fWaitingCheese;
    eTextureCollection fWaitingWheat;
    eTextureCollection fWaitingWood;
    eTextureCollection fWaitingBronze;
    eTextureCollection fWaitingGrapes;
    eTextureCollection fWaitingOlives;
    eTextureCollection fWaitingArmor;

//    eTextureCollection fAgoraRoad;
//    eTextureCollection fAgora;

    std::shared_ptr<eTexture> fFoodVendor;
    std::shared_ptr<eTexture> fFoodVendorOverlay;

    std::shared_ptr<eTexture> fFleeceVendor;
    std::shared_ptr<eTexture> fFleeceVendorOverlay;

    std::shared_ptr<eTexture> fOilVendor;
    std::shared_ptr<eTexture> fOilVendorOverlay;

    std::shared_ptr<eTexture> fArmsVendor;
    std::shared_ptr<eTexture> fArmsVendorOverlay;

    std::shared_ptr<eTexture> fWineVendor;
    std::shared_ptr<eTexture> fWineVendorOverlay;

    std::shared_ptr<eTexture> fHorseTrainer;
    std::shared_ptr<eTexture> fHorseTrainerOverlay;

    std::shared_ptr<eTexture> fWarehouse;
    eTextureCollection fWarehouseOverlay;

    std::shared_ptr<eTexture> fWarehouseEmpty;
    eTextureCollection fWarehouseUrchin;
    eTextureCollection fWarehouseFish;
    eTextureCollection fWarehouseMeat;
    eTextureCollection fWarehouseCheese;
    eTextureCollection fWarehouseCarrots;
    eTextureCollection fWarehouseOnions;
    eTextureCollection fWarehouseWheat;

    eTextureCollection fWarehouseWood;
    eTextureCollection fWarehouseBronze;
    eTextureCollection fWarehouseMarble;
    eTextureCollection fWarehouseGrapes;
    eTextureCollection fWarehouseOlives;
    eTextureCollection fWarehouseFleece;
    eTextureCollection fWarehouseArmor;
    std::shared_ptr<eTexture> fWarehouseSculpture;
    eTextureCollection fWarehouseOliveOil;
    eTextureCollection fWarehouseWine;

    std::shared_ptr<eTexture> fGranary;
    eTextureCollection fGranaryOverlay;

    std::shared_ptr<eTexture> fGranaryUrchin;
    std::shared_ptr<eTexture> fGranaryFish;
    std::shared_ptr<eTexture> fGranaryMeat;
    std::shared_ptr<eTexture> fGranaryCheese;
    std::shared_ptr<eTexture> fGranaryCarrots;
    std::shared_ptr<eTexture> fGranaryOnions;
    std::shared_ptr<eTexture> fGranaryWheat;

    eTextureCollection fPark;
    eTextureCollection fLargePark;
    eTextureCollection fHugePark;

    std::shared_ptr<eTexture> fBench;
    std::shared_ptr<eTexture> fFlowerGarden;
    std::shared_ptr<eTexture> fGazebo;
    std::shared_ptr<eTexture> fHedgeMaze;
    std::shared_ptr<eTexture> fFishPond;

    std::shared_ptr<eTexture> fDoricColumn;
    std::shared_ptr<eTexture> fIonicColumn;
    std::shared_ptr<eTexture> fCorinthianColumn;

    std::shared_ptr<eTexture> fColumnConnectionH;
    std::shared_ptr<eTexture> fColumnConnectionW;

    eTextureCollection fClouds;
};

#endif // EBUILDINGTEXTURES_H
