#include "eeconomicmilitarychangeeventbase.h"

void eEconomicMilitaryChangeEventBase::write(eWriteStream& dst) const {
    eGameEvent::write(dst);
    dst << mBy;
    dst.writeCity(mCity.get());
}

void eEconomicMilitaryChangeEventBase::read(eReadStream& src) {
    eGameEvent::read(src);
    src >> mBy;
    auto& board = getBoard();
    src.readCity(&board, [this](const stdsptr<eWorldCity>& c) {
        mCity = c;
    });
}