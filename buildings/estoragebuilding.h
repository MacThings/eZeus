#ifndef ESTORAGEBUILDING_H
#define ESTORAGEBUILDING_H

#include "eemployingbuilding.h"

class eStorageBuilding : public eEmployingBuilding {
public:
    eStorageBuilding(eGameBoard& board,
                     const eBuildingType type,
                     const int sw, const int sh,
                     const int maxEmployees,
                     const eResourceType canAccept,
                     const eCityId cid,
                     const int spaceCount = 8);
    ~eStorageBuilding();

    void timeChanged(const int by) override;

    int addNotAccept(const eResourceType type, const int count);

    int add(const eResourceType type, const int count) override;
    int take(const eResourceType type, const int count) override;

    int count(const eResourceType type) const override;
    int spaceLeft(const eResourceType type) const override;

    int spaceLeftDontAccept(const eResourceType type) const;

    std::vector<eCartTask> cartTasks() const override;

    static int sCount(const eResourceType type,
                      const int resourceCount[15],
                      const eResourceType resourceType[15],
                      const int spaceCount);
    static int sSpaceLeftDontAccept(const eResourceType type,
                                    const int resourceCount[15],
                                    const eResourceType resourceType[15],
                                    const std::map<eResourceType, int>& maxCounts,
                                    const int spaceCount);
    static int sSpaceLeft(const eResourceType type,
                          const int resourceCount[15],
                          const eResourceType resourceType[15],
                          const eResourceType accepts,
                          const std::map<eResourceType, int>& maxCounts,
                          const int spaceCount);

    int resourceCount(const int id) const { return mResourceCount[id]; }
    eResourceType resourceType(const int id) const { return mResource[id]; }

    const int* resourceCounts() const { return &mResourceCount[0]; }
    const eResourceType* resourceTypes() const { return &mResource[0]; }
    const std::map<eResourceType, int>& maxCount() const { return mMaxCount; };
    void setMaxCount(const std::map<eResourceType, int>& m);

    eResourceType accepts() const { return mAccept; }
    eResourceType get() const { return mGet; }
    eResourceType empties() const { return mEmpty; }
    eResourceType canAccept() const { return mCanAccept; }

    bool accepts(const eResourceType res) const
    { return static_cast<bool>(mAccept & res); }
    bool get(const eResourceType res) const
    { return static_cast<bool>(mGet & res); }
    bool empties(const eResourceType res) const
    { return static_cast<bool>(mEmpty & res); }
    bool canAccept(const eResourceType res) const
    { return static_cast<bool>(mCanAccept & res); }

    void setOrders(const eResourceType get,
                   const eResourceType empty,
                   const eResourceType accept);

    void getOrders(eResourceType& get,
                   eResourceType& empty,
                   eResourceType& accept) const;

    int spaceCount() const { return mSpaceCount; }

    void read(eReadStream& src) override;
    void write(eWriteStream& dst) const override;
private:
    const eResourceType mCanAccept;

    const int mSpaceCount;

    eResourceType mGet = eResourceType::none;
    eResourceType mEmpty = eResourceType::none;
    eResourceType mAccept = mCanAccept; // includes get

    int mResourceCount[15] = {0};
    eResourceType mResource[15] = {eResourceType::none,
                                   eResourceType::none,
                                   eResourceType::none,
                                   eResourceType::none,
                                   eResourceType::none,
                                   eResourceType::none,
                                   eResourceType::none,
                                   eResourceType::none,
                                   eResourceType::none,
                                   eResourceType::none,
                                   eResourceType::none,
                                   eResourceType::none,
                                   eResourceType::none,
                                   eResourceType::none,
                                   eResourceType::none};

    std::map<eResourceType, int> mMaxCount;

    stdptr<eCartTransporter> mCart1;
    stdptr<eCartTransporter> mCart2;
};

#endif // ESTORAGEBUILDING_H
