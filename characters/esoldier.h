#ifndef ESOLDIER_H
#define ESOLDIER_H

#include "efightingpatroler.h"

class eSoldierAction;
class eSoldierBanner;

class eSoldier : public eFightingPatroler {
public:
    using eCharTexs = eFightingCharacterTextures eCharacterTextures::*;
    eSoldier(eGameBoard& board,
             const eCharTexs charTexs,
             const eCharacterType type);
    ~eSoldier();

    eSoldierAction* soldierAction() const;

    int range() const { return mRange; }
    void setRange(const int r) { mRange = r; }

    eSoldierBanner* banner() const;
    void setBanner(eSoldierBanner* const b);
private:
    stdptr<eSoldierBanner> mBanner;

    int mRange = 0;
};

#endif // ESOLDIER_H
