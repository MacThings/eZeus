#ifndef EGAMEWIDGET_H
#define EGAMEWIDGET_H

#include "ewidget.h"
#include "eframedlabel.h"
#include "etexturecollection.h"

#include "engine/etile.h"
#include "engine/egameboard.h"
#include "engine/ethreadpool.h"

#include "textures/eterraintextures.h"
#include "textures/egodtextures.h"
#include "textures/ebuildingtextures.h"
#include "textures/echaractertextures.h"

#include "buildings/epatrolbuilding.h"

#include "infowidgets/egyminfowidget.h"

#include "widgets/egamemenu.h"
#include "egamemainmenu.h"
#include "etopbarwidget.h"

#include "eviewmode.h"
#include "emessage.h"

class eTerrainEditMenu;
class eDomesticatedAnimal;
struct eSanctBlueprint;

using eBuildingCreator = std::function<stdsptr<eBuilding>()>;
using eRendererCreator = std::function<stdsptr<eBuildingRenderer>()>;

class eGameWidget : public eWidget {
public:
    eGameWidget(eMainWindow* const window);

    void initialize(const int w, const int h);

    void pixToId(const int pixX, const int pixY,
                 int& idX, int& idY) const;

    void setViewMode(const eViewMode m);
    eViewMode viewMode() const { return mViewMode; }

    void viewFraction(const double fx, const double fy);
    void viewTile(eTile* const tile);
    bool tileVisible(eTile* const tile) const;
protected:
    void paintEvent(ePainter& p);

    bool keyPressEvent(const eKeyPressEvent& e);
    bool mousePressEvent(const eMouseEvent& e);
    bool mouseMoveEvent(const eMouseEvent& e);
    bool mouseReleaseEvent(const eMouseEvent& e);
    bool mouseWheelEvent(const eMouseWheelEvent& e);
private:
    void buildMouseRelease();

    static void sClearScrub(const int x, const int y,
                            const int sw, const int sh,
                            eGameBoard& board);
    static void sClearForest(const int x, const int y,
                             const int sw, const int sh,
                             eGameBoard& board);
    static bool sTileFertile(eTile* const tile);

    using eTileAction = std::function<void(eTile* const)>;
    void iterateOverVisibleTiles(const eTileAction& a);

    void setTileSize(const eTileSize size);

    void actionOnSelectedTiles(const eTileAction& apply);

    using eSpecialRequirement = std::function<bool(eTile*)>;
    bool canBuildBase(const int minX, const int maxX,
                      const int minY, const int maxY,
                      const eSpecialRequirement& specReq = {});
    bool canBuild(const int tx, const int ty,
                  const int sw, const int sh,
                  const eSpecialRequirement& specReq = {});
    bool build(const int tx, const int ty,
               const int sw, const int sh,
               const eBuildingCreator& bc,
               const eSpecialRequirement& specReq = {},
               const eRendererCreator& rc = {});
    using eDA = eDomesticatedAnimal;
    using eAnimalCreator = std::function<stdsptr<eDA>(eGameBoard&)>;
    void buildAnimal(eTile* const tile,
                     const eBuildingType type,
                     const eAnimalCreator& creator);

    const eSanctBlueprint* sanctuaryBlueprint(
            const eBuildingType type, const bool rotate);

    bool erase(eTile* const tile);
    std::vector<ePatrolGuide>::iterator
        findGuide(const int tx, const int ty);

    void handleEvent(const eEvent e, eTile* const tile);

    void mapDimensions(int& mdx, int& mdy) const;
    void viewBoxSize(double& fx, double& fy) const;
    void viewedFraction(double& fx, double& fy) const;
    void tileViewFraction(eTile* const tile,
                          double& xf, double& yf) const;

    void updateMinimap();

    int waterParkId() const;

    void updateAppealMap();

    void showMessage(eTile* const tile, const eMessage& msg);
    void showMessage(eTile* const tile, const eMessageType& msg);

    bool roadPath(std::vector<eOrientation>& path);

    bool mRotate = false;

    const int sSpeeds[5] = {2, 10, 25, 50, 100};

    bool mPaused = false;
    int mFrame{0};
    int mTime{0};
    int mSpeedId = 1;
    int mSpeed = sSpeeds[mSpeedId];

    int mWheel = 0;

    int mDX = 0;
    int mDY = 0;

    int mHoverX = -1;
    int mHoverY = -1;
    int mPressedX = -1;
    int mPressedY = -1;
    int mLastX = -1;
    int mLastY = -1;

    eViewMode mViewMode = eViewMode::defaultView;

    eTileSize mTileSize = eTileSize::s30;
    int mTileW = 58;
    int mTileH = 30;

    eThreadPool mThreadPool;

    int mUpdateRect = 0;
    std::vector<SDL_Rect> mUpdateRects;
    eGameBoard mBoard;

    bool mDrawElevation = true;
    ePatrolBuilding* mPatrolBuilding = nullptr;

    eFramedLabel* mPausedLabel = nullptr;
    eGameMainMenu* mMenu = nullptr;

    eTopBarWidget* mTopBar = nullptr;
    eInfoWidget* mInfoWidget = nullptr;
    eTerrainEditMenu* mTem = nullptr;
    eGameMenu* mGm = nullptr;
};

#endif // EGAMEWIDGET_H
