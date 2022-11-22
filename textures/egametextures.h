﻿#ifndef EGAMETEXTURES_H
#define EGAMETEXTURES_H

#include "eterraintextures.h"
#include "egodtextures.h"
#include "ebuildingtextures.h"
#include "echaractertextures.h"
#include "einterfacetextures.h"
#include "edestructiontextures.h"

#include "esettings.h"

class eGameTextures {
public:
    static const std::vector<eTerrainTextures>& terrain() {
        return sTerrainTextures;
    }
    static const std::vector<eGodTextures>& gods() {
        return sGodTextures;
    }
    static const std::vector<eBuildingTextures>& buildings() {
        return sBuildingTextures;
    }
    static const std::vector<eCharacterTextures>& characters() {
        return sCharacterTextures;
    }
    static const std::vector<eInterfaceTextures>& interface() {
        return sInterfaceTextures;
    }
    static const std::vector<eDestructionTextures>& destrution() {
        return sDestructionTextures;
    }

    static void loadTexture(const std::function<void(int)>& func);

    static void loadPeddler();

    static bool initialize(SDL_Renderer* const r);
    static bool loadNextMenu(const eSettings& settings,
                             std::string& text);
    static bool loadNextGame(const eSettings& settings,
                             std::string& text);
    static int gameSize(const eSettings& settings);
    static int menuSize();

    static void setSettings(const eSettings& s);
private:
    static eSettings sSettings;
    static bool sInitialized;
    static std::vector<eTerrainTextures> sTerrainTextures;
    static std::vector<eGodTextures> sGodTextures;
    static std::vector<eBuildingTextures> sBuildingTextures;
    static std::vector<eCharacterTextures> sCharacterTextures;
    static std::vector<eInterfaceTextures> sInterfaceTextures;
    static std::vector<eDestructionTextures> sDestructionTextures;
};

#endif // EGAMETEXTURES_H
