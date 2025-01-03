#include "eboarspawner.h"

#include "characters/eboar.h"
#include "enumbers.h"

eBoarSpawner::eBoarSpawner(const int id,
                           eTile* const tile,
                           eGameBoard& board) :
    eAnimalSpawner(eBannerTypeS::boar, id, tile,
                   eNumbers::sBoarMaxCount,
                   eNumbers::sBoarSpawnPeriod, board) {}

stdsptr<eWildAnimal> eBoarSpawner::create(eGameBoard& board) {
    const auto b = e::make_shared<eBoar>(board);
    b->setOnCityId(cityId());
    return b;
}
