TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CFLAGS_RELEASE += -m64 -O3
QMAKE_CXXFLAGS_RELEASE += -m64 -O3

LIBS += -L/usr/lib/x86_64-linux-gnu
LIBS += -lpthread -lSDL2 -lSDL2_image -lSDL2_ttf

SOURCES += \
        buildings/ebuilding.cpp \
        buildings/ecollege.cpp \
        buildings/edramaschool.cpp \
        buildings/efoundry.cpp \
        buildings/efountain.cpp \
        buildings/egymnasium.cpp \
        buildings/ehospital.cpp \
        buildings/ehuntinglodge.cpp \
        buildings/emaintenanceoffice.cpp \
        buildings/emint.cpp \
        buildings/epalace1.cpp \
        buildings/epalace2.cpp \
        buildings/epatrolbuilding.cpp \
        buildings/epodium.cpp \
        buildings/eresourcebuilding.cpp \
        buildings/eresourcecollectbuilding.cpp \
        buildings/eroad.cpp \
        buildings/esmallhouse.cpp \
        buildings/estadium1.cpp \
        buildings/estadium2.cpp \
        buildings/etaxoffice.cpp \
        buildings/etheater.cpp \
        buildings/etimbermill.cpp \
        buildings/ewatchpost.cpp \
        characters/actions/eactionwithcomeback.cpp \
        characters/actions/echaracteraction.cpp \
        characters/actions/ecollectaction.cpp \
        characters/actions/ecollectresourceaction.cpp \
        characters/actions/edieaction.cpp \
        characters/actions/efightaction.cpp \
        characters/actions/efirefighteraction.cpp \
        characters/actions/ehuntaction.cpp \
        characters/actions/emoveaction.cpp \
        characters/actions/emovearoundaction.cpp \
        characters/actions/emovepathaction.cpp \
        characters/actions/epatrolaction.cpp \
        characters/actions/epatrolmoveaction.cpp \
        characters/actions/ewaitaction.cpp \
        characters/eanimal.cpp \
        characters/ebasicpatroler.cpp \
        characters/eboar.cpp \
        characters/ebronzeminer.cpp \
        characters/echaracter.cpp \
        characters/echaracterbase.cpp \
        characters/edemeter.cpp \
        characters/efightingpatroler.cpp \
        characters/efirefighter.cpp \
        characters/egymnast.cpp \
        characters/ehealer.cpp \
        characters/ehunter.cpp \
        characters/elumberjack.cpp \
        characters/ephilosopher.cpp \
        characters/eresourcecollector.cpp \
        characters/esilverminer.cpp \
        characters/etaxcollector.cpp \
        characters/ewatchman.cpp \
        characters/ewaterdistributor.cpp \
        emainwindow.cpp \
        engine/efunctask.cpp \
        engine/egameboard.cpp \
        engine/egameboarditerator.cpp \
        engine/egameobject.cpp \
        engine/emovinggameobject.cpp \
        engine/epathfinder.cpp \
        engine/etask.cpp \
        engine/eterrain.cpp \
        engine/ethreadpool.cpp \
        engine/etile.cpp \
        engine/etilebase.cpp \
        engine/thread/ethreadboard.cpp \
        engine/thread/ethreadbuilding.cpp \
        engine/thread/ethreadcharacter.cpp \
        engine/thread/ethreaddata.cpp \
        engine/thread/ethreadtile.cpp \
        epathfindtask.cpp \
        espriteframe.cpp \
        esprites.cpp \
        etexture.cpp \
        etexturecollection.cpp \
        main.cpp \
        spawners/eboarspawner.cpp \
        spawners/espawner.cpp \
        textures/ebeachtodry.cpp \
        textures/ebuildingtextures.cpp \
        textures/echaractertextures.cpp \
        textures/edemetertextures.cpp \
        textures/efertiletodry.cpp \
        textures/eforesttodry.cpp \
        textures/egametextures.cpp \
        textures/einterfacetextures.cpp \
        textures/estonestodry.cpp \
        textures/esurroundingterrain.cpp \
        textures/eterraintextures.cpp \
        textures/etextureloadinghelpers.cpp \
        textures/etiletotexture.cpp \
        textures/ewatercorner.cpp \
        textures/ewatertobeach.cpp \
        textures/ewatertodry.cpp \
        textures/ewatertox.cpp \
        widgets/eactionlistwidget.cpp \
        widgets/ebutton.cpp \
        widgets/echeckablebutton.cpp \
        widgets/echeckbox.cpp \
        widgets/ecombobox.cpp \
        widgets/econtextmenu.cpp \
        widgets/efonts.cpp \
        widgets/eframedlabel.cpp \
        widgets/eframedwidget.cpp \
        widgets/egameloadingwidget.cpp \
        widgets/egamemenu.cpp \
        widgets/egamemenubase.cpp \
        widgets/egamewidget.cpp \
        widgets/elabel.cpp \
        widgets/elabelbase.cpp \
        widgets/elabeledwidget.cpp \
        widgets/eloadingwidget.cpp \
        widgets/emainmenu.cpp \
        widgets/emenuloadingwidget.cpp \
        widgets/emouseevent.cpp \
        widgets/epainter.cpp \
        widgets/eprogressbar.cpp \
        widgets/eresolution.cpp \
        widgets/esettingsmenu.cpp \
        widgets/etabwidget.cpp \
        widgets/eterraineditmenu.cpp \
        widgets/etilepainter.cpp \
        widgets/ewidget.cpp \
        widgets/infowidgets/egyminfowidget.cpp \
        widgets/infowidgets/einfowidget.cpp

HEADERS += \
    buildings/ebuilding.h \
    buildings/ecollege.h \
    buildings/edramaschool.h \
    buildings/efoundry.h \
    buildings/efountain.h \
    buildings/egymnasium.h \
    buildings/ehospital.h \
    buildings/ehuntinglodge.h \
    buildings/emaintenanceoffice.h \
    buildings/emint.h \
    buildings/epalace1.h \
    buildings/epalace2.h \
    buildings/epatrolbuilding.h \
    buildings/epodium.h \
    buildings/eresourcebuilding.h \
    buildings/eresourcecollectbuilding.h \
    buildings/eroad.h \
    buildings/esmallhouse.h \
    buildings/estadium1.h \
    buildings/estadium2.h \
    buildings/etaxoffice.h \
    buildings/etheater.h \
    buildings/etimbermill.h \
    buildings/ewatchpost.h \
    characters/actions/eactionwithcomeback.h \
    characters/actions/echaracteraction.h \
    characters/actions/ecollectaction.h \
    characters/actions/ecollectresourceaction.h \
    characters/actions/edieaction.h \
    characters/actions/efightaction.h \
    characters/actions/efirefighteraction.h \
    characters/actions/ehuntaction.h \
    characters/actions/emoveaction.h \
    characters/actions/emovearoundaction.h \
    characters/actions/emovepathaction.h \
    characters/actions/epatrolaction.h \
    characters/actions/epatrolmoveaction.h \
    characters/actions/ewaitaction.h \
    characters/eanimal.h \
    characters/ebasicpatroler.h \
    characters/eboar.h \
    characters/ebronzeminer.h \
    characters/echaracter.h \
    characters/echaracterbase.h \
    characters/edemeter.h \
    characters/efightingpatroler.h \
    characters/efirefighter.h \
    characters/egymnast.h \
    characters/ehealer.h \
    characters/ehunter.h \
    characters/elumberjack.h \
    characters/ephilosopher.h \
    characters/eresourcecollector.h \
    characters/esilverminer.h \
    characters/etaxcollector.h \
    characters/ewatchman.h \
    characters/ewaterdistributor.h \
    emainwindow.h \
    engine/efunctask.h \
    engine/egameboard.h \
    engine/egameboarditerator.h \
    engine/egameobject.h \
    engine/emovedirection.h \
    engine/emovinggameobject.h \
    engine/eorientation.h \
    engine/epathfinder.h \
    engine/etask.h \
    engine/eterrain.h \
    engine/ethreadpool.h \
    engine/etile.h \
    engine/etilebase.h \
    engine/thread/ethreadboard.h \
    engine/thread/ethreadbuilding.h \
    engine/thread/ethreadcharacter.h \
    engine/thread/ethreaddata.h \
    engine/thread/ethreadtile.h \
    epathfindtask.h \
    espriteframe.h \
    esprites.h \
    etexture.h \
    etexturecollection.h \
    offsets/SprMain.h \
    offsets/Zeus_Demeter.h \
    spawners/eboarspawner.h \
    spawners/espawner.h \
    textures/ebeachtodry.h \
    textures/ebuildingtextures.h \
    textures/echaractertextures.h \
    textures/edemetertextures.h \
    textures/efertiletodry.h \
    textures/eforesttodry.h \
    textures/egametextures.h \
    textures/einterfacetextures.h \
    textures/estonestodry.h \
    textures/esurroundingterrain.h \
    textures/eterraintextures.h \
    textures/etextureloadinghelpers.h \
    textures/etiletotexture.h \
    textures/ewatercorner.h \
    textures/ewatertobeach.h \
    textures/ewatertodry.h \
    textures/ewatertox.h \
    widgets/eactionlistwidget.h \
    widgets/ebutton.h \
    widgets/echeckablebutton.h \
    widgets/echeckbox.h \
    widgets/ecombobox.h \
    widgets/econtextmenu.h \
    widgets/efonts.h \
    widgets/eframedlabel.h \
    widgets/eframedwidget.h \
    widgets/egameloadingwidget.h \
    widgets/egamemenu.h \
    widgets/egamemenubase.h \
    widgets/egamewidget.h \
    widgets/elabel.h \
    widgets/elabelbase.h \
    widgets/elabeledwidget.h \
    widgets/eloadingwidget.h \
    widgets/emainmenu.h \
    widgets/emenuloadingwidget.h \
    widgets/emouseevent.h \
    widgets/epainter.h \
    widgets/eprogressbar.h \
    widgets/eresolution.h \
    widgets/esettingsmenu.h \
    widgets/etabwidget.h \
    widgets/eterraineditmenu.h \
    widgets/etilepainter.h \
    widgets/ewidget.h \
    widgets/infowidgets/egyminfowidget.h \
    widgets/infowidgets/einfowidget.h
