﻿#ifndef EMAINWINDOW_H
#define EMAINWINDOW_H

#include "widgets/ewidget.h"
#include "widgets/eresolution.h"
#include "textures/eterraintextures.h"
#include "textures/egodtextures.h"
#include "textures/ebuildingtextures.h"
#include "textures/echaractertextures.h"
#include "widgets/esettingsmenu.h"

using eSlot = std::function<void()>;

class eMainWindow {
public:
    eMainWindow();
    ~eMainWindow();

    bool initialize(const eResolution& res);
public:

    void setWidget(eWidget* const w);

    int exec();

    void addSlot(const eSlot& slot);

    int width() const { return resolution().width(); }
    int height() const { return resolution().height(); }
    eResolution resolution() const { return mSettings.fRes; }
    SDL_Renderer* renderer() const { return mSdlRenderer; }

    void setResolution(const eResolution res);
    void setFullscreen(const bool f);

    void showMenuLoading();
    void showMainMenu();
    void showSettingsMenu();
    void showGame();

    const eSettings& settings() const { return mSettings; }
private:
    eSettings mSettings{false, false, false, eResolution()};

    bool mQuit = false;

    std::vector<eSlot> mSlots;

    eWidget* mWidget = nullptr;
    SDL_Window* mSdlWindow = nullptr;
    SDL_Renderer* mSdlRenderer = nullptr;
};

#endif // EMAINWINDOW_H
