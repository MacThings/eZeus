#include "eepisodeintroductionwidget.h"

#include "textures/egametextures.h"

#include "eframedwidget.h"
#include "engine/ecampaign.h"

#include "etextscroller.h"
#include "eproceedbutton.h"

#include "elanguage.h"
#include "emainwindow.h"
#include "eupbutton.h"
#include "edownbutton.h"
#include "egamewidget.h"
#include "eframedbutton.h"
#include "estringhelpers.h"

#include "audio/emusic.h"

void eEpisodeIntroductionWidget::initialize(
        const stdsptr<eCampaign>& c,
        const std::string& title,
        const std::string& text,
        const eGoals& goals,
        const eAction& proceedA,
        const eEpisodeIntroType type) {
    const auto res = resolution();
    const auto& intrfc = eGameTextures::interface();
    const int iRes = static_cast<int>(res.uiScale());
    const auto& texs = intrfc[iRes];
    if(type == eEpisodeIntroType::intro ||
       type == eEpisodeIntroType::campaingVictory) {
        setTexture(texs.fIntroductionImage);
    }

    const auto frame = new eFramedWidget(window());
    frame->setType(eFrameType::message);
    const int h = 2*height()/3;
    const int w = 3*h/2;
    frame->resize(w, h);

    const int p = res.hugePadding();

    const auto inner = new eWidget(window());
    inner->setNoPadding();
    const int iw = w - 2*p;
    const int ih = h - 2*p;
    inner->resize(iw, ih);
    inner->move(p, p);
    frame->addWidget(inner);

    const auto mainTitle = new eLabel(window());
    mainTitle->setNoPadding();
    mainTitle->setHugeFontSize();
    mainTitle->setText(title);
    mainTitle->fitContent();
    inner->addWidget(mainTitle);
    mainTitle->align(eAlignment::hcenter);

    const auto subTitle = new eLabel(window());
    subTitle->setNoPadding();
    if(type == eEpisodeIntroType::campaingVictory) {
        subTitle->setText(c->titleText());
    } else {
        const auto e = c->currentEpisode();
        subTitle->setText(e->fTitle);
    }
    subTitle->fitContent();
    inner->addWidget(subTitle);

    eFramedWidget* goalsFrame = nullptr;
    if(type != eEpisodeIntroType::campaingVictory) {
        goalsFrame = new eFramedWidget(window());
        goalsFrame->setType(eFrameType::inner);
        goalsFrame->setNoPadding();
        goalsFrame->setWidth(iw);

        const auto goalsInner = new eWidget(window());
        goalsInner->setNoPadding();
        goalsInner->setWidth(iw - 2*p);

        const auto e = c->currentEpisode();
        const auto et = c->currentEpisodeType();
        const auto board = e->fBoard;
        const auto ppid = board->personPlayer();
        const auto cids = board->playerCitiesOnBoard(ppid);
        const auto capitalCid = board->playerCapital(ppid);
        const auto capital = board->boardCityWithId(capitalCid);
        const bool atlantean = capital ? capital->atlantean() : false;
        for(const auto cid : cids) {
            board->updateResources(cid);
        }

        if(type == eEpisodeIntroType::goals) {
            board->checkGoalsFulfilled();
        }
        for(const auto& g : goals) {
            const auto w = new eWidget(window());
            w->setWidth(goalsInner->width());
            w->setNoPadding();

            const auto checkBox = new eLabel(window());
            checkBox->setNoPadding();
            const auto& ctexs = texs.fCheckBox;
            const int tid = g->met() ? 0 : 1;
            checkBox->setTexture(ctexs.getTexture(tid));
            checkBox->fitContent();
            w->addWidget(checkBox);

            const bool col = et == eEpisodeType::colony;
            const auto t = g->text(col, atlantean, *board);
            const auto l = new eLabel(window());
            l->setSmallFontSize();
            l->setTinyPadding();
            l->setText(t);
            l->fitContent();
            w->addWidget(l);
            w->stackHorizontally(p);
            w->fitHeight();
            checkBox->align(eAlignment::vcenter);
            l->align(eAlignment::vcenter);
            goalsInner->addWidget(w);

            if(type == eEpisodeIntroType::goals) {
                bool addStatusText = true;
                if(g->fType == eEpisodeGoalType::setAsideGoods) {
                    const auto res = static_cast<eResourceType>(g->fEnumInt1);
                    int has = 0;
                    const auto cids = board->personPlayerCitiesOnBoard();
                    for(const auto cid : cids) {
                        has += board->resourceCount(cid, res);
                    }
                    if(!g->met() && has >= g->fRequiredCount) {
                        const auto setAside = new eFramedButton(window());
                        setAside->setUnderline(false);
                        setAside->setRenderBg(true);
                        setAside->setTinyPadding();
                        setAside->setSmallFontSize();
                        setAside->setText(eLanguage::zeusText(194, 61));
                        setAside->fitContent();
                        setAside->setWidth(2*setAside->width());
                        w->addWidget(setAside);
                        setAside->align(eAlignment::vcenter | eAlignment::right);
                        setAside->setPressAction([board, res, g, setAside,
                                                  checkBox, ctexs, cids]() {
                            int rem = g->fRequiredCount;
                            for(const auto cid : cids) {
                                rem -= board->takeResource(cid, res, rem);
                                if(rem <= 0) break;
                            }
                            g->fStatusCount = g->fRequiredCount;
                            setAside->hide();
                            checkBox->setTexture(ctexs.getTexture(0));
                        });
                        addStatusText = false;
                    }
                }
                if(addStatusText) {
                    const auto st = g->statusText(*board);
                    if(!st.empty()) {
                        const auto stl = new eLabel(window());
                        stl->setSmallFontSize();
                        stl->setTinyPadding();
                        stl->setText(st);
                        stl->fitContent();
                        w->addWidget(stl);
                        stl->align(eAlignment::vcenter | eAlignment::right);
                    }
                }
            }
        }

        goalsInner->stackVertically();
        goalsInner->fitHeight();
        goalsInner->move(p, p);
        goalsFrame->addWidget(goalsInner);
        goalsFrame->setHeight(goalsInner->height() + 2*p);
        inner->addWidget(goalsFrame);
    }

    const auto lowerButtons = new eWidget(window());
    lowerButtons->setNoPadding();
    lowerButtons->setWidth(iw);

    if(type == eEpisodeIntroType::intro) {
        const auto diffW = new eWidget(window());
        diffW->setNoPadding();

        const auto diffLabel = new eLabel(window());
        diffLabel->setNoPadding();
        diffLabel->setSmallFontSize();
        const auto diffText = eLanguage::zeusText(44, 219);
        const auto defDiff = eDifficulty::hero;
        const auto diff = std::make_shared<eDifficulty>(defDiff);
        c->setDifficulty(defDiff);
        const auto hdiff = eDifficultyHelpers::name(*diff);
        diffLabel->setText("  " + diffText + "  " + hdiff);
        diffLabel->fitContent();

        const auto up = new eUpButton(window());
        diffW->addWidget(up);
        up->setPressAction([diff, diffLabel, diffText, c]() {
            if(*diff == eDifficulty::olympian) return;
            const int diffi = static_cast<int>(*diff);
            *diff = static_cast<eDifficulty>(diffi + 1);
            const auto hdiff = eDifficultyHelpers::name(*diff);
            diffLabel->setText("  " + diffText + "  " + hdiff);
            diffLabel->fitContent();
            c->setDifficulty(*diff);
        });

        const auto down = new eDownButton(window());
        diffW->addWidget(down);
        down->setPressAction([diff, diffLabel, diffText, c]() {
            if(*diff == eDifficulty::beginner) return;
            const int diffi = static_cast<int>(*diff);
            *diff = static_cast<eDifficulty>(diffi - 1);
            const auto hdiff = eDifficultyHelpers::name(*diff);
            diffLabel->setText("  " + diffText + " " + hdiff);
            diffLabel->fitContent();
            c->setDifficulty(*diff);
        });

        diffW->addWidget(diffLabel);

        diffW->stackHorizontally();
        diffW->fitContent();
        lowerButtons->addWidget(diffW);

        const auto proceedW = new eWidget(window());
        proceedW->setNoPadding();

        const auto proceedLabel = new eLabel(window());
        proceedLabel->setSmallFontSize();
        proceedLabel->setSmallPadding();
        proceedLabel->setText(eLanguage::zeusText(62, 5));
        proceedLabel->fitContent();
        proceedW->addWidget(proceedLabel);

        const auto proceedB = new eProceedButton(window());
        proceedB->setPressAction(proceedA);
        proceedW->addWidget(proceedB);

        proceedW->stackHorizontally(2*p);
        proceedW->fitContent();
        proceedLabel->align(eAlignment::vcenter);
        proceedB->align(eAlignment::vcenter);

        lowerButtons->addWidget(proceedW);
        proceedW->align(eAlignment::right);
    } else if(type == eEpisodeIntroType::goals) {
        const auto rpc = new eLabel(window());
        rpc->setNoPadding();
        rpc->setSmallFontSize();
        rpc->setText(eLanguage::zeusText(13, 1));
        rpc->fitContent();
        lowerButtons->addWidget(rpc);
        rpc->align(eAlignment::hcenter);
    } else if(type == eEpisodeIntroType::victory ||
              type == eEpisodeIntroType::campaingVictory) {
        const auto p = new eFramedButton(window());
        p->setUnderline(false);
        p->setText(eLanguage::zeusText(62, 2));
        p->fitContent();
        p->setPressAction(proceedA);
        lowerButtons->addWidget(p);
        p->align(eAlignment::hcenter);
    }

    lowerButtons->fitHeight();

    const auto introText = new eTextScroller(window());
    introText->setWidth(iw);
    int ith = ih - 4*p;
    ith -= mainTitle->height();
    ith -= subTitle->height();
    ith -= goalsFrame ? goalsFrame->height() : 0;
    ith -= lowerButtons->height();
    introText->setHeight(ith);
    introText->initialize();
    introText->setSmallTextFontSize();
    introText->setTinyTextPadding();
    auto textPrep = text;
    eStringHelpers::replaceSpecial(textPrep);
    introText->setText(textPrep);
    inner->addWidget(introText);

    inner->addWidget(lowerButtons);

    inner->stackVertically(p);

    addWidget(frame);
    frame->align(eAlignment::center);
}
