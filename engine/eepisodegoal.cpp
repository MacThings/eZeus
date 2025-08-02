#include "eepisodegoal.h"

#include "elanguage.h"
#include "estringhelpers.h"
#include "characters/gods/egod.h"
#include "characters/esoldierbanner.h"
#include "engine/eworldcity.h"
#include "buildings/esmallhouse.h"
#include "buildings/eelitehousing.h"
#include "engine/egameboard.h"

stdsptr<eEpisodeGoal> eEpisodeGoal::makeCopy() const {
    const auto result = std::make_shared<eEpisodeGoal>();
    *result = *this;
    return result;
}

void eEpisodeGoal::read(eWorldBoard* const board,
                        eReadStream& src) {
    src >> fType;
    src >> fEnumInt1;
    src >> fEnumInt2;
    src >> fRequiredCount;
    src >> fStatusCount;
    src.readCity(board, [this](const stdsptr<eWorldCity>& c) {
        fCity = c;
    });
}

void eEpisodeGoal::write(eWriteStream& dst) const {
    dst << fType;
    dst << fEnumInt1;
    dst << fEnumInt2;
    dst << fRequiredCount;
    dst << fStatusCount;
    dst.writeCity(fCity.get());
}

bool eEpisodeGoal::met() const {
    return fStatusCount >= fRequiredCount;
}

std::string eEpisodeGoal::sText(const eEpisodeGoalType type) {
    switch(type) {
    case eEpisodeGoalType::population:
        return eLanguage::zeusText(194, 0);
    case eEpisodeGoalType::treasury:
        return eLanguage::zeusText(194, 1);
    case eEpisodeGoalType::sanctuary:
        return eLanguage::zeusText(194, 2);
    case eEpisodeGoalType::support:
        return eLanguage::zeusText(194, 3);
    case eEpisodeGoalType::quest:
        return eLanguage::zeusText(194, 4);
    case eEpisodeGoalType::slay:
        return eLanguage::zeusText(194, 5);
    case eEpisodeGoalType::yearlyProduction:
        return eLanguage::zeusText(194, 6);
    case eEpisodeGoalType::rule:
        return eLanguage::zeusText(194, 7);
    case eEpisodeGoalType::yearlyProfit:
        return eLanguage::zeusText(194, 8);
    case eEpisodeGoalType::housing:
        return eLanguage::zeusText(194, 9);
    case eEpisodeGoalType::setAsideGoods:
        return eLanguage::zeusText(194, 14);
    case eEpisodeGoalType::surviveUntil:
        return eLanguage::zeusText(194, 13);
    case eEpisodeGoalType::completeBefore:
        return eLanguage::zeusText(194, 12);
    case eEpisodeGoalType::tradingPartners:
        return eLanguage::zeusText(194, 10);
    }
}

std::string eEpisodeGoal::text(const bool colonyEpisode,
                               const bool atlantean) const {
    switch(fType) {
    case eEpisodeGoalType::population: {
        auto t = eLanguage::zeusText(194, 17);
        eStringHelpers::replace(t, "[amount]", std::to_string(fRequiredCount));
        return t;
    } break;
    case eEpisodeGoalType::treasury: {
        auto t = eLanguage::zeusText(194, 18);
        eStringHelpers::replace(t, "[amount]", std::to_string(fRequiredCount));
        return t;
    } break;
    case eEpisodeGoalType::sanctuary: {
        if(fRequiredCount == 0) {
            auto t = eLanguage::zeusText(194, 19);
            const auto type = static_cast<eGodType>(fEnumInt1);
            eStringHelpers::replace(t, "[god]", eGod::sGodName(type));
            return t;
        } else if(fRequiredCount == 1) {
            return eLanguage::zeusText(194, 20);
        } else {
            auto t = eLanguage::zeusText(194, 22);
            eStringHelpers::replace(t, "[amount]", std::to_string(fRequiredCount));
            return t;
        }
    } break;
    case eEpisodeGoalType::support: {
        auto t = eLanguage::zeusText(194, 24);
        const auto type = static_cast<eBannerType>(fEnumInt1);
        eStringHelpers::replace(t, "[amount]", std::to_string(fRequiredCount));
        const auto name = eSoldierBanner::sName(type, atlantean);
        eStringHelpers::replace(t, "[military_or_better]", name);
        return t;
    } break;
    case eEpisodeGoalType::quest: {
        return eLanguage::zeusText(194, 26);
    } break;
    case eEpisodeGoalType::slay: {
        return eLanguage::zeusText(194, 27);
    } break;
    case eEpisodeGoalType::yearlyProduction: {
        auto t = eLanguage::zeusText(194, 30);
        eStringHelpers::replace(t, "[amount]", std::to_string(fRequiredCount));
        const auto res = static_cast<eResourceType>(fEnumInt1);
        const auto resName = eResourceTypeHelpers::typeLongName(res);
        eStringHelpers::replace(t, "[item]", resName);
    } break;
    case eEpisodeGoalType::rule: {
        auto t = eLanguage::zeusText(194, 31);
        const auto n = fCity ? fCity->name() : "";
        eStringHelpers::replace(t, "[city]", n);
        return t;
    } break;
    case eEpisodeGoalType::yearlyProfit: {
        auto t = eLanguage::zeusText(194, 32);
        eStringHelpers::replace(t, "[amount]", std::to_string(fRequiredCount));
        return t;
    } break;
    case eEpisodeGoalType::housing: {
        auto t = eLanguage::zeusText(194, 33);
        eStringHelpers::replace(t, "[amount]", std::to_string(fRequiredCount));
        std::string level;
        const auto type = fEnumInt1 == 0 ? eBuildingType::commonHouse :
                                           eBuildingType::eliteHousing;
        switch(type) {
        case eBuildingType::commonHouse:
            level = eSmallHouse::sName(fEnumInt2);
            break;
        case eBuildingType::eliteHousing:
            level = eEliteHousing::sName(fEnumInt2);
            break;
        default:
            break;
        }

        eStringHelpers::replace(t, "[house_level]", level);
        return t;
    } break;
    case eEpisodeGoalType::setAsideGoods: {
        auto t = colonyEpisode ? eLanguage::zeusText(194, 35) :
                                 eLanguage::zeusText(194, 36);
        eStringHelpers::replace(t, "[amount]", std::to_string(fRequiredCount));
        const auto res = static_cast<eResourceType>(fEnumInt1);
        const auto resName = eResourceTypeHelpers::typeLongName(res);
        eStringHelpers::replace(t, "[item]", resName);
        return t;
    } break;
    case eEpisodeGoalType::surviveUntil: {
        const auto sdate = date();
        auto text = eLanguage::zeusText(194, 37); // Survive until
        eStringHelpers::replace(text, "[finish_date]", sdate.shortString());
        return text;
    } break;
    case eEpisodeGoalType::completeBefore: {
        const auto sdate = date();
        auto text = eLanguage::zeusText(194, 38); // Complete before
        eStringHelpers::replace(text, "[finish_date]", sdate.shortString());
        return text;
    } break;
    case eEpisodeGoalType::tradingPartners: {
        const int c = fRequiredCount;
        const auto cStr = std::to_string(c);
        auto text = eLanguage::zeusText(194, 34); // trading partners
        eStringHelpers::replace(text, "[amount]", cStr);
        return text;
    } break;
    }
    return "";
}

std::string eEpisodeGoal::statusText(const eGameBoard* const b) const {
    switch(fType) {
    case eEpisodeGoalType::population: {
        auto text = eLanguage::zeusText(194, 43);
        const auto popStr = std::to_string(fStatusCount);
        eStringHelpers::replace(text, "[amount]", popStr);
        return text;
    } break;
    case eEpisodeGoalType::treasury: {
        auto text = eLanguage::zeusText(194, 44);
        const auto treStr = std::to_string(fStatusCount);
        eStringHelpers::replace(text, "[amount]", treStr);
        return text;
    } break;
    case eEpisodeGoalType::sanctuary: {
        auto text = eLanguage::zeusText(194, 45);
        const auto perStr = std::to_string(fStatusCount);
        eStringHelpers::replace(text, "[percent]", perStr + "%");
        return text;
    } break;
    case eEpisodeGoalType::support: {
        auto text = eLanguage::zeusText(194, 50);
        const auto countStr = std::to_string(fStatusCount);
        eStringHelpers::replace(text, "[amount]", countStr);
        return text;
    } break;
    case eEpisodeGoalType::quest: {
        if(fStatusCount == 0) {
            return eLanguage::zeusText(194, 52);
        } else {
            return eLanguage::zeusText(194, 51);
        }
    } break;
    case eEpisodeGoalType::slay: {
        if(fStatusCount == 0) {
            return eLanguage::zeusText(194, 54);
        } else {
            return eLanguage::zeusText(194, 53);
        }
    } break;
    case eEpisodeGoalType::yearlyProduction: {
        auto t = eLanguage::zeusText(194, 55);
        const auto countStr = std::to_string(fStatusCount);
        eStringHelpers::replace(t, "[amount]", countStr);
        return t;
    } break;
    case eEpisodeGoalType::rule: {
        auto t = eLanguage::zeusText(194, 56);
        const auto rel = fCity->relationship();
        const auto relStr = eWorldCity::sRelationshipName(rel);
        eStringHelpers::replace(t, "[diplomatic_status]", relStr);
        return t;
    } break;
    case eEpisodeGoalType::yearlyProfit: {
        auto t = eLanguage::zeusText(194, 57);
        const auto countStr = std::to_string(fStatusCount);
        eStringHelpers::replace(t, "[amount]", countStr);
        return t;
    } break;
    case eEpisodeGoalType::housing: {
        auto text = eLanguage::zeusText(194, 58);
        const auto qStr = std::to_string(fStatusCount);
        eStringHelpers::replace(text, "[amount]", qStr);
        return text;
    } break;
    case eEpisodeGoalType::setAsideGoods: {
        auto text = eLanguage::zeusText(194, 60);
        const auto countStr = std::to_string(fPreviewCount);
        eStringHelpers::replace(text, "[amount]", countStr);
        return text;
    } break;
    case eEpisodeGoalType::surviveUntil: {
        auto text = eLanguage::zeusText(194, 63); // months remaining
        const auto cdate = b->date();
        const auto sdate = date();
        int rem;
        if(cdate > sdate) {
            rem = 0;
        } else {
            rem = std::round((sdate - cdate)/30.5);
        }
        const auto countStr = std::to_string(rem);
        eStringHelpers::replace(text, "[months_remaining]", countStr);
        return text;
    } break;
    case eEpisodeGoalType::completeBefore: {
        auto text = eLanguage::zeusText(194, 63); // months remaining
        const auto cdate = b->date();
        const auto sdate = date();
        int rem;
        if(cdate > sdate) {
            rem = 0;
        } else {
            rem = std::round((sdate - cdate)/30.5);
        }
        const auto countStr = std::to_string(rem);
        eStringHelpers::replace(text, "[months_remaining]", countStr);
        return text;
    } break;
    case eEpisodeGoalType::tradingPartners: {
        const int c = fStatusCount;
        const auto cStr = std::to_string(c);
        auto text = eLanguage::zeusText(194, 59); // trading partners
        eStringHelpers::replace(text, "[amount]", cStr);
        return text;
    } break;
    }
    return "";
}

void eEpisodeGoal::update(const eGameBoard* const b) {
    const auto pid = b->personPlayer();
    switch(fType) {
    case eEpisodeGoalType::population: {
        const bool wasMet = met();
        fStatusCount = b->population(pid);
        const bool isMet = met();
        if(!wasMet && isMet) {
            b->showTip(pid, eLanguage::zeusText(194, 73));
        } else if(wasMet && !isMet) {
            b->showTip(pid, eLanguage::zeusText(194, 74));
        }
    } break;
    case eEpisodeGoalType::treasury: {
        const bool wasMet = met();
        fStatusCount = b->drachmas(pid);
        const bool isMet = met();
        if(!wasMet && isMet) {
            b->showTip(pid, eLanguage::zeusText(194, 75));
        } else if(wasMet && !isMet) {
            b->showTip(pid, eLanguage::zeusText(194, 76));
        }
    } break;
    case eEpisodeGoalType::sanctuary: {
        const bool wasMet = met();
        const auto type = static_cast<eGodType>(fEnumInt1);
        fStatusCount = 0;
        const auto cids = b->personPlayerCitiesOnBoard();
        for(const auto cid : cids) {
            const auto s = b->sanctuary(cid, type);
            const int sc = s ? s->progress() : 0;
            if(sc > fStatusCount) fStatusCount = sc;
        }
        const bool isMet = met();
        if(!wasMet && isMet) {
            b->showTip(pid, eLanguage::zeusText(194, 77));
        }
    } break;
    case eEpisodeGoalType::support: {
        const auto type = static_cast<eBannerType>(fEnumInt1);
        const auto cs = b->personPlayerCitiesOnBoard();
        fStatusCount = 0;
        for(const auto c : cs) {
            if(type == eBannerType::trireme) {
                fStatusCount += b->countWorkingTriremes(c);
            } else {
                fStatusCount += b->countSoldiers(type, c);
                if(type == eBannerType::rockThrower) {
                    fStatusCount += b->countSoldiers(eBannerType::hoplite, c);
                    fStatusCount += b->countSoldiers(eBannerType::horseman, c);
                } else if(type == eBannerType::hoplite) {
                    fStatusCount += b->countSoldiers(eBannerType::horseman, c);
                }
            }
        }
    } break;
    case eEpisodeGoalType::quest: {
        const bool wasMet = met();
        fStatusCount = b->fulfilledQuests(pid).size();
        const bool isMet = met();
        if(!wasMet && isMet) {
            b->showTip(pid, eLanguage::zeusText(194, 81));
        }
    } break;
    case eEpisodeGoalType::slay: {
        const bool wasMet = met();
        fStatusCount = b->slayedMonsters(pid).size();
        const bool isMet = met();
        if(!wasMet && isMet) {
            b->showTip(pid, eLanguage::zeusText(194, 83));
        }
    } break;
    case eEpisodeGoalType::yearlyProduction: {
        const bool wasMet = met();
        const auto type = static_cast<eResourceType>(fEnumInt1);
        fStatusCount = b->bestYearlyProduction(type);
        const bool isMet = met();
        if(!wasMet && isMet) {
            b->showTip(pid, eLanguage::zeusText(194, 85));
        }
    } break;
    case eEpisodeGoalType::rule: {
        const bool wasMet = met();
        fStatusCount = fCity->isVassal() ? 1 : 0;
        const bool isMet = met();
        if(!wasMet && isMet) {
            b->showTip(pid, eLanguage::zeusText(194, 87));
        }
    } break;
    case eEpisodeGoalType::yearlyProfit: {
        const bool wasMet = met();
        fStatusCount = b->bestYearlyProduction(eResourceType::drachmas);
        const bool isMet = met();
        if(!wasMet && isMet) {
            b->showTip(pid, eLanguage::zeusText(194, 89));
        }
    } break;
    case eEpisodeGoalType::housing: {
        const bool wasMet = met();
        fStatusCount = 0;
        const auto type = fEnumInt1 == 0 ? eBuildingType::commonHouse :
                                           eBuildingType::eliteHousing;
        const auto cids = b->personPlayerCitiesOnBoard();
        for(const auto cid : cids) {
            b->buildings(cid, [&](eBuilding* const b) {
                const auto btype = b->type();
                if(btype != type) return false;
                const auto h = static_cast<eHouseBase*>(b);
                const int l = h->level();
                if(l >= fEnumInt2) {
                    fStatusCount += h->people();
                }
                return false;
            });
        }
        const bool isMet = met();
        if(!wasMet && isMet) {
            b->showTip(pid, eLanguage::zeusText(194, 91));
        } else if(wasMet && !isMet) {
            b->showTip(pid, eLanguage::zeusText(194, 92));
        }
    } break;
    case eEpisodeGoalType::setAsideGoods: {
        const auto res = static_cast<eResourceType>(fEnumInt1);
        fPreviewCount = 0;
        const auto cids = b->personPlayerCitiesOnBoard();
        for(const auto cid : cids) {
            fPreviewCount += b->resourceCount(cid, res);
        }
    } break;
    case eEpisodeGoalType::surviveUntil: {
        const auto sdate = date();
        const auto cdate = b->date();
        fStatusCount = cdate > sdate ? 1 : 0;
    } break;
    case eEpisodeGoalType::completeBefore: {
        const auto sdate = date();
        const auto cdate = b->date();
        if(cdate > sdate) {
            b->setEpisodeLost();
        } else {
            fStatusCount = cdate < sdate ? 1 : 0;
        }
    } break;
    case eEpisodeGoalType::tradingPartners: {
        const bool wasMet = met();
        fStatusCount = b->tradingPartners();
        const bool isMet = met();
        if(!wasMet && isMet) {
            b->showTip(pid, eLanguage::zeusText(194, 93));
        } else if(wasMet && !isMet) {
            b->showTip(pid, eLanguage::zeusText(194, 94));
        }
    } break;
    }
}

eDate eEpisodeGoal::date() const {
    const int day = fEnumInt1;
    const int month = fEnumInt2;
    const int year = fRequiredCount;
    return eDate{day, static_cast<eMonth>(month), year};
}

void eEpisodeGoal::initializeDate(const eGameBoard* const b) {
    if(fType != eEpisodeGoalType::surviveUntil &&
       fType != eEpisodeGoalType::completeBefore) return;
    auto date = b->date();
    const int days = fEnumInt1;
    const int months = fEnumInt2;
    const int years = fRequiredCount;
    date.nextYears(years);
    date.nextMonths(months);
    bool nm;
    bool ny;
    date.nextDays(days, nm, ny);
    fEnumInt1 = date.day();
    fEnumInt2 = static_cast<int>(date.month());
    fRequiredCount = date.year();
}
