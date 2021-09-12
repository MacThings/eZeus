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
LIBS += -lpthread
LIBS += -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
LIBS += -lnoise

SOURCES += \
        buildings/eaestheticsbuilding.cpp \
        buildings/eanimalbuilding.cpp \
        buildings/eappeal.cpp \
        buildings/earmory.cpp \
        buildings/earmsvendor.cpp \
        buildings/ebuilding.cpp \
        buildings/ebuildingwithresource.cpp \
        buildings/ecardingshed.cpp \
        buildings/ecarrotfarm.cpp \
        buildings/ecollege.cpp \
        buildings/ecolumn.cpp \
        buildings/edairy.cpp \
        buildings/edramaschool.cpp \
        buildings/eemployingbuilding.cpp \
        buildings/efarmbase.cpp \
        buildings/efleecevendor.cpp \
        buildings/efoodvendor.cpp \
        buildings/efoundry.cpp \
        buildings/efountain.cpp \
        buildings/egranary.cpp \
        buildings/egrowerslodge.cpp \
        buildings/egymnasium.cpp \
        buildings/ehorsevendor.cpp \
        buildings/ehospital.cpp \
        buildings/ehuntinglodge.cpp \
        buildings/emaintenanceoffice.cpp \
        buildings/emint.cpp \
        buildings/eoilvendor.cpp \
        buildings/eolivepress.cpp \
        buildings/eonionfarm.cpp \
        buildings/epalace1.cpp \
        buildings/epalace2.cpp \
        buildings/epark.cpp \
        buildings/epatrolbuilding.cpp \
        buildings/epatroltarget.cpp \
        buildings/epodium.cpp \
        buildings/eprocessingbuilding.cpp \
        buildings/eresourcebuilding.cpp \
        buildings/eresourcebuildingbase.cpp \
        buildings/eresourcecollectbuilding.cpp \
        buildings/eroad.cpp \
        buildings/eruins.cpp \
        buildings/esculpturestudio.cpp \
        buildings/eshepherbuildingbase.cpp \
        buildings/esmallhouse.cpp \
        buildings/estadium1.cpp \
        buildings/estadium2.cpp \
        buildings/estoragebuilding.cpp \
        buildings/etaxoffice.cpp \
        buildings/etheater.cpp \
        buildings/etimbermill.cpp \
        buildings/evendor.cpp \
        buildings/ewarehouse.cpp \
        buildings/ewatchpost.cpp \
        buildings/ewheatfarm.cpp \
        buildings/ewinery.cpp \
        buildings/ewinevendor.cpp \
        characters/actions/eactionwithcomeback.cpp \
        characters/actions/eanimalaction.cpp \
        characters/actions/ecarttransporteraction.cpp \
        characters/actions/echaracteraction.cpp \
        characters/actions/ecollectaction.cpp \
        characters/actions/ecollectresourceaction.cpp \
        characters/actions/ecomplexaction.cpp \
        characters/actions/edieaction.cpp \
        characters/actions/efightaction.cpp \
        characters/actions/efirefighteraction.cpp \
        characters/actions/egodvisitaction.cpp \
        characters/actions/egroweraction.cpp \
        characters/actions/ehuntaction.cpp \
        characters/actions/emoveaction.cpp \
        characters/actions/emovearoundaction.cpp \
        characters/actions/emovepathaction.cpp \
        characters/actions/epatrolaction.cpp \
        characters/actions/epatrolguidedmoveaction.cpp \
        characters/actions/epatrolmoveaction.cpp \
        characters/actions/esettleraction.cpp \
        characters/actions/eshepherdaction.cpp \
        characters/actions/ewaitaction.cpp \
        characters/eactor.cpp \
        characters/eanimal.cpp \
        characters/earmsvendorchar.cpp \
        characters/ebasicpatroler.cpp \
        characters/eboar.cpp \
        characters/ebronzeminer.cpp \
        characters/ecarttransporter.cpp \
        characters/echaracter.cpp \
        characters/echaracterbase.cpp \
        characters/edeer.cpp \
        characters/edomesticatedanimal.cpp \
        characters/efightingpatroler.cpp \
        characters/efirefighter.cpp \
        characters/efleecevendorchar.cpp \
        characters/efoodvendorchar.cpp \
        characters/egoat.cpp \
        characters/egoatherd.cpp \
        characters/egrower.cpp \
        characters/egymnast.cpp \
        characters/ehealer.cpp \
        characters/ehorsevendorchar.cpp \
        characters/ehunter.cpp \
        characters/elumberjack.cpp \
        characters/eoilvendorchar.cpp \
        characters/ephilosopher.cpp \
        characters/eresourcecollector.cpp \
        characters/esettler.cpp \
        characters/esheep.cpp \
        characters/eshepherd.cpp \
        characters/esilverminer.cpp \
        characters/etaxcollector.cpp \
        characters/ewatchman.cpp \
        characters/ewaterdistributor.cpp \
        characters/ewildanimal.cpp \
        characters/ewinevendorchar.cpp \
        characters/gods/eathena.cpp \
        characters/gods/edemeter.cpp \
        emainwindow.cpp \
        emusic.cpp \
        engine/boardData/eappealmap.cpp \
        engine/boardData/eappealupdatetask.cpp \
        engine/boardData/eemploymentdata.cpp \
        engine/boardData/epopulationdata.cpp \
        engine/edate.cpp \
        engine/edifficulty.cpp \
        engine/efunctask.cpp \
        engine/egameboard.cpp \
        engine/egameboarditerator.cpp \
        engine/emainthreadpathfinder.cpp \
        engine/emapgenerator.cpp \
        engine/eotherthreadpathfinder.cpp \
        engine/epathfinder.cpp \
        engine/eresourcetype.cpp \
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
        esounds.cpp \
        espriteframe.cpp \
        esprites.cpp \
        etexture.cpp \
        etexturecollection.cpp \
        main.cpp \
        pointers/eobject.cpp \
        pointers/estdselfref.cpp \
        spawners/eboarspawner.cpp \
        spawners/edeerspawner.cpp \
        spawners/esettlerspawner.cpp \
        spawners/espawner.cpp \
        textures/ebeachtodry.cpp \
        textures/ebuildingtextures.cpp \
        textures/echaractertextures.cpp \
        textures/edestructiontextures.cpp \
        textures/efertiletodry.cpp \
        textures/eforesttodry.cpp \
        textures/egametextures.cpp \
        textures/egodtextures.cpp \
        textures/einterfacetextures.cpp \
        textures/eparktexture.cpp \
        textures/estonestodry.cpp \
        textures/esurroundingterrain.cpp \
        textures/eterraintextures.cpp \
        textures/etextureloadinghelpers.cpp \
        textures/etiletotexture.cpp \
        textures/evaryingsizetex.cpp \
        textures/ewatercorner.cpp \
        textures/ewatertobeach.cpp \
        textures/ewatertodry.cpp \
        textures/ewatertox.cpp \
        widgets/datawidgets/eappealdatawidget.cpp \
        widgets/datawidgets/eculturedatawidget.cpp \
        widgets/datawidgets/eemploymentdatawidget.cpp \
        widgets/datawidgets/ehygienesafetydatawidget.cpp \
        widgets/datawidgets/epopulationdatawidget.cpp \
        widgets/datawidgets/estoragedatawidget.cpp \
        widgets/datawidgets/eviewmodebutton.cpp \
        widgets/eactionlistwidget.cpp \
        widgets/ebuildingmode.cpp \
        widgets/ebutton.cpp \
        widgets/echeckablebutton.cpp \
        widgets/echeckbox.cpp \
        widgets/ecombobox.cpp \
        widgets/econtextmenu.cpp \
        widgets/eeventwidget.cpp \
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
        widgets/emainmenubase.cpp \
        widgets/emenuloadingwidget.cpp \
        widgets/eminimap.cpp \
        widgets/emouseevent.cpp \
        widgets/epainter.cpp \
        widgets/eprogressbar.cpp \
        widgets/eresolution.cpp \
        widgets/esettingsmenu.cpp \
        widgets/espinbox.cpp \
        widgets/eswitchbutton.cpp \
        widgets/etabwidget.cpp \
        widgets/eterraineditmenu.cpp \
        widgets/etilepainter.cpp \
        widgets/etopbarwidget.cpp \
        widgets/eviewmode.cpp \
        widgets/ewidget.cpp \
        widgets/infowidgets/egyminfowidget.cpp \
        widgets/infowidgets/einfowidget.cpp \
        widgets/infowidgets/estorageinfowidget.cpp

HEADERS += \
    buildings/eaestheticsbuilding.h \
    buildings/eanimalbuilding.h \
    buildings/eappeal.h \
    buildings/earmory.h \
    buildings/earmsvendor.h \
    buildings/ebuilding.h \
    buildings/ebuildingwithresource.h \
    buildings/ecardingshed.h \
    buildings/ecarrotfarm.h \
    buildings/ecollege.h \
    buildings/ecolumn.h \
    buildings/edairy.h \
    buildings/edramaschool.h \
    buildings/eemployingbuilding.h \
    buildings/efarmbase.h \
    buildings/efleecevendor.h \
    buildings/efoodvendor.h \
    buildings/efoundry.h \
    buildings/efountain.h \
    buildings/egranary.h \
    buildings/egrowerslodge.h \
    buildings/egymnasium.h \
    buildings/ehorsevendor.h \
    buildings/ehospital.h \
    buildings/ehuntinglodge.h \
    buildings/emaintenanceoffice.h \
    buildings/emint.h \
    buildings/eoilvendor.h \
    buildings/eolivepress.h \
    buildings/eonionfarm.h \
    buildings/epalace1.h \
    buildings/epalace2.h \
    buildings/epark.h \
    buildings/epatrolbuilding.h \
    buildings/epatroltarget.h \
    buildings/epodium.h \
    buildings/eprocessingbuilding.h \
    buildings/eresourcebuilding.h \
    buildings/eresourcebuildingbase.h \
    buildings/eresourcecollectbuilding.h \
    buildings/eroad.h \
    buildings/eruins.h \
    buildings/esculpturestudio.h \
    buildings/eshepherbuildingbase.h \
    buildings/esmallhouse.h \
    buildings/estadium1.h \
    buildings/estadium2.h \
    buildings/estoragebuilding.h \
    buildings/etaxoffice.h \
    buildings/etheater.h \
    buildings/etimbermill.h \
    buildings/evendor.h \
    buildings/ewarehouse.h \
    buildings/ewatchpost.h \
    buildings/ewheatfarm.h \
    buildings/ewinery.h \
    buildings/ewinevendor.h \
    characters/actions/eactionwithcomeback.h \
    characters/actions/eanimalaction.h \
    characters/actions/ecarttransporteraction.h \
    characters/actions/echaracteraction.h \
    characters/actions/echaracteractiontype.h \
    characters/actions/ecollectaction.h \
    characters/actions/ecollectresourceaction.h \
    characters/actions/ecomplexaction.h \
    characters/actions/edieaction.h \
    characters/actions/efightaction.h \
    characters/actions/efirefighteraction.h \
    characters/actions/egodvisitaction.h \
    characters/actions/egroweraction.h \
    characters/actions/ehuntaction.h \
    characters/actions/emoveaction.h \
    characters/actions/emovearoundaction.h \
    characters/actions/emovepathaction.h \
    characters/actions/epatrolaction.h \
    characters/actions/epatrolguidedmoveaction.h \
    characters/actions/epatrolmoveaction.h \
    characters/actions/esettleraction.h \
    characters/actions/eshepherdaction.h \
    characters/actions/ewaitaction.h \
    characters/eactor.h \
    characters/eanimal.h \
    characters/earmsvendorchar.h \
    characters/ebasicpatroler.h \
    characters/eboar.h \
    characters/ebronzeminer.h \
    characters/ecarttransporter.h \
    characters/echaracter.h \
    characters/echaracterbase.h \
    characters/edeer.h \
    characters/edomesticatedanimal.h \
    characters/efightingpatroler.h \
    characters/efirefighter.h \
    characters/efleecevendorchar.h \
    characters/efoodvendorchar.h \
    characters/egoat.h \
    characters/egoatherd.h \
    characters/egrower.h \
    characters/egymnast.h \
    characters/ehealer.h \
    characters/ehorsevendorchar.h \
    characters/ehunter.h \
    characters/elumberjack.h \
    characters/eoilvendorchar.h \
    characters/ephilosopher.h \
    characters/eresourcecollector.h \
    characters/esettler.h \
    characters/esheep.h \
    characters/eshepherd.h \
    characters/esilverminer.h \
    characters/etaxcollector.h \
    characters/ewatchman.h \
    characters/ewaterdistributor.h \
    characters/ewildanimal.h \
    characters/ewinevendorchar.h \
    characters/gods/eathena.h \
    characters/gods/edemeter.h \
    emainwindow.h \
    emusic.h \
    engine/boardData/eappealmap.h \
    engine/boardData/eappealupdatetask.h \
    engine/boardData/eemploymentdata.h \
    engine/boardData/epopulationdata.h \
    engine/edate.h \
    engine/edifficulty.h \
    engine/efunctask.h \
    engine/egameboard.h \
    engine/egameboarditerator.h \
    engine/emainthreadpathfinder.h \
    engine/emapgenerator.h \
    engine/emovedirection.h \
    engine/eorientation.h \
    engine/eotherthreadpathfinder.h \
    engine/epathfinder.h \
    engine/eprovide.h \
    engine/eresourcetype.h \
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
    eoverlay.h \
    epathfindtask.h \
    esounds.h \
    espriteframe.h \
    esprites.h \
    etexture.h \
    etexturecollection.h \
    offsets/PoseidonImps.h \
    offsets/SprMain.h \
    offsets/Zeus_Athena.h \
    offsets/Zeus_Demeter.h \
    pointers/eobject.h \
    pointers/eobject.h \
    pointers/estdpointer.h \
    pointers/estdselfref.h \
    spawners/eboarspawner.h \
    spawners/edeerspawner.h \
    spawners/esettlerspawner.h \
    spawners/espawner.h \
    textures/ebeachtodry.h \
    textures/ebuildingtextures.h \
    textures/echaractertextures.h \
    textures/edestructiontextures.h \
    textures/efertiletodry.h \
    textures/eforesttodry.h \
    textures/egametextures.h \
    textures/egodtextures.h \
    textures/einterfacetextures.h \
    textures/eparktexture.h \
    textures/estonestodry.h \
    textures/esurroundingterrain.h \
    textures/eterraintextures.h \
    textures/etextureloadinghelpers.h \
    textures/etiletotexture.h \
    textures/evaryingsizetex.h \
    textures/ewatercorner.h \
    textures/ewatertobeach.h \
    textures/ewatertodry.h \
    textures/ewatertox.h \
    widgets/datawidgets/eappealdatawidget.h \
    widgets/datawidgets/eculturedatawidget.h \
    widgets/datawidgets/eemploymentdatawidget.h \
    widgets/datawidgets/ehygienesafetydatawidget.h \
    widgets/datawidgets/epopulationdatawidget.h \
    widgets/datawidgets/estoragedatawidget.h \
    widgets/datawidgets/eviewmodebutton.h \
    widgets/eactionlistwidget.h \
    widgets/ebuildingmode.h \
    widgets/ebutton.h \
    widgets/echeckablebutton.h \
    widgets/echeckbox.h \
    widgets/ecombobox.h \
    widgets/econtextmenu.h \
    widgets/eeventwidget.h \
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
    widgets/emainmenubase.h \
    widgets/emenuloadingwidget.h \
    widgets/eminimap.h \
    widgets/emouseevent.h \
    widgets/epainter.h \
    widgets/eprogressbar.h \
    widgets/eresolution.h \
    widgets/esettingsmenu.h \
    widgets/espinbox.h \
    widgets/eswitchbutton.h \
    widgets/etabwidget.h \
    widgets/eterraineditmenu.h \
    widgets/etilepainter.h \
    widgets/etopbarwidget.h \
    widgets/eviewmode.h \
    widgets/ewidget.h \
    widgets/infowidgets/egyminfowidget.h \
    widgets/infowidgets/einfowidget.h \
    widgets/infowidgets/estorageinfowidget.h
