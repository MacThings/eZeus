﻿#include "estorageinfowidget.h"

#include "textures/egametextures.h"
#include "textures/einterfacetextures.h"

#include "elanguage.h"

class eResourceStorageStack : public eWidget {
public:
    using eWidget::eWidget;

    void initialize(eStorageBuilding* const stor,
                    const std::vector<eResourceType>& types,
                    const eResourceType get,
                    const eResourceType empty,
                    const eResourceType accept,
                    std::map<eResourceType, eSwitchButton*>& buttons,
                    std::map<eResourceType, eSpinBox*>& spinBoxes,
                    const std::map<eResourceType, int>& maxCount) {
        const auto countW = new eWidget(window());
        const auto iconsW = new eWidget(window());
        const auto namesW = new eWidget(window());
        const auto buttonsW = new eWidget(window());
        const auto spinsW = new eWidget(window());

        const auto res = resolution();
        const double mult = res.multiplier();
        const int rowHeight = mult*40;
        const int countWidth = mult*25;
        const int iconsWidth = mult*40;
        const int namesWidth = mult*80;
        const int buttonsWidth = mult*120;
        const int spinsWidth = mult*90;

        for(const auto type : types) {
            const auto count = new eLabel(window());
            count->setSmallFontSize();
            const int c = stor->count(type);
            count->setText(std::to_string(c));
            count->fitContent();
            count->setHeight(rowHeight);

            const auto icon = new eLabel(window());
            const auto tex = eResourceTypeHelpers::icon(
                                 res.uiScale(), type);
            icon->setTexture(tex);
            icon->fitContent();
            icon->setHeight(rowHeight);

            const auto nameStr = eResourceTypeHelpers::typeName(type);
//            const auto n = new eLabel(window());
            const auto n = new eButton(window());
            n->setPressAction([stor, type, count]() {
                stor->add(type, 1);
                const int c = stor->count(type);
                count->setText(std::to_string(c));
            });
            n->setSmallFontSize();
            n->setText(nameStr);
            n->fitContent();
            n->setHeight(rowHeight);

            const auto b = new eSwitchButton(window());

            b->setSwitchAction([b](const int i) {
                if(i == 0 || i == 3) {
                    b->setDarkFontColor();
                } else {
                    b->setLightFontColor();
                }
            });

            b->setSmallFontSize();
            b->addValue(eLanguage::text("dont_accept"));
            b->addValue(eLanguage::text("accept"));
            b->addValue(eLanguage::text("get"));
            b->addValue(eLanguage::text("empty"));
            b->fitContent();
            b->setHeight(rowHeight);

            buttons[type] = b;

            const auto s = new eSpinBox(window());
            s->setHeight(rowHeight);
            s->setWidth(spinsWidth);
            s->initialize();
            s->label()->setSmallFontSize();
            const int space = stor->spaceCount();
            if(type == eResourceType::sculpture) {
                s->setRange(0, space);
            } else {
                s->setRange(0, 4*space);
                s->setIncrement(4);
            }
            s->setValue(maxCount.at(type));

            spinBoxes[type] = s;

            countW->addWidget(count);
            iconsW->addWidget(icon);
            namesW->addWidget(n);
            buttonsW->addWidget(b);
            spinsW->addWidget(s);

            icon->align(eAlignment::left);
            n->align(eAlignment::left);
            b->align(eAlignment::left);

            if(static_cast<bool>(get & type)) {
                b->setValue(2);
            } else if(static_cast<bool>(empty & type)) {
                b->setValue(3);
            } else if(static_cast<bool>(accept & type)) {
                b->setValue(1);
            } else {
                b->setValue(0);
            }
        }

        countW->stackVertically();
        iconsW->stackVertically();
        namesW->stackVertically();
        buttonsW->stackVertically();
        spinsW->stackVertically();

        countW->fitContent();
        iconsW->fitContent();
        namesW->fitContent();
        buttonsW->fitContent();
        spinsW->fitContent();

        countW->setWidth(countWidth);
        iconsW->setWidth(iconsWidth);
        namesW->setWidth(namesWidth);
        buttonsW->setWidth(buttonsWidth);
        spinsW->setWidth(spinsWidth);

        addWidget(countW);
        addWidget(iconsW);
        addWidget(namesW);
        addWidget(buttonsW);
        addWidget(spinsW);

        stackHorizontally();
    }
};

void eStorageInfoWidget::initialize(eStorageBuilding* const stor) {
    eInfoWidget::initialize();

    eResourceType get;
    eResourceType empty;
    eResourceType accept;
    stor->getOrders(get, empty, accept);
    const auto all = stor->canAccept();
    const auto& maxCount = stor->maxCount();

    const auto rect = centralWidgetRect();

    const auto stWid = new eWidget(window());

    const auto types = eResourceTypeHelpers::extractResourceTypes(all);

    stWid->move(rect.x, rect.y);
    stWid->resize(rect.w, rect.h);

    if(types.size() < 7) {
        const auto r = new eResourceStorageStack(window());
        r->setWidth(rect.w);
        r->setHeight(rect.h);
        r->initialize(stor, types, get, empty, accept,
                      mButtons, mSpinBoxes, maxCount);
        stWid->addWidget(r);
    } else {
        {
            const auto r = new eResourceStorageStack(window());
            r->setWidth(rect.w/2);
            r->setHeight(rect.h);
            const std::vector<eResourceType> types1(
                        types.begin(), types.begin() + 6);
            r->initialize(stor, types1, get, empty, accept,
                          mButtons, mSpinBoxes, maxCount);
            stWid->addWidget(r);
        }
        {
            const auto r = new eResourceStorageStack(window());
            r->setWidth(rect.w/2);
            r->setHeight(rect.h);
            const std::vector<eResourceType> types1(
                        types.begin() + 6, types.end());
            r->initialize(stor, types1, get, empty, accept,
                          mButtons, mSpinBoxes, maxCount);
            stWid->addWidget(r);
        }
    }

    stWid->layoutHorizontally();

    addWidget(stWid);
}

void eStorageInfoWidget::get(eResourceType& get,
                             eResourceType& empty,
                             eResourceType& accept,
                             eResourceType& dontaccept,
                             std::map<eResourceType, int>& count) const {
    get = eResourceType::none;
    empty = eResourceType::none;
    accept = eResourceType::none;
    dontaccept = eResourceType::none;
    for(const auto b : mButtons) {
        const auto type = b.first;
        const int val = b.second->currentValue();
        if(val == 0) {
            dontaccept = dontaccept | type;
        } else if(val == 1) {
            accept = accept | type;
        } else if(val == 2) {
            get = get | type;
        } else if(val == 3) {
            empty = empty | type;
        }
    }
    for(const auto s : mSpinBoxes) {
        const auto type = s.first;
        const int val = s.second->value();
        count[type] = val;
    }
}
