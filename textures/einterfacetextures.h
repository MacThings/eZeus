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

    eTexture fGameMenuBackground;

    // normal, hover, selected, disabled
    eTextureCollection fShowInfo;
    eTextureCollection fShowMap;

    // normal, hover, selected, disabled
    eTextureCollection fPopulation;
    eTextureCollection fHusbandry;
    eTextureCollection fIndustry;
    eTextureCollection fDistribution;
    eTextureCollection fHygieneSafety;
    eTextureCollection fAdministration;
    eTextureCollection fCulture;
    eTextureCollection fMythology;
    eTextureCollection fMilitary;
    eTextureCollection fAesthetics;
    eTextureCollection fOverview;

    // normal, hover, pressed
    eTextureCollection fCommonHousing;
    eTextureCollection fEliteHousing;

    eTextureCollection fFoodFarming;
    eTextureCollection fOtherFarming; // e.g. wine
    eTextureCollection fAnimalFarming;
    eTextureCollection fAnimalHunting;

    eTextureCollection fResources; // e.g. wood, marble
    eTextureCollection fProcessing; // e.g. olives
    eTextureCollection fBuildersGuild;

    eTextureCollection fGranary;
    eTextureCollection fWarehouse;
    eTextureCollection fAgoras;
    eTextureCollection fTrade;

    eTextureCollection fFireFighter;
    eTextureCollection fFountain;
    eTextureCollection fPolice;
    eTextureCollection fHospital;

    eTextureCollection fPalace;
    eTextureCollection fTaxCollector;
    eTextureCollection fBridge;

    eTextureCollection fPhilosophy;
    eTextureCollection fGymnasium;
    eTextureCollection fDrama;
    eTextureCollection fStadium;

    eTextureCollection fTemples;
    eTextureCollection fHeroShrines;

    eTextureCollection fFortifications;
    eTextureCollection fMilitaryProduction;

    eTextureCollection fBeautification;
    eTextureCollection fRecreation;
    eTextureCollection fMonuments;

    // normal, hover, pressed, disabled
    eTextureCollection fBuildRoad;
    eTextureCollection fRoadBlock;
    eTextureCollection fClear;
    eTextureCollection fUndo;

    eTextureCollection fMessages;

    eTextureCollection fGoals;
    eTextureCollection fRotation;
    eTextureCollection fWorld;

    // normal, hoer, pressed
    eTextureCollection fFireAlert;
    eTextureCollection fGroundFissureAlert;
    eTextureCollection fFloodAlert;
    eTextureCollection fAvalancheAltert;
    eTextureCollection fLavaAltert;
    eTextureCollection fBuildingCollapseAltert;
    eTextureCollection fRoadBlockAltert;
    eTextureCollection fVirusAlert;
    eTextureCollection fIllnessAlert;
    eTextureCollection fInvasionAlert;
    eTextureCollection fMonsterAltert;
    eTextureCollection fGodAttackAlert;
    eTextureCollection fGodVisitAlert;
    eTextureCollection fHeroArrivalAlert;
    eTextureCollection fArmyComebackAlert;

    // topLeft, top, topRight, left, center, right, bottomLeft, bottom, bottomRight,
    std::vector<eTextureCollection> fComboBox;
};

#endif // EINTERFACETEXTURES_H