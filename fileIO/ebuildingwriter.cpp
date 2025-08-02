#include "ebuildingwriter.h"

#include "buildings/allbuildings.h"
#include "engine/egameboard.h"

void eBuildingWriter::sWrite(const eBuilding* const b,
                             eWriteStream& dst) {
    const auto cid = b->cityId();
    dst << cid;
    const auto type = b->type();
    switch(type) {
    case eBuildingType::road: {
        const auto r = static_cast<const eRoad*>(b);
        dst << r->isRoadblock();
        const auto a = r->underAgora();
        dst.writeBuilding(a);
        const auto g = r->underGatehouse();
        dst.writeBuilding(g);
    } break;
    case eBuildingType::commonAgora: {
        const auto ca = static_cast<const eCommonAgora*>(b);
        dst << ca->orientation();
    } break;
    case eBuildingType::grandAgora: {
        const auto ga = static_cast<const eGrandAgora*>(b);
        dst << ga->orientation();
    } break;
    case eBuildingType::agoraSpace:
        return;
    case eBuildingType::commonHouse:
    case eBuildingType::gymnasium:
    case eBuildingType::podium:
    case eBuildingType::fountain:
    case eBuildingType::watchPost:
    case eBuildingType::maintenanceOffice:
    case eBuildingType::college:
    case eBuildingType::dramaSchool:
    case eBuildingType::theater:
    case eBuildingType::hospital:
    case eBuildingType::bibliotheke:
    case eBuildingType::observatory:
    case eBuildingType::university:
    case eBuildingType::laboratory:
    case eBuildingType::inventorsWorkshop:
    case eBuildingType::museum:
        break;
    case eBuildingType::stadium: {
        const auto s = static_cast<const eStadium*>(b);
        dst << s->rotated();
    } break;
    case eBuildingType::palace: {
        const auto p = static_cast<const ePalace*>(b);
        dst << p->rotated();
    } break;
    case eBuildingType::palaceTile: {
        const auto p = static_cast<const ePalaceTile*>(b);
        dst << p->other();
        dst.writeBuilding(p->palace());
    } break;
    case eBuildingType::eliteHousing:
    case eBuildingType::taxOffice:
    case eBuildingType::mint:
    case eBuildingType::foundry:
    case eBuildingType::timberMill:
    case eBuildingType::refinery:
    case eBuildingType::blackMarbleWorkshop:
    case eBuildingType::masonryShop:
    case eBuildingType::oliveTree:
    case eBuildingType::vine:
    case eBuildingType::orangeTree:
    case eBuildingType::huntingLodge:
    case eBuildingType::corral:
    case eBuildingType::chariotFactory:
        break;
    case eBuildingType::urchinQuay: {
        const auto p = static_cast<const eUrchinQuay*>(b);
        dst << p->orientation();
    } break;
    case eBuildingType::fishery: {
        const auto p = static_cast<const eFishery*>(b);
        dst << p->orientation();
    } break;
    case eBuildingType::triremeWharf: {
        const auto p = static_cast<const eTriremeWharf*>(b);
        dst << p->orientation();
    } break;
    case eBuildingType::pier: {
        const auto p = static_cast<const ePier*>(b);
        dst << p->orientation();
        const auto tp = p->tradePost();
        dst.writeBuilding(tp);
    } break;
    case eBuildingType::tradePost: {
        const auto tp = static_cast<const eTradePost*>(b);
        const auto tpt = tp->tpType();
        dst << tpt;
        dst << tp->orientation();
        const auto& c = tp->city();
        dst << c.cityId();

        if(tpt == eTradePostType::pier) {
            const auto ub = tp->unpackBuilding();
            dst.writeBuilding(ub);
        }
    } break;
    case eBuildingType::dairy:
    case eBuildingType::cardingShed:
    case eBuildingType::sheep:
    case eBuildingType::goat:
    case eBuildingType::cattle:
    case eBuildingType::wheatFarm:
    case eBuildingType::onionsFarm:
    case eBuildingType::carrotsFarm:
    case eBuildingType::growersLodge:
    case eBuildingType::orangeTendersLodge:
    case eBuildingType::granary:
    case eBuildingType::warehouse:
    case eBuildingType::wall:
    case eBuildingType::tower:
        break;
    case eBuildingType::gatehouse: {
        const auto p = static_cast<const eGatehouse*>(b);
        dst << p->rotated();
    } break;
    case eBuildingType::armory:
        break;

    case eBuildingType::horseRanch: {
        const auto hr = static_cast<const eHorseRanch*>(b);
        const auto hre = hr->enclosure();
        dst.writeBuilding(hre);
    } break;
    case eBuildingType::horseRanchEnclosure: {
        const auto hre = static_cast<const eHorseRanchEnclosure*>(b);
        const auto hr = hre->ranch();
        dst.writeBuilding(hr);
    } break;

    case eBuildingType::olivePress:
    case eBuildingType::winery:
    case eBuildingType::sculptureStudio:
    case eBuildingType::artisansGuild:
        break;

    case eBuildingType::foodVendor:
    case eBuildingType::fleeceVendor:
    case eBuildingType::oilVendor:
    case eBuildingType::wineVendor:
    case eBuildingType::armsVendor:
    case eBuildingType::horseTrainer:
    case eBuildingType::chariotVendor: {
        const auto v = static_cast<const eVendor*>(b);
        const auto a = v->agora();
        dst.writeBuilding(a);
        const int sid = v->agoraSpaceId();
        dst << sid;
    } break;

    case eBuildingType::park:
    case eBuildingType::doricColumn:
    case eBuildingType::ionicColumn:
    case eBuildingType::corinthianColumn:
    case eBuildingType::avenue:
        break;

    case eBuildingType::commemorative: {
        const auto p = static_cast<const eCommemorative*>(b);
        dst << p->id();
    } break;

    case eBuildingType::godMonument: {
        const auto gm = static_cast<const eGodMonument*>(b);
        dst << gm->god();
        dst << gm->id();
    } break;
    case eBuildingType::godMonumentTile: {
        const auto gmt = static_cast<const eGodMonumentTile*>(b);
        dst.writeBuilding(gmt->monument());
    } break;

    case eBuildingType::bench:
    case eBuildingType::flowerGarden:
    case eBuildingType::gazebo:
    case eBuildingType::hedgeMaze:
    case eBuildingType::fishPond:
    case eBuildingType::orrery:
        break;

    case eBuildingType::waterPark: {
        const auto p = static_cast<const eWaterPark*>(b);
        dst << p->id();
    } break;

    case eBuildingType::birdBath:
    case eBuildingType::shortObelisk:
    case eBuildingType::tallObelisk:
    case eBuildingType::shellGarden:
    case eBuildingType::sundial:
    case eBuildingType::dolphinSculpture:
    case eBuildingType::spring:
    case eBuildingType::topiary:
    case eBuildingType::baths:
    case eBuildingType::stoneCircle:
        break;

    case eBuildingType::templeAphrodite:
    case eBuildingType::templeApollo:
    case eBuildingType::templeAres:
    case eBuildingType::templeArtemis:
    case eBuildingType::templeAthena:
    case eBuildingType::templeAtlas:
    case eBuildingType::templeDemeter:
    case eBuildingType::templeDionysus:
    case eBuildingType::templeHades:
    case eBuildingType::templeHephaestus:
    case eBuildingType::templeHera:
    case eBuildingType::templeHermes:
    case eBuildingType::templePoseidon:
    case eBuildingType::templeZeus: {
        const auto s = static_cast<const eSanctuary*>(b);
        dst << s->spanW();
        dst << s->spanH();
    } break;
    case eBuildingType::templeStatue: {
        const auto s = static_cast<const eTempleStatueBuilding*>(b);
        dst << s->godType();
        dst << s->id();
        const auto sanct = s->sanctuary();
        dst << sanct->ioID();
    } break;
    case eBuildingType::templeMonument: {
        const auto s = static_cast<const eTempleMonumentBuilding*>(b);
        dst << s->godType();
        dst << s->id();
        const auto sanct = s->sanctuary();
        dst.writeBuilding(sanct);
    } break;
    case eBuildingType::templeAltar: {
        const auto s = static_cast<const eTempleAltarBuilding*>(b);
        const auto sanct = s->sanctuary();
        dst.writeBuilding(sanct);
    } break;
    case eBuildingType::temple: {
        const auto s = static_cast<const eTempleBuilding*>(b);
        const auto sanct = s->sanctuary();
        dst.writeBuilding(sanct);
    } break;
    case eBuildingType::templeTile: {
        const auto s = static_cast<const eTempleTileBuilding*>(b);
        dst << s->id();
        const auto sanct = s->sanctuary();
        dst.writeBuilding(sanct);
    } break;

    case eBuildingType::ruins:
    case eBuildingType::placeholder:
        break;

    case eBuildingType::achillesHall:
    case eBuildingType::atalantaHall:
    case eBuildingType::bellerophonHall:
    case eBuildingType::herculesHall:
    case eBuildingType::jasonHall:
    case eBuildingType::odysseusHall:
    case eBuildingType::perseusHall:
    case eBuildingType::theseusHall:
        break;

    case eBuildingType::none:
    case eBuildingType::erase:
    case eBuildingType::bridge:
        return;
    }
    b->write(dst);
}
