#include "edefendattackcityaction.h"

#include "emovetoaction.h"
#include "engine/egameboard.h"
#include "characters/esoldier.h"
#include "characters/actions/esoldieraction.h"
#include "characters/actions/egodaction.h"
#include "vec2.h"

void eDefendAttackCityAction::read(eReadStream& src) {
    auto& board = eGodMonsterAction::board();
    eGodMonsterAction::read(src);
    src >> mStage;
    mStartTile = src.readTile(board);
    src.readCharacter(&board, [this](eCharacter* const c) {
        mAttackTarget = c;
    });
    src >> mAttack;
    src >> mLookForEnemy;
    src >> mAttackTime;
    src >> mRangeAttack;
    src >> mAngle;
    src >> mMissile;
    src >> mKilled;
}

void eDefendAttackCityAction::write(eWriteStream& dst) const {
    eGodMonsterAction::write(dst);
    dst << mStage;
    dst.writeTile(mStartTile);
    dst.writeCharacter(mAttackTarget);
    dst << mAttack;
    dst << mLookForEnemy;
    dst << mAttackTime;
    dst << mRangeAttack;
    dst << mAngle;
    dst << mMissile;
    dst << mKilled;
}

bool eDefendAttackCityAction::goTo(const int fx, const int fy, const int dist) {
    const auto c = character();
    const auto t = c->tile();
    const int sx = t->x();
    const int sy = t->y();
    const int dx = fx - sx;
    const int dy = fy - sy;
    if(sqrt(dx*dx + dy*dy) <= dist) return true;

    const auto hha = [fx, fy, dist](eThreadTile* const t) {
        const int dx = fx - t->x();
        const int dy = fy - t->y();
        return sqrt(dx*dx + dy*dy) <= dist;
    };

    const auto walkable =
            eWalkableObject::sCreateDefault();

    const auto a = e::make_shared<eMoveToAction>(c);

    const stdptr<eCharacter> cptr(character());
    const stdptr<eDefendAttackCityAction> tptr(this);
    a->setFoundAction([cptr, tptr]() {
        if(!cptr) return;
        cptr->setActionType(eCharacterActionType::walk);
    });
    a->start(hha, walkable, nullptr,
             [fx, fy](eThreadBoard& board) {
        return board.tile(fx, fy);
    });
    setCurrentAction(a);
    return false;
}

void eDefendAttackCityAction::goBack() {
    auto& board = eGodMonsterAction::board();
    using eGTTT = eGoToTargetTeleport;
    const auto tele = std::make_shared<eGTTT>(board, this);
    goToTile(mStartTile, tele);
}

void eDefendAttackCityAction::increment(const int by) {
    if(mKilled >= mMaxKilled) {
        return eGodMonsterAction::increment(by);
    }
    const int rangeAttackCheck = 500;
    const int lookForEnemyCheck = 500;
    int missileCheck = 200;

    auto& brd = board();
    const auto c = character();
    if(c->dead()) return;
    const auto cTile = c->tile();
    if(!cTile) return;
    const int tx = cTile->x();
    const int ty = cTile->y();
    const int range = this->range();
    const auto tid = c->teamId();
    const auto ct = c->type();

    if(mAttack) {
        if(range > 0 && mAttackTarget && mAttackTarget->tile()) {
            bool isGod;
            const auto gt = eGod::sCharacterToGodType(ct, &isGod);
            if(isGod) {
                missileCheck = eGod::sGodAttackTime(gt);
            } else {
                bool isHero;
                const auto ht = eHero::sCharacterToHeroType(ct, &isHero);
                if(isHero) {
                    missileCheck = eHero::sHeroAttackTime(ht);
                } else {
                    bool isMonster;
                    const auto mt = eMonster::sCharacterToMonsterType(ct, &isMonster);
                    if(isMonster) {
                        missileCheck = eMonster::sMonsterAttackTime(mt);
                    } else {
                        missileCheck = 200;
                    }
                }
            }
            mMissile += by;
            if(mMissile > missileCheck) {
                mMissile -= missileCheck;
                const auto tt = mAttackTarget->tile();
                spawnMissile(eCharacterActionType::fight2,
                             ct, missileCheck, tt,
                             nullptr, nullptr);
            }
        }
        mAttackTime += by;
        bool finishAttack = !mAttackTarget ||
                            mAttackTarget->dead() ||
                            mAttackTime > 1000;
        if(mAttackTarget && !mAttackTarget->dead()) {
            const double att = by*c->attack();
            const bool d = mAttackTarget->defend(att);
            if(d) finishAttack = true;
        }
        if(finishAttack) {
            mAttack = false;
            mAttackTarget.clear();
            mAttackTime = 0;
            mRangeAttack = rangeAttackCheck;
            c->setActionType(eCharacterActionType::stand);
            setCurrentAction(nullptr);
            mLookForEnemy = lookForEnemyCheck;
            mKilled++;
            if(mKilled >= mMaxKilled) {
                mStage = eDefendAttackCityStage::comeback;
                goBack();
                return eGodMonsterAction::increment(by);
            }
        } else {
            return;
        }
    }

    const vec2d cpos{c->absX(), c->absY()};
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            const auto t = brd.tile(tx + i, ty + j);
            if(!t) continue;
            const auto& chars = t->characters();
            for(const auto& cc : chars) {
                if(!cc->isSoldier()) continue;
                const auto cctid = cc->teamId();
                if(!eTeamIdHelpers::isEnemy(cctid, tid)) continue;
                if(cc->dead()) continue;
                const vec2d ccpos{cc->absX(), cc->absY()};
                const vec2d posdif = ccpos - cpos;
                const double dist = posdif.length();
                if(dist > 1.) continue;
                mAttackTarget = cc;
                mAttack = true;
                mAttackTime = 0;
                c->setActionType(eCharacterActionType::fight);
                mAngle = posdif.angle();
                const auto o = sAngleOrientation(mAngle);
                c->setOrientation(o);
                return;
            }
        }
    }

    if(range > 0) {
        mRangeAttack += by;
        if(mRangeAttack > rangeAttackCheck) {
            mRangeAttack -= rangeAttackCheck;
            for(int i = -range; i <= range; i++) {
                for(int j = -range; j <= range; j++) {
                    const auto t = brd.tile(tx + i, ty + j);
                    if(!t) continue;
                    const auto& chars = t->characters();
                    for(const auto& cc : chars) {
                        if(!cc->isSoldier()) continue;
                        const auto cctid = cc->teamId();
                        if(!eTeamIdHelpers::isEnemy(cctid, tid)) continue;
                        if(cc->dead()) continue;
                        const vec2d ccpos{cc->absX(), cc->absY()};
                        const vec2d posdif = ccpos - cpos;
                        mAttackTarget = cc;
                        mAttack = true;
                        mAttackTime = 0;
                        c->setActionType(eCharacterActionType::fight2);
                        mAngle = posdif.angle();
                        const auto o = sAngleOrientation(mAngle);
                        c->setOrientation(o);
                        const auto ss = static_cast<eSoldier*>(cc.get());
                        eSoldierAction::sSignalBeingAttack(ss, c, brd);
                        return;
                    }
                }
            }
        }
    }

    mLookForEnemy += by;
    if(mLookForEnemy > lookForEnemyCheck) {
        mLookForEnemy -= lookForEnemyCheck;
        const int erange = 3 + range;
        bool found = false;
        for(int i = -erange; i <= erange && !found; i++) {
            for(int j = -erange; j <= erange && !found; j++) {
                const int ttx = tx + i;
                const int tty = ty + j;
                const auto t = brd.tile(ttx, tty);
                if(!t) continue;
                const auto& chars = t->characters();
                for(const auto& cc : chars) {
                    if(!cc->isSoldier()) continue;
                    const auto cctid = cc->teamId();
                    if(!eTeamIdHelpers::isEnemy(cctid, tid)) continue;
                    if(cc->dead()) continue;
                    found = true;
                    goTo(ttx, tty, range);
                    break;
                }
            }
        }
    }

    eGodMonsterAction::increment(by);
}

int eDefendAttackCityAction::range() const {
    const auto c = character();
    const auto ct = c->type();
    switch(ct) {
    case eCharacterType::atalanta:
        return 5;
    case eCharacterType::artemis:
    case eCharacterType::apollo:
        return 5;
    default:
        return 0;
    }
    return 0;
}
