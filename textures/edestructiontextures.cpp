#include "edestructiontextures.h"

#include "etextureloadinghelpers.h"

#include "etextureloader.h"

eDestructionTextures::eDestructionTextures(const int tileW, const int tileH,
                                           SDL_Renderer* const renderer) :
      fTileW(tileW), fTileH(tileH),
      fRenderer(renderer) {

}

void eDestructionTextures::load() {
    eTextureLoader texLoader(fRenderer);
    texLoader.initialize();

    std::string dir{"../ZeusTextures/"};
    dir += std::to_string(fTileH) + "/";
    dir += "destruction/";
    const std::string pathBase{dir + "destruction_"};
    eTextureClass texClass(pathBase, texLoader);

    for(int i = 0; i < 3; i++) {
        fFire.emplace_back(fRenderer);
    }

    for(int i = 37; i < 63; i++) {
        texClass.load(i, fFire[0]);
    }
    for(int i = 63; i < 89; i++) {
        texClass.load(i, fFire[1]);
    }
//    for(int i = 89; i < 115; i++) {
//        eTextureLoadingHelpers::loadTex(pathBase, i, fFire[2]);
//    }
    for(int i = 115; i < 133; i++) {
        texClass.load(i, fFire[2]);
    }
//    for(int i = 133; i < 157; i++) {
//        eTextureLoadingHelpers::loadTex(pathBase, i, fFire[4]);
//    }
//    for(int i = 1007; i < 1033; i++) {
//        eTextureLoadingHelpers::loadTex(pathBase, i, fFire[4]);
//    }

//    for(int i = 0; i < 2; i++) {
//        fBigFire.emplace_back(fRenderer);
//    }

//    for(int i = 1033; i < 1059; i++) {
//        eTextureLoadingHelpers::loadTex(pathBase, i, fBigFire[0]);
//    }
//    for(int i = 1059; i < 1085; i++) {
//        eTextureLoadingHelpers::loadTex(pathBase, i, fBigFire[1]);
//    }
    texLoader.waitUntilFinished();
}
