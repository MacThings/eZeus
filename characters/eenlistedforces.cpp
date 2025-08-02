#include "eenlistedforces.h"

#include <algorithm>

#include "engine/egameboard.h"
#include "enumbers.h"

void eEnlistedForces::read(eGameBoard& board,
                           eWorldBoard& wboard,
                           eReadStream& src) {
    {
        int ns;
        src >> ns;
        for(int i = 0; i < ns; i++) {
            const auto func = [this](const stdsptr<eSoldierBanner>& b) {
                fSoldiers.push_back(b);
            };
            src.readSoldierBanner(&board, func);
        }
    }
    {
        int nh;
        src >> nh;
        for(int i = 0; i < nh; i++) {
            eHeroType h;
            src >> h;
            fHeroes.push_back(h);
        }
    }
    {
        int nc;
        src >> nc;
        for(int i = 0; i < nc; i++) {
            const auto func = [this](const stdsptr<eWorldCity>& c) {
                fAllies.push_back(c);
            };
            src.readCity(&wboard, func);
        }
    }

    src >> fAres;
}

void eEnlistedForces::write(eWriteStream& dst) const {
    dst << fSoldiers.size();
    for(const auto& s : fSoldiers) {
        dst.writeSoldierBanner(s.get());
    }

    dst << fHeroes.size();
    for(const auto h : fHeroes) {
        dst << h;
    }

    dst << fAllies.size();
    for(const auto& c : fAllies) {
        dst.writeCity(c.get());
    }

    dst << fAres;
}

void eEnlistedForces::clear() {
    fSoldiers.clear();
    fHeroes.clear();
    fAllies.clear();
    fAres = false;
}

void eEnlistedForces::add(const eEnlistedForces& o) {
    for(const auto& s : o.fSoldiers) {
        fSoldiers.push_back(s);
    }
    for(const auto& h : o.fHeroes) {
        fHeroes.push_back(h);
    }
    for(const auto& a : o.fAllies) {
        fAllies.push_back(a);
    }
    fAres = fAres || o.fAres;
}

const double gHelpFrac = 0.33;

int eEnlistedForces::strength() const {
    int str = 0;
    for(const auto& s : fSoldiers) {
        double mult = 1.;
        switch(s->type()) {
        case eBannerType::horseman:
            mult = 1.5;
            break;
        default:
            break;
        }

        str += std::floor(mult*s->count());
    }
    for(const auto& c : fAllies) {
        str += gHelpFrac*c->troops();
    }
    str += 8*fHeroes.size();
    if(fAres) str += 8;
    return str;
}

void eEnlistedForces::kill(const double killFrac) const {
    for(const auto& s : fSoldiers) {
        const auto cid = s->cityId();
        const int oC = s->count();
        int nC = std::round((1 - killFrac)*oC);
        nC = std::clamp(nC, 0, eNumbers::sSoldiersPerBanner);
        auto& board = s->getBoard();
        const auto type = s->type();
        for(int i = nC; i < oC; i++) {
            s->decCount();
            switch(type) {
            case eBannerType::hoplite:
                board.hopliteKilled(cid);
                break;
            case eBannerType::horseman:
                board.horsemanKilled(cid);
                break;
            default:
                break;
            }
        }
    }

    for(const auto& c : fAllies) {
        const int t = c->troops();
        c->setTroops((1 - gHelpFrac*killFrac)*t);
    }
}

int eEnlistedForces::count() const {
    int result = 0;
    for(const auto& b : fSoldiers) {
        result += b->count();
    }
    for(const auto& f : fAllies) {
        std::vector<eSoldierBanner*> solds;
        int infantry;
        int cavalry;
        int archers;
        f->troopsByType(infantry, cavalry, archers);
        result += infantry;
        result += cavalry;
        result += archers;
    }
    return result;
}
