﻿#ifndef EGAMEMENU_H
#define EGAMEMENU_H

#include "egamemenubase.h"

class eCheckableButton;
class eTextureCollection;
class eInterfaceTextures;
class eButton;
class eGameBoard;
class ePopulationDataWidget;
class eEmploymentDataWidget;

struct eSubButtonData;

enum class eBuildingMode {
    none,
    erase,

    road,

    commonHousing,
    eliteHousing,

    wheatFarm,
    carrotFarm,
    onionFarm,

    vine,
    oliveTree,
    growersLodge,

    dairy,
    goat,
    cardingShed,
    sheep,

    fishery,
    urchinQuay,
    huntingLodge,

    mint,
    foundry,
    timberMill,
    masonryShop,

    winery,
    olivePress,
    sculptureStudio,
    artisansGuild,
    armory,

    podium,
    college,

    theater,
    dramaSchool,

    gymnasium,
    stadium,

    granary,
    warehouse,

    maintenanceOffice,
    fountain,
    hospital,
    watchpost,

    taxOffice,
    palace
};

class eGameMenu : public eGameMenuBase {
public:
    using eGameMenuBase::eGameMenuBase;
    void initialize();

    eBuildingMode mode() const { return mMode; }
    void clearMode() { mMode = eBuildingMode::none; }

    void setBoard(eGameBoard* const b);
private:
    void setMode(const eBuildingMode mode);
    eButton* createSubButton(const eTextureCollection& texs,
                             eWidget* const buttons);
    using eButtonsDataVec = std::vector<eSubButtonData>;
    eWidget* createSubButtons(const int resoltuionMult,
                              const eButtonsDataVec& buttons);

    eGameBoard* mBoard{nullptr};

    ePopulationDataWidget* mPopDataW = nullptr;
    eEmploymentDataWidget* mEmplDataW = nullptr;

    eBuildingMode mMode{eBuildingMode::road};

    std::vector<eWidget*> mWidgets;
};

#endif // EGAMEMENU_H
