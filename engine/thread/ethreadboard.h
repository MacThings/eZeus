#ifndef ETHREADBOARD_H
#define ETHREADBOARD_H

#include "ethreadtile.h"

class eThreadBoard {
public:
    void initialize(const int w, const int h);
    void clear();

    int width() const { return mWidth; }
    int height() const { return mHeight; }

    eThreadTile* tile(const int x, const int y);
    eThreadTile* dtile(const int x, const int y);

    int state() const { return mState; }
    void setState(const int s) { mState = s; }

    int allBuildingsState() const { return mAllBuildingsState; }
    void setAllBuildingsState(const int s) { mAllBuildingsState = s; }

    int resourcesInBuildingsState() const { return mResourcesInBuildingsState; }
    void setResourcesInBuildingsState(const int s) { mResourcesInBuildingsState = s; }

    int houseVacanciesState() const { return mHouseVacanciesState; }
    void setHouseVacanciesState(const int s) { mHouseVacanciesState = s; }

    int terrainState() const { return mTerrainState; }
    void setTerrainState(const int s) { mTerrainState = s; }

    int forestsState() const { return mForestsState; }
    void setForestsState(const int s) { mForestsState = s; }

    int resourcesState() const { return mResourcesState; }
    void setResourcesState(const int s) { mResourcesState = s; }

    int sanctuariesState() const { return mSanctuariesState; }
    void setSanctuariesState(const int s) { mSanctuariesState = s; }

    int treesAndVinesState() const { return mTreesAndVinesState; }
    void setTreesAndVinesState(const int s) { mTreesAndVinesState = s; }

    int domesticatedAnimalsState() const { return mDomesticatedAnimalsState; }
    void setDomesticatedAnimalsState(const int s) { mDomesticatedAnimalsState = s; }
private:
    void updateNeighbours();

    int mState = 0;
    int mAllBuildingsState = 0;
    int mResourcesInBuildingsState = 0;
    int mHouseVacanciesState = 0;
    int mTerrainState = 0;
    int mForestsState = 0;
    int mSanctuariesState = 0;
    int mTreesAndVinesState = 0;
    int mDomesticatedAnimalsState = 0;
    int mResourcesState = 0;

    int mWidth = 0;
    int mHeight = 0;
    std::vector<std::vector<eThreadTile>> mTiles;
};

#endif // ETHREADBOARD_H
