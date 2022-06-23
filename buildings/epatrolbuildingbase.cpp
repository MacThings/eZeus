#include "epatrolbuildingbase.h"

ePatrolBuildingBase::ePatrolBuildingBase(
        eGameBoard& board,
        const eCharGenerator& charGen,
        const eActGenerator& actGen,
        const eBuildingType type,
        const int sw, const int sh,
        const int maxEmployees) :
    eEmployingBuilding(board, type, sw, sh, maxEmployees),
    mCharGenerator(charGen),
    mActGenerator(actGen) {

}

stdsptr<eCharacterAction>
ePatrolBuildingBase::sDefaultActGenerator(
           eCharacter* const c,
           ePatrolBuildingBase* const b,
           const std::vector<ePatrolGuide>& guides,
           const eAction& failAction,
           const eAction& finishAction) {
    return e::make_shared<ePatrolAction>(
                c, b, guides, failAction, finishAction);
}

ePatrolBuildingBase::ePatrolBuildingBase(
        eGameBoard& board,
        const eCharGenerator& charGen,
        const eBuildingType type,
        const int sw, const int sh,
        const int maxEmployees) :
    ePatrolBuildingBase(board, charGen, sDefaultActGenerator,
                        type, sw, sh, maxEmployees) {}

ePatrolBuildingBase::~ePatrolBuildingBase() {
    if(mChar) mChar->kill();
}

void ePatrolBuildingBase::timeChanged(const int by) {
    const int t = time();
    if(mSpawnPatrolers && enabled()) {
        if(!mChar && t >= mSpawnTime) {
            spawn();
            mSpawnTime = t + mWaitTime;
        }
    }
    eEmployingBuilding::timeChanged(by);
}

void ePatrolBuildingBase::setPatrolGuides(const ePatrolGuides &g) {
    mPatrolGuides = g;
}

bool ePatrolBuildingBase::spawn() {
    mChar = mCharGenerator();
    if(!mChar) return false;
    if(mPatrolGuides.empty()) {
        eTile* t = nullptr;
        const auto bt = type();
        if(bt == eBuildingType::commonAgora ||
           bt == eBuildingType::grandAgora) {
            t = centerTile();
        } else {
            auto dirs = gExtractDirections(eMoveDirection::allDirections);
            std::random_shuffle(dirs.begin(), dirs.end());
            for(const auto dir : dirs) {
                t = road(dir);
                if(t) break;
            }
        }
        if(!t) return false;
        mChar->changeTile(t);
    } else {
        mChar->changeTile(centerTile());
    }
    const eStdPointer<ePatrolBuildingBase> tptr(this);
    const eStdPointer<eCharacter> cptr(mChar);
    const auto finishAct = [tptr, this, cptr]() {
        if(cptr) cptr->kill();
        if(tptr) {
            mSpawnTime = time() + mWaitTime;
            mChar.reset();
        }
    };
    const auto a = mActGenerator(mChar.get(), this,
                                 mPatrolGuides,
                                 finishAct, finishAct);
    mChar->setAction(a);
    return true;
}

void ePatrolBuildingBase::setSpawnPatrolers(const bool s) {
    mSpawnPatrolers = s;
}