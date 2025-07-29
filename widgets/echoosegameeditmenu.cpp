#include "echoosegameeditmenu.h"

#include "eframedwidget.h"
#include "eframedbutton.h"
#include "escrollwidgetcomplete.h"
#include "ebuttonbase.h"
#include "ecancelbutton.h"
#include "emainwindow.h"
#include "eproceedbutton.h"
#include "elineedit.h"
#include "equestionwidget.h"
#include "eeditormainmenu.h"
#include "ebitmapwidget.h"
#include "eepisodeintroductionwidget.h"
#include "estringhelpers.h"
#include "elanguage.h"
#include "egamedir.h"
#include "evectorhelpers.h"

#include <filesystem>
namespace fs = std::filesystem;

#include "pak/gamefile.h"

int pakBitmapIdConvert(const int id) {
    if(id == 1) return 0;
    else if(id == 2) return 6;
    else if(id == 3) return 1;
    else if(id == 4) return 11;
    else if(id == 5) return 9;
    else if(id == 6) return 10;
    else if(id == 7) return 3;
    else if(id == 8) return 0;
    else if(id == 9) return 12;
    else if(id == 10) return 13;
    else if(id == 11) return 14;
    else if(id == 12) return 15;
    else if(id == 13) return 16;
    else if(id == 14) return 4;
    return 0;
}

bool readPakGlossary(const std::string& filename,
                     eCampaignGlossary& glossary) {
    glossary.fIsPak = true;
    const auto name = eStringHelpers::pathToName(filename);
    const bool test = name.find("Test") != name.npos;
    if(test && name != "Test6.pak") return false;
    const auto ext = name.substr(name.size() - 3);
    if(ext != "pak") return false;
    const auto txtFile = filename.substr(0, filename.size() - 3) + "txt";
    glossary.fPakPath = filename;
    std::ifstream file(txtFile);
    GameFile in(filename);
    in.seek(4);
    const bool newVersion = in.readUByte() == 0x1a;
    in.seek(7788);
    const bool atlantean = in.readUByte();
    uint8_t bitmapId;
    if(newVersion) {
        in.seek(836249);
    } else {
        in.seek(835185);
    }
    bitmapId = in.readUByte();
    glossary.fBitmap = pakBitmapIdConvert(bitmapId);
    if(file.good()) {
        std::map<std::string, std::string> map;
        const bool r = eCampaign::sLoadStrings(txtFile, map);
        if(!r) return false;
        glossary.fTitle = map["Adventure_Title"];
        glossary.fIntroduction = map["Adventure_Introduction"];
        glossary.fComplete = map["Adventure_Complete"];
    } else {
        const std::vector<char> special = {'@', '[', '&', ']',
                                           '{', '}', '^', '#'};
        bool found = false;
        for(const auto c : special) {
            const auto pos = std::find(name.begin(), name.end(), c);
            if(pos != name.end()) {
                found = true;
                break;
            }
        }
        if(!found) return false;
        in.seek(35648);
        const auto briefId = in.readUShort();

        const auto brief = eLanguage::zeusMM(briefId);
        glossary.fTitle = brief.first;
        glossary.fIntroduction = brief.second;
    }
    return true;
}

void eChooseGameEditMenu::initialize(const bool editor) {
    eMainMenuBase::initialize();

    std::vector<eCampaignGlossary> glossaries;
    {
        const auto folder = eGameDir::adventuresDir();
        std::filesystem::create_directories(folder);
        for(const auto& entry : fs::directory_iterator(folder)) {
            const bool dir = entry.is_directory();
            if(!dir) continue;
            const auto path = entry.path();
            const std::string pathStr = path.u8string();
            const auto name = eStringHelpers::pathToName(pathStr);
            eCampaignGlossary glossary;
            const bool r = eCampaign::sReadGlossary(name, glossary);
            if(r) glossaries.push_back(glossary);
        }
    }
    {
        std::function<void(std::string)> procesFolder;
        procesFolder = [&](const std::string& folder) {
            for(const auto& entry : fs::directory_iterator(folder)) {
                const bool dir = entry.is_directory();
                const auto path = entry.path();
                if(dir) {
                    procesFolder(path);
                    continue;
                }
                const std::string pathStr = path.u8string();
                const auto ext = pathStr.substr(pathStr.size() - 3);
                if(ext != "pak") continue;
                eCampaignGlossary glossary;
                const bool r = readPakGlossary(pathStr, glossary);
                if(r) glossaries.push_back(glossary);
            }
        };
        const auto folder = eGameDir::pakAdventuresDir();
        procesFolder(folder);
    }

    const auto iww = new eWidget(window());
    iww->setNoPadding();

    const auto res = resolution();
    const double mult = res.multiplier();
    const auto chooseW = new eFramedWidget(window());
    chooseW->setType(eFrameType::message);
    const int p = res.largePadding();
    const int cw = 400*mult;
    const int ch = 400*mult;
    chooseW->resize(cw, ch);
    iww->addWidget(chooseW);

    const auto iw = new eWidget(window());
    iw->setNoPadding();
    iw->resize(cw - 4*p, ch - 4*p);
    chooseW->addWidget(iw);
    iw->move(2*p, 2*p);

    const auto title = new eLabel(window());
    title->setHugeFontSize();
    title->setSmallPadding();
    title->setText(editor ? eLanguage::zeusText(287, 3) :
                            eLanguage::zeusText(293, 9));
    title->fitContent();
    iw->addWidget(title);
    title->align(eAlignment::hcenter);

    const auto scrollCont = new eScrollWidgetComplete(window());
    iw->addWidget(scrollCont);
    scrollCont->resize(cw - 4*p, ch/2);
    scrollCont->initialize();

    if(editor) {
        const auto buttonsW = new eWidget(window());
        buttonsW->setNoPadding();

        const auto newB = new eFramedButton(window());
        newB->setUnderline(false);
        newB->setSmallFontSize();
        newB->setSmallPadding();
        newB->setText(eLanguage::zeusText(287, 0));
        newB->fitContent();
        buttonsW->addWidget(newB);
        newB->setPressAction([cw, p, this]() {
            const auto box = new eFramedWidget(window());
            box->setType(eFrameType::message);
            box->setWidth(cw);

            const auto iw = new eWidget(window());
            iw->setNoPadding();
            iw->setWidth(cw - 4*p);

            const auto title = new eLabel(window());
            title->setHugeFontSize();
            title->setSmallPadding();
            title->setText(eLanguage::zeusText(287, 0));
            title->fitContent();
            iw->addWidget(title);
            title->align(eAlignment::hcenter);

            const auto edit = new eLineEdit(window());
            edit->setRenderBg(true);
            edit->setText(eLanguage::zeusText(287, 0));
            edit->fitContent();
            edit->setText("");
            edit->setWidth(cw - 6*p);
            iw->addWidget(edit);
            edit->align(eAlignment::hcenter);

            const auto buttonsW = new eWidget(window());
            buttonsW->setNoPadding();
            buttonsW->setWidth(iw->width());

            const auto cButton = new eCancelButton(window());
            buttonsW->addWidget(cButton);
            cButton->setPressAction([this]() {
                window()->showChooseGameEditMenu();
            });

            const auto proceedW = new eWidget(window());
            proceedW->setNoPadding();

            const auto proceedLabel = new eLabel(window());
            proceedLabel->setSmallFontSize();
            proceedLabel->setSmallPadding();
            proceedLabel->setText(eLanguage::zeusText(287, 2));
            proceedLabel->fitContent();
            proceedW->addWidget(proceedLabel);

            const auto proceedB = new eProceedButton(window());
            proceedB->setPressAction([this, edit]() {
                auto name = edit->text();
                std::string trimmedName;
                while(!name.empty() && name[0] == ' ') {
                    name.erase(name.begin());
                }
                while(!name.empty() && name[name.size() - 1] == ' ') {
                    name.pop_back();
                }
                if(name.empty()) return;
                eCampaign c;
                c.initialize(name);
                c.save();
                window()->showChooseGameEditMenu();
            });
            proceedW->addWidget(proceedB);

            proceedW->stackHorizontally(2*p);
            proceedW->fitContent();
            proceedLabel->align(eAlignment::vcenter);
            proceedB->align(eAlignment::vcenter);
            buttonsW->addWidget(proceedW);

            buttonsW->layoutHorizontallyWithoutSpaces();
            buttonsW->fitHeight();

            iw->addWidget(buttonsW);

            iw->stackVertically(2*p);
            iw->fitHeight();
            box->addWidget(iw);
            iw->move(2*p, 2*p);
            box->setHeight(iw->height() + 4*p);

            window()->execDialog(box);
            box->align(eAlignment::center);

            edit->grabKeyboard();
        });

        const auto deleteB = new eFramedButton(window());
        deleteB->setUnderline(false);
        deleteB->setSmallFontSize();
        deleteB->setSmallPadding();
        deleteB->setText(eLanguage::zeusText(287, 1));
        deleteB->fitContent();
        buttonsW->addWidget(deleteB);
        deleteB->setPressAction([this]() {
           const auto q = new eQuestionWidget(window());
           const auto acceptA = [this]() {
               const auto dir = eGameDir::adventuresDir() + mSelected.fFolderName + "/";
               std::filesystem::remove_all(dir);
               window()->showChooseGameEditMenu();
           };
           q->initialize(eLanguage::zeusText(5, 179),
                         eLanguage::zeusText(5, 180),
                         acceptA, nullptr);
           window()->execDialog(q);
           q->align(eAlignment::center);
        });

        buttonsW->setWidth(iw->width());
        buttonsW->layoutHorizontallyWithoutSpaces();
        buttonsW->fitHeight();
        iw->addWidget(buttonsW);
    }

    const auto buttons2W = new eWidget(window());
    buttons2W->setNoPadding();

    const auto cButton = new eCancelButton(window());
    buttons2W->addWidget(cButton);
    cButton->setPressAction([this]() {
        window()->showMainMenu();
    });

    const auto proceedW = new eWidget(window());
    proceedW->setNoPadding();

    const auto proceedLabel = new eLabel(window());
    proceedLabel->setSmallFontSize();
    proceedLabel->setSmallPadding();
    proceedLabel->setText(editor ? eLanguage::zeusText(287, 2) :
                                   eLanguage::zeusText(287, 6));
    proceedLabel->fitContent();
    proceedW->addWidget(proceedLabel);

    const auto proceedB = new eProceedButton(window());
    proceedB->setPressAction([this, editor]() {
        if(!mSelected.fIsPak && mSelected.fFolderName.empty()) return;
        if(mSelected.fIsPak && mSelected.fPakPath.empty()) return;
        const auto w = window();

        if(editor) {
            const auto e = new eEditorMainMenu(window());
            e->resize(w->width(), w->height());
            const auto c = std::make_shared<eCampaign>();
            if(mSelected.fIsPak) c->readPak(mSelected.fPakPath);
            else c->load(mSelected.fFolderName);
            c->setEditorMode(true);
            e->initialize(c);
            w->setWidget(e);
        } else {
            const auto c = std::make_shared<eCampaign>();
            if(mSelected.fIsPak) c->readPak(mSelected.fPakPath);
            else c->load(mSelected.fFolderName);
            w->showEpisodeIntroduction(c);
        }
    });
    proceedW->addWidget(proceedB);

    proceedW->stackHorizontally(2*p);
    proceedW->fitContent();
    proceedLabel->align(eAlignment::vcenter);
    proceedB->align(eAlignment::vcenter);
    buttons2W->addWidget(proceedW);

    buttons2W->setWidth(iw->width());
    buttons2W->layoutHorizontallyWithoutSpaces();
    buttons2W->fitHeight();
    iw->addWidget(buttons2W);

    iw->layoutVerticallyWithoutSpaces();

    const auto sideW = new eWidget(window());
    sideW->setNoPadding();

    const auto bitmapWW = new eFramedWidget(window());
    bitmapWW->setType(eFrameType::message);
    mBitmap = new eBitmapWidget(window());
    mBitmap->setNoPadding();
    mBitmap->fitContent();
    bitmapWW->resize(mBitmap->width() + 2*p, mBitmap->height() + 2*p);
    bitmapWW->addWidget(mBitmap);
    mBitmap->move(p, p);
    sideW->addWidget(bitmapWW);

    const auto descWW = new eFramedWidget(window());
    descWW->setType(eFrameType::message);
    descWW->resize(bitmapWW->width(), 2*bitmapWW->height()/3);

    const auto descW = new eFramedWidget(window());
    descW->setType(eFrameType::inner);
    descW->resize(descWW->width() - 2*p, descWW->height() - 2*p);
    descWW->addWidget(descW);
    descW->move(p, p);

    const auto descIW = new eWidget(window());
    descIW->resize(descW->width() - 2*p, descW->height() - 2*p);
    descW->addWidget(descIW);
    descIW->move(p, p);

    mTitle = new eLabel(window());
    mTitle->setSmallFontSize();
    mTitle->setNoPadding();
    mTitle->setText("Height");
    mTitle->fitContent();
    mTitle->setText("");
    mTitle->setYellowFontColor();
    mTitle->align(eAlignment::hcenter);

    mDesc = new eLabel(window());
    mDesc->setWrapWidth(descIW->width());
    mDesc->setSmallFontSize();
    mDesc->setNoPadding();

    descIW->addWidget(mTitle);
    descIW->addWidget(mDesc);
    descIW->stackVertically(p);

    sideW->addWidget(descWW);

    sideW->stackVertically(p);
    sideW->fitContent();
    iww->addWidget(sideW);

    iww->stackHorizontally(p);
    iww->fitContent();
    addWidget(iww);
    iww->align(eAlignment::center);

    {
        const auto scrollArea = new eWidget(window());

        const auto selected = std::make_shared<eButtonBase*>(nullptr);
        bool first = true;
        for(const auto& g : glossaries) {
            const auto w = new eButtonBase(window());
            w->setTinyFontSize();
            w->setNoPadding();
            w->setText(g.fTitle);
            w->fitContent();
            w->setMouseEnterAction([w]() {
                w->setYellowFontColor();
            });
            w->setMouseLeaveAction([w, selected]() {
                const auto s = *selected;
                if(s != w) w->setLightFontColor();
            });
            w->setPressAction([selected, w, g, this]() {
                const auto s = *selected;
                if(s) s->setLightFontColor();
                setGlossary(g);
                w->setYellowFontColor();
                *selected = w;
            });
            if(first) {
                setGlossary(g);
                w->setYellowFontColor();
                *selected = w;
                first = false;
            }
            scrollArea->addWidget(w);
        }

        scrollArea->stackVertically();
        scrollArea->setNoPadding();
        scrollArea->fitContent();
        scrollCont->setScrollArea(scrollArea);
    }
}

void eChooseGameEditMenu::setGlossary(const eCampaignGlossary& g) {
    mSelected = g;
    mBitmap->setBitmap(g.fBitmap);
    mTitle->setText(g.fTitle);
    mTitle->fitContent();
    mTitle->align(eAlignment::hcenter);
    mDesc->setText(g.fIntroduction);
    mDesc->fitContent();
}
