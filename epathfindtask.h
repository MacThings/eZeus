#ifndef EPATHFINDTASK_H
#define EPATHFINDTASK_H

#include "engine/etask.h"
#include "engine/thread/ethreadtile.h"
#include "engine/thread/ethreaddata.h"

class ePathFindTask : public eTask {
public:
    using ePath = std::vector<eOrientation>;
    using eTileGetter = std::function<eThreadTile*(eThreadBoard&)>;
    using eTileChecker = std::function<bool(eThreadTile*)>;
    using eFinishFunc = std::function<void(const ePath&)>;
    using eFailFunc = std::function<void()>;
    using eTileDistance = std::function<int(eTileBase* const)>;
    ePathFindTask(const eCityId cid,
                  const SDL_Rect& tileBRect,
                  const eTileGetter& startTile,
                  const stdsptr<eWalkableObject>& tileWalkable,
                  const eTileChecker& endTileFunc,
                  const eFinishFunc& finishFunc,
                  const eFailFunc& failFunc,
                  const bool onlyDiagonal,
                  const int range,
                  const eTileDistance& distance = nullptr,
                  const eTileGetter& endTile = nullptr,
                  const bool findAll = false);
protected:
    void run(eThreadBoard& data);
    void finish();
private:
    const SDL_Rect mTileBRect;
    const eTileGetter mStartTile;
    const eTileGetter mEndTile;
    const stdsptr<eWalkableObject> mTileWalkable;
    const eTileChecker mEndTileFunc;
    const eFinishFunc mFinish;
    const eFailFunc mFailFunc;
    const bool mOnlyDiagonal;
    const int mRange;
    const eTileDistance mDistance;
    const bool mFindAll;

    bool mR{false};
    ePath mPath;
};

#endif // EPATHFINDTASK_H
