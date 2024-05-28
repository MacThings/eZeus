#ifndef EINVASIONHANDLER_H
#define EINVASIONHANDLER_H

#include <vector>

#include "pointers/estdselfref.h"

class eGameBoard;
class eInvasionEvent;
class eTile;
class eReadStream;
class eWriteStream;
class eWorldCity;
class eSoldierBanner;

enum class eInvasionStage {
    spread, invade, comeback
};

class eInvasionHandler {
public:
    eInvasionHandler(eGameBoard& board,
                     const stdsptr<eWorldCity>& city);
    ~eInvasionHandler();

    void initialize(eTile* const tile,
                    const int infantry,
                    const int cavalry,
                    const int archers);

    void incTime(const int by);

    void read(eReadStream& src);
    void write(eWriteStream& dst) const;

    void killAllWithCorpse();

    eInvasionStage stage() const { return mStage; }
    eTile* tile() const { return mTile; }
private:
    eGameBoard& mBoard;
    stdsptr<eWorldCity> mCity;
    eTile* mTile = nullptr;
    eInvasionStage mStage = eInvasionStage::spread;
    std::vector<stdsptr<eSoldierBanner>> mBanners;

    int mWait = 0;
};

#endif // EINVASIONHANDLER_H
