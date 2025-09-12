#include "eeventwidget.h"

#include "engine/egameboard.h"
#include "engine/eevent.h"
#include "textures/egametextures.h"
#include "engine/eeventdata.h"

void eEventWidget::pushEvent(const eEvent e, const eEventData& ed) {
    const auto button = new eEventButton(e, window());
    mButtons.insert(mButtons.begin(), button);
    if(mButtons.size() > 4) {
        const auto b = mButtons.back();
        b->deleteLater();
        mButtons.pop_back();
    }
    prependWidget(button);
    button->setPressAction([this, ed]() {
        if(mViewTileHandler) {
            const auto ch = ed.fChar;
            if(ch) {
                const auto tile = ch->tile();
                mViewTileHandler(tile);
            } else {
                const auto tile = ed.fTile;
                mViewTileHandler(tile);
            }
        }
    });
    setHeight(button->height());
    setWidth(4*button->width());
    stackHorizontally();
}

void eEventWidget::clear() {
    removeChildren();
}

void eEventWidget::setViewTileHandler(const eViewTileHandler& h) {
    mViewTileHandler = h;
}

eEventButton::eEventButton(const eEvent e,
                           eMainWindow* const window) :
    eButton(window) {
    const auto intrfc = eGameTextures::interface();
    const auto uiScale = resolution().uiScale();
    const int iRes = static_cast<int>(uiScale);
    const auto& texs = intrfc[iRes];
    const eTextureCollection* coll = nullptr;
    switch(e) {
    case eEvent::fire:
        coll = &texs.fFireAlert;
        break;
    case eEvent::collapse:
        coll = &texs.fCollapseAltert;
        break;
    case eEvent::earthquake:
    case eEvent::earthquakeGod:
        coll = &texs.fGroundFissureAlert;
        break;
    case eEvent::tidalWave:
    case eEvent::tidalWaveGod:
        coll = &texs.fFloodAlert;
        break;
    case eEvent::lavaFlow:
        coll = &texs.fLavaAltert;
        break;

    case eEvent::godVisit:
    case eEvent::godHelp:
        coll = &texs.fGodVisitAlert;
        break;

    case eEvent::godInvasion:
    case eEvent::playerGodAttack:
        coll = &texs.fGodAttackAlert;
        break;
    case eEvent::monsterInvasion:
    case eEvent::godMonsterUnleash:
    case eEvent::monsterInCity:
        coll = &texs.fMonsterAltert;
        break;
    case eEvent::heroArrival:
        coll = &texs.fHeroArrivalAlert;
        break;
    case eEvent::invasion:
    case eEvent::playerInvasion:
        coll = &texs.fInvasionAlert;
        break;
    case eEvent::plague:
        coll = &texs.fIllnessAlert;
        break;
    case eEvent::armyReturns:
    case eEvent::aidArrives:
        coll = &texs.fArmyComebackAlert;
        break;
    default:
        return;
    }

    setTexture(coll->getTexture(0));
    setHoverTexture(coll->getTexture(1));
    setPressedTexture(coll->getTexture(2));

    setNoPadding();
    fitContent();
}
