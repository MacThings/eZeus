#ifndef EHEATMAPTASK_H
#define EHEATMAPTASK_H

#include "../etask.h"

#include <functional>

#include "eheatmap.h"

enum class eBuildingType;

class eHeatMapTask : public eTask {
public:
    using eHeatGetter = std::function<eHeat(eBuildingType)>;
    using eFunc = std::function<void(eHeatMap&)>;
    eHeatMapTask(const eCityId cid,
                 const eHeatGetter& heatGetter,
                 const eFunc& finish);

    void run(eThreadBoard& board);
    void finish();
private:
    const eHeatGetter mHeatGetter;
    const eFunc mFinish;
    eHeatMap mMap;
};

#endif // EHEATMAPTASK_H
