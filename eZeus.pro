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
LIBS += -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lstdc++fs
LIBS += -lnoise

SOURCES += \
        audio/egodsounds.cpp \
        audio/eherosounds.cpp \
        audio/emusic.cpp \
        audio/esounds.cpp \
        buildings/eaestheticsbuilding.cpp \
        buildings/eagorabase.cpp \
        buildings/eagoraspace.cpp \
        buildings/eanimalbuilding.cpp \
        buildings/earmory.cpp \
        buildings/earmsvendor.cpp \
        buildings/eartisansguild.cpp \
        buildings/eavenue.cpp \
        buildings/ebuilding.cpp \
        buildings/ebuildingrenderer.cpp \
        buildings/ebuildingwithresource.cpp \
        buildings/ecardingshed.cpp \
        buildings/ecarrotfarm.cpp \
        buildings/ecollege.cpp \
        buildings/ecolumn.cpp \
        buildings/ecommonagora.cpp \
        buildings/edairy.cpp \
        buildings/edramaschool.cpp \
        buildings/eelitehousing.cpp \
        buildings/eelitehousingrenderer.cpp \
        buildings/eemployingbuilding.cpp \
        buildings/efarmbase.cpp \
        buildings/efishery.cpp \
        buildings/efleecevendor.cpp \
        buildings/efoodvendor.cpp \
        buildings/efoundry.cpp \
        buildings/efountain.cpp \
        buildings/egatehouse.cpp \
        buildings/egatehouserenderer.cpp \
        buildings/egranary.cpp \
        buildings/egrandagora.cpp \
        buildings/egrowerslodge.cpp \
        buildings/egymnasium.cpp \
        buildings/eheatgetters.cpp \
        buildings/eheroshall.cpp \
        buildings/ehorseranch.cpp \
        buildings/ehorseranchenclosure.cpp \
        buildings/ehorsevendor.cpp \
        buildings/ehospital.cpp \
        buildings/ehousebase.cpp \
        buildings/ehuntinglodge.cpp \
        buildings/emaintenanceoffice.cpp \
        buildings/emasonryshop.cpp \
        buildings/emint.cpp \
        buildings/eoilvendor.cpp \
        buildings/eolivepress.cpp \
        buildings/eonionfarm.cpp \
        buildings/epalace.cpp \
        buildings/epalacerenderer.cpp \
        buildings/epalacetile.cpp \
        buildings/epark.cpp \
        buildings/epatrolbuilding.cpp \
        buildings/epatrolbuildingbase.cpp \
        buildings/epatroltarget.cpp \
        buildings/epier.cpp \
        buildings/epodium.cpp \
        buildings/eprocessingbuilding.cpp \
        buildings/eresourcebuilding.cpp \
        buildings/eresourcebuildingbase.cpp \
        buildings/eresourcecollectbuilding.cpp \
        buildings/eresourcecollectbuildingbase.cpp \
        buildings/eroad.cpp \
        buildings/eruins.cpp \
        buildings/esculpturestudio.cpp \
        buildings/eshepherbuildingbase.cpp \
        buildings/esmallhouse.cpp \
        buildings/estadium.cpp \
        buildings/estadiumrenderer.cpp \
        buildings/estoragebuilding.cpp \
        buildings/etaxoffice.cpp \
        buildings/etheater.cpp \
        buildings/etimbermill.cpp \
        buildings/etower.cpp \
        buildings/etradepost.cpp \
        buildings/evendor.cpp \
        buildings/ewall.cpp \
        buildings/ewarehouse.cpp \
        buildings/ewarehousebase.cpp \
        buildings/ewatchpost.cpp \
        buildings/ewheatfarm.cpp \
        buildings/ewinery.cpp \
        buildings/ewinevendor.cpp \
        buildings/sanctuaries/eartemissanctuary.cpp \
        buildings/sanctuaries/ehephaestussanctuary.cpp \
        buildings/sanctuaries/esanctbuilding.cpp \
        buildings/sanctuaries/esanctuary.cpp \
        buildings/sanctuaries/esanctuaryblueprint.cpp \
        buildings/sanctuaries/estairsrenderer.cpp \
        buildings/sanctuaries/etemplealtarbuilding.cpp \
        buildings/sanctuaries/etemplebuilding.cpp \
        buildings/sanctuaries/etemplemonumentbuilding.cpp \
        buildings/sanctuaries/etemplerenderer.cpp \
        buildings/sanctuaries/etemplestatuebuilding.cpp \
        buildings/sanctuaries/etempletilebuilding.cpp \
        characters/actions/eactionwithcomeback.cpp \
        characters/actions/eanimalaction.cpp \
        characters/actions/earcheraction.cpp \
        characters/actions/eartisanaction.cpp \
        characters/actions/ebuildaction.cpp \
        characters/actions/ecarttransporteraction.cpp \
        characters/actions/echaracteraction.cpp \
        characters/actions/ecollectaction.cpp \
        characters/actions/ecollectresourceaction.cpp \
        characters/actions/ecomplexaction.cpp \
        characters/actions/edieaction.cpp \
        characters/actions/efightaction.cpp \
        characters/actions/efirefighteraction.cpp \
        characters/actions/efollowaction.cpp \
        characters/actions/eforce.cpp \
        characters/actions/egodaction.cpp \
        characters/actions/egodattackaction.cpp \
        characters/actions/egodmonsteraction.cpp \
        characters/actions/egodvisitaction.cpp \
        characters/actions/egodworshippedaction.cpp \
        characters/actions/egroweraction.cpp \
        characters/actions/ehuntaction.cpp \
        characters/actions/emonsteraction.cpp \
        characters/actions/emoveaction.cpp \
        characters/actions/emovearoundaction.cpp \
        characters/actions/emovepathaction.cpp \
        characters/actions/emovetoaction.cpp \
        characters/actions/epatrolaction.cpp \
        characters/actions/epatrolguidedmoveaction.cpp \
        characters/actions/epatrolmoveaction.cpp \
        characters/actions/esettleraction.cpp \
        characters/actions/eshepherdaction.cpp \
        characters/actions/esoldieraction.cpp \
        characters/actions/etraderaction.cpp \
        characters/actions/ewaitaction.cpp \
        characters/eactor.cpp \
        characters/eanimal.cpp \
        characters/earcher.cpp \
        characters/eartisan.cpp \
        characters/ebasicpatroler.cpp \
        characters/eboar.cpp \
        characters/ebronzeminer.cpp \
        characters/ecarttransporter.cpp \
        characters/echaracter.cpp \
        characters/echaracterbase.cpp \
        characters/edeer.cpp \
        characters/edomesticatedanimal.cpp \
        characters/edonkey.cpp \
        characters/efightingpatroler.cpp \
        characters/efirefighter.cpp \
        characters/efishingboat.cpp \
        characters/egoat.cpp \
        characters/egoatherd.cpp \
        characters/egreekhoplite.cpp \
        characters/egreekrockthrower.cpp \
        characters/egrower.cpp \
        characters/egymnast.cpp \
        characters/ehealer.cpp \
        characters/ehoplite.cpp \
        characters/ehoplitebase.cpp \
        characters/ehorse.cpp \
        characters/ehorseman.cpp \
        characters/ehorsemanbase.cpp \
        characters/ehunter.cpp \
        characters/elumberjack.cpp \
        characters/emarbleminer.cpp \
        characters/eox.cpp \
        characters/epeddler.cpp \
        characters/ephilosopher.cpp \
        characters/eporter.cpp \
        characters/erangesoldier.cpp \
        characters/eresourcecollector.cpp \
        characters/eresourcecollectorbase.cpp \
        characters/erockthrower.cpp \
        characters/erockthrowerbase.cpp \
        characters/esettler.cpp \
        characters/esheep.cpp \
        characters/eshepherd.cpp \
        characters/eshepherdbase.cpp \
        characters/esilverminer.cpp \
        characters/esoldier.cpp \
        characters/esoldierbanner.cpp \
        characters/etaxcollector.cpp \
        characters/etradeboat.cpp \
        characters/etrader.cpp \
        characters/etrailer.cpp \
        characters/ewatchman.cpp \
        characters/ewaterdistributor.cpp \
        characters/ewildanimal.cpp \
        characters/gods/eathena.cpp \
        characters/gods/ebasicgod.cpp \
        characters/gods/edemeter.cpp \
        characters/gods/edionysus.cpp \
        characters/gods/eextendedgod.cpp \
        characters/gods/egod.cpp \
        characters/gods/ehermes.cpp \
        characters/heroes/eachilles.cpp \
        characters/heroes/ebasichero.cpp \
        characters/heroes/ehero.cpp \
        characters/monsters/ebasicmonster.cpp \
        characters/monsters/ecalydonianboar.cpp \
        characters/monsters/emonster.cpp \
        characters/monsters/ewatermonster.cpp \
        efontcolor.cpp \
        egamedir.cpp \
        eiteratesquare.cpp \
        elanguage.cpp \
        eloadtexthelper.cpp \
        emainwindow.cpp \
        emessage.cpp \
        emessages.cpp \
        engine/boardData/eemploymentdata.cpp \
        engine/boardData/eheatmap.cpp \
        engine/boardData/eheatmapdivisor.cpp \
        engine/boardData/eheatmaptask.cpp \
        engine/boardData/epopulationdata.cpp \
        engine/edate.cpp \
        engine/edifficulty.cpp \
        engine/efunctask.cpp \
        engine/egameboard.cpp \
        engine/egameboarditerator.cpp \
        engine/emapgenerator.cpp \
        engine/epathboard.cpp \
        engine/epathfinder.cpp \
        engine/eplannedaction.cpp \
        engine/eresourcetype.cpp \
        engine/etask.cpp \
        engine/etaxrate.cpp \
        engine/eterrain.cpp \
        engine/ethreadpool.cpp \
        engine/etile.cpp \
        engine/etilebase.cpp \
        engine/ewagerate.cpp \
        engine/eworldboard.cpp \
        engine/eworldcity.cpp \
        engine/thread/ethreadboard.cpp \
        engine/thread/ethreadbuilding.cpp \
        engine/thread/ethreadcharacter.cpp \
        engine/thread/ethreaddata.cpp \
        engine/thread/ethreadtile.cpp \
        epathdatafindtask.cpp \
        epathfindtask.cpp \
        estringhelpers.cpp \
        etexture.cpp \
        etexturecollection.cpp \
        etilehelper.cpp \
        evectorhelpers.cpp \
        ewalkablehelpers.cpp \
        fileIO/ebuildingreader.cpp \
        fileIO/ebuildingrendererreader.cpp \
        fileIO/ebuildingrendererwriter.cpp \
        fileIO/ebuildingwriter.cpp \
        fileIO/ereadstream.cpp \
        fileIO/ewritestream.cpp \
        gameEvents/egameevent.cpp \
        gameEvents/egameeventcycle.cpp \
        gameEvents/egodattackevent.cpp \
        gameEvents/egodvisitevent.cpp \
        gameEvents/emonsterattackevent.cpp \
        main.cpp \
        missiles/earrowmissile.cpp \
        missiles/egodmissile.cpp \
        missiles/emissile.cpp \
        missiles/erockmissile.cpp \
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
        textures/emarbletile.cpp \
        textures/eparktexture.cpp \
        textures/estonestodry.cpp \
        textures/esurroundingterrain.cpp \
        textures/eterraintextures.cpp \
        textures/etextureclass.cpp \
        textures/etextureloader.cpp \
        textures/etextureloadinghelpers.cpp \
        textures/etiletotexture.cpp \
        textures/evaryingsizetex.cpp \
        textures/ewatercorner.cpp \
        textures/ewatertobeach.cpp \
        textures/ewatertodry.cpp \
        textures/ewatertox.cpp \
        widgets/datawidgets/eadmindatawidget.cpp \
        widgets/datawidgets/eappealdatawidget.cpp \
        widgets/datawidgets/eculturedatawidget.cpp \
        widgets/datawidgets/edatawidget.cpp \
        widgets/datawidgets/eemploymentdatawidget.cpp \
        widgets/datawidgets/ehusbandrydatawidget.cpp \
        widgets/datawidgets/ehygienesafetydatawidget.cpp \
        widgets/datawidgets/emilitarydatawidget.cpp \
        widgets/datawidgets/emythologydatawidget.cpp \
        widgets/datawidgets/eoverviewdatawidget.cpp \
        widgets/datawidgets/epopulationdatawidget.cpp \
        widgets/datawidgets/estoragedatawidget.cpp \
        widgets/datawidgets/eviewmodebutton.cpp \
        widgets/eacceptbutton.cpp \
        widgets/eactionlistwidget.cpp \
        widgets/earmymenu.cpp \
        widgets/ebasicbutton.cpp \
        widgets/ebuildbutton.cpp \
        widgets/ebuildingmode.cpp \
        widgets/ebuildingstoerase.cpp \
        widgets/ebuildwidget.cpp \
        widgets/ebutton.cpp \
        widgets/ebuttonbase.cpp \
        widgets/ecancelbutton.cpp \
        widgets/echeckablebutton.cpp \
        widgets/echeckbox.cpp \
        widgets/ecombobox.cpp \
        widgets/econtextmenu.cpp \
        widgets/edownbutton.cpp \
        widgets/eeventwidget.cpp \
        widgets/eexclamationbutton.cpp \
        widgets/efilewidget.cpp \
        widgets/eflatbutton.cpp \
        widgets/efonts.cpp \
        widgets/eframedbutton.cpp \
        widgets/eframedlabel.cpp \
        widgets/eframedwidget.cpp \
        widgets/egameloadingwidget.cpp \
        widgets/egamemainmenu.cpp \
        widgets/egamemenu.cpp \
        widgets/egamemenubase.cpp \
        widgets/egamewidget.cpp \
        widgets/egamewidgetbuild.cpp \
        widgets/egamewidgetevents.cpp \
        widgets/egamewidgetinfo.cpp \
        widgets/egamewidgetpaint.cpp \
        widgets/ehelpbutton.cpp \
        widgets/elabel.cpp \
        widgets/elabelbase.cpp \
        widgets/elabeledwidget.cpp \
        widgets/elineedit.cpp \
        widgets/elinewidget.cpp \
        widgets/eloadingwidget.cpp \
        widgets/emainmenu.cpp \
        widgets/emainmenubase.cpp \
        widgets/emenuloadingwidget.cpp \
        widgets/emessagebox.cpp \
        widgets/eminimap.cpp \
        widgets/emouseevent.cpp \
        widgets/emultilinelabel.cpp \
        widgets/eokbutton.cpp \
        widgets/epainter.cpp \
        widgets/eprogressbar.cpp \
        widgets/equestionwidget.cpp \
        widgets/eresolution.cpp \
        widgets/escrollbar.cpp \
        widgets/escrollwidget.cpp \
        widgets/esettingsmenu.cpp \
        widgets/espinbox.cpp \
        widgets/eswitchbutton.cpp \
        widgets/etabwidget.cpp \
        widgets/eterraineditmenu.cpp \
        widgets/etilepainter.cpp \
        widgets/etopbarwidget.cpp \
        widgets/eupbutton.cpp \
        widgets/eviewmode.cpp \
        widgets/ewidget.cpp \
        widgets/eworldgoodswidget.cpp \
        widgets/eworldmapwidget.cpp \
        widgets/eworldmenu.cpp \
        widgets/eworldwidget.cpp \
        widgets/infowidgets/eagorainfowidget.cpp \
        widgets/infowidgets/ecommonhouseinfowidget.cpp \
        widgets/infowidgets/eemployingbuildinginfowidget.cpp \
        widgets/infowidgets/egyminfowidget.cpp \
        widgets/infowidgets/eheroshallinfowidget.cpp \
        widgets/infowidgets/einfowidget.cpp \
        widgets/infowidgets/estorageinfowidget.cpp \
        widgets/infowidgets/etradepostinfowidget.cpp

HEADERS += \
    audio/egodsounds.h \
    audio/eherosounds.h \
    audio/emusic.h \
    audio/esounds.h \
    buildings/allbuildings.h \
    buildings/eaestheticsbuilding.h \
    buildings/eagorabase.h \
    buildings/eagoraspace.h \
    buildings/eanimalbuilding.h \
    buildings/earmory.h \
    buildings/earmsvendor.h \
    buildings/eartisansguild.h \
    buildings/eavenue.h \
    buildings/ebuilding.h \
    buildings/ebuildingrenderer.h \
    buildings/ebuildingwithresource.h \
    buildings/ecardingshed.h \
    buildings/ecarrotfarm.h \
    buildings/ecollege.h \
    buildings/ecolumn.h \
    buildings/ecommonagora.h \
    buildings/edairy.h \
    buildings/edramaschool.h \
    buildings/eelitehousing.h \
    buildings/eelitehousingrenderer.h \
    buildings/eemployingbuilding.h \
    buildings/efarmbase.h \
    buildings/efishery.h \
    buildings/efleecevendor.h \
    buildings/efoodvendor.h \
    buildings/efoundry.h \
    buildings/efountain.h \
    buildings/egatehouse.h \
    buildings/egatehouserenderer.h \
    buildings/egranary.h \
    buildings/egrandagora.h \
    buildings/egrowerslodge.h \
    buildings/egymnasium.h \
    buildings/eheatgetters.h \
    buildings/eheroshall.h \
    buildings/ehorseranch.h \
    buildings/ehorseranchenclosure.h \
    buildings/ehorsevendor.h \
    buildings/ehospital.h \
    buildings/ehousebase.h \
    buildings/ehuntinglodge.h \
    buildings/emaintenanceoffice.h \
    buildings/emasonryshop.h \
    buildings/emint.h \
    buildings/eoilvendor.h \
    buildings/eolivepress.h \
    buildings/eonionfarm.h \
    buildings/epalace.h \
    buildings/epalacerenderer.h \
    buildings/epalacetile.h \
    buildings/epark.h \
    buildings/epatrolbuilding.h \
    buildings/epatrolbuildingbase.h \
    buildings/epatroltarget.h \
    buildings/epier.h \
    buildings/epodium.h \
    buildings/eprocessingbuilding.h \
    buildings/eresourcebuilding.h \
    buildings/eresourcebuildingbase.h \
    buildings/eresourcecollectbuilding.h \
    buildings/eresourcecollectbuildingbase.h \
    buildings/eroad.h \
    buildings/eruins.h \
    buildings/esculpturestudio.h \
    buildings/eshepherbuildingbase.h \
    buildings/esmallhouse.h \
    buildings/estadium.h \
    buildings/estadiumrenderer.h \
    buildings/estoragebuilding.h \
    buildings/etaxoffice.h \
    buildings/etheater.h \
    buildings/etimbermill.h \
    buildings/etower.h \
    buildings/etradepost.h \
    buildings/evendor.h \
    buildings/ewall.h \
    buildings/ewarehouse.h \
    buildings/ewarehousebase.h \
    buildings/ewatchpost.h \
    buildings/ewheatfarm.h \
    buildings/ewinery.h \
    buildings/ewinevendor.h \
    buildings/sanctuaries/eartemissanctuary.h \
    buildings/sanctuaries/ehephaestussanctuary.h \
    buildings/sanctuaries/esanctbuilding.h \
    buildings/sanctuaries/esanctuary.h \
    buildings/sanctuaries/esanctuaryblueprint.h \
    buildings/sanctuaries/estairsrenderer.h \
    buildings/sanctuaries/etemplealtarbuilding.h \
    buildings/sanctuaries/etemplebuilding.h \
    buildings/sanctuaries/etemplemonumentbuilding.h \
    buildings/sanctuaries/etemplerenderer.h \
    buildings/sanctuaries/etemplestatuebuilding.h \
    buildings/sanctuaries/etempletilebuilding.h \
    characters/actions/eactionwithcomeback.h \
    characters/actions/eanimalaction.h \
    characters/actions/earcheraction.h \
    characters/actions/eartisanaction.h \
    characters/actions/ebuildaction.h \
    characters/actions/ecarttransporteraction.h \
    characters/actions/echaracteraction.h \
    characters/actions/echaracteractiontype.h \
    characters/actions/ecollectaction.h \
    characters/actions/ecollectresourceaction.h \
    characters/actions/ecomplexaction.h \
    characters/actions/edieaction.h \
    characters/actions/efightaction.h \
    characters/actions/efirefighteraction.h \
    characters/actions/efollowaction.h \
    characters/actions/eforce.h \
    characters/actions/egodaction.h \
    characters/actions/egodattackaction.h \
    characters/actions/egodmonsteraction.h \
    characters/actions/egodvisitaction.h \
    characters/actions/egodworshippedaction.h \
    characters/actions/egroweraction.h \
    characters/actions/ehuntaction.h \
    characters/actions/emonsteraction.h \
    characters/actions/emoveaction.h \
    characters/actions/emovearoundaction.h \
    characters/actions/emovepathaction.h \
    characters/actions/emovetoaction.h \
    characters/actions/epatrolaction.h \
    characters/actions/epatrolguidedmoveaction.h \
    characters/actions/epatrolmoveaction.h \
    characters/actions/esettleraction.h \
    characters/actions/eshepherdaction.h \
    characters/actions/esoldieraction.h \
    characters/actions/etraderaction.h \
    characters/actions/ewaitaction.h \
    characters/eactor.h \
    characters/eanimal.h \
    characters/earcher.h \
    characters/eartisan.h \
    characters/ebasicpatroler.h \
    characters/eboar.h \
    characters/ebronzeminer.h \
    characters/ecarttransporter.h \
    characters/echaracter.h \
    characters/echaracterbase.h \
    characters/edeer.h \
    characters/edomesticatedanimal.h \
    characters/edonkey.h \
    characters/efightingpatroler.h \
    characters/efirefighter.h \
    characters/efishingboat.h \
    characters/egoat.h \
    characters/egoatherd.h \
    characters/egreekhoplite.h \
    characters/egreekrockthrower.h \
    characters/egrower.h \
    characters/egymnast.h \
    characters/ehealer.h \
    characters/ehoplite.h \
    characters/ehoplitebase.h \
    characters/ehorse.h \
    characters/ehorseman.h \
    characters/ehorsemanbase.h \
    characters/ehunter.h \
    characters/elumberjack.h \
    characters/emarbleminer.h \
    characters/eox.h \
    characters/epeddler.h \
    characters/ephilosopher.h \
    characters/eporter.h \
    characters/erangesoldier.h \
    characters/eresourcecollector.h \
    characters/eresourcecollectorbase.h \
    characters/erockthrower.h \
    characters/erockthrowerbase.h \
    characters/esettler.h \
    characters/esheep.h \
    characters/eshepherd.h \
    characters/eshepherdbase.h \
    characters/esilverminer.h \
    characters/esoldier.h \
    characters/esoldierbanner.h \
    characters/etaxcollector.h \
    characters/etradeboat.h \
    characters/etrader.h \
    characters/etrailer.h \
    characters/ewatchman.h \
    characters/ewaterdistributor.h \
    characters/ewildanimal.h \
    characters/gods/eathena.h \
    characters/gods/ebasicgod.h \
    characters/gods/edemeter.h \
    characters/gods/edionysus.h \
    characters/gods/eextendedgod.h \
    characters/gods/egod.h \
    characters/gods/ehermes.h \
    characters/heroes/eachilles.h \
    characters/heroes/ebasichero.h \
    characters/heroes/ehero.h \
    characters/monsters/ebasicmonster.h \
    characters/monsters/ecalydonianboar.h \
    characters/monsters/emonster.h \
    characters/monsters/ewatermonster.h \
    efontcolor.h \
    egamedir.h \
    eiteratesquare.h \
    elanguage.h \
    eloadtexthelper.h \
    emainwindow.h \
    emessage.h \
    emessages.h \
    engine/boardData/eemploymentdata.h \
    engine/boardData/eheatmap.h \
    engine/boardData/eheatmapdivisor.h \
    engine/boardData/eheatmaptask.h \
    engine/boardData/epopulationdata.h \
    engine/edate.h \
    engine/edifficulty.h \
    engine/efunctask.h \
    engine/egameboard.h \
    engine/egameboarditerator.h \
    engine/emapgenerator.h \
    engine/emovedirection.h \
    engine/eorientation.h \
    engine/epathboard.h \
    engine/epathfinder.h \
    engine/eplannedaction.h \
    engine/eprovide.h \
    engine/eresourcetype.h \
    engine/etask.h \
    engine/etaxrate.h \
    engine/eterrain.h \
    engine/ethreadpool.h \
    engine/etile.h \
    engine/etilebase.h \
    engine/ewagerate.h \
    engine/eworldboard.h \
    engine/eworldcity.h \
    engine/thread/ethreadboard.h \
    engine/thread/ethreadbuilding.h \
    engine/thread/ethreadcharacter.h \
    engine/thread/ethreaddata.h \
    engine/thread/ethreadtile.h \
    eoverlay.h \
    epathdatafindtask.h \
    epathfindtask.h \
    esettings.h \
    estringhelpers.h \
    etexture.h \
    etexturecollection.h \
    etilehelper.h \
    evectorhelpers.h \
    ewalkablehelpers.h \
    fileIO/ebuildingreader.h \
    fileIO/ebuildingrendererreader.h \
    fileIO/ebuildingrendererwriter.h \
    fileIO/ebuildingwriter.h \
    fileIO/ereadstream.h \
    fileIO/estreams.h \
    fileIO/ewritestream.h \
    gameEvents/egameevent.h \
    gameEvents/egameeventcycle.h \
    gameEvents/egodattackevent.h \
    gameEvents/egodvisitevent.h \
    gameEvents/emonsterattackevent.h \
    missiles/earrowmissile.h \
    missiles/egodmissile.h \
    missiles/emissile.h \
    missiles/erockmissile.h \
    offsets/PoseidonImps.h \
    offsets/Poseidon_Atalanta.h \
    offsets/Poseidon_Atlas.h \
    offsets/Poseidon_Bellerophon.h \
    offsets/Poseidon_Chimera.h \
    offsets/Poseidon_Echidna.h \
    offsets/Poseidon_Harpie.h \
    offsets/Poseidon_Hera.h \
    offsets/Poseidon_Sphinx.h \
    offsets/SprAmbient.h \
    offsets/SprMain.h \
    offsets/Zeus_Aphrodite.h \
    offsets/Zeus_Apollo.h \
    offsets/Zeus_Ares.h \
    offsets/Zeus_Artemis.h \
    offsets/Zeus_Athena.h \
    offsets/Zeus_Demeter.h \
    offsets/Zeus_Dionysus.h \
    offsets/Zeus_Greek.h \
    offsets/Zeus_Hades.h \
    offsets/Zeus_Hephaestus.h \
    offsets/Zeus_Hermes.h \
    offsets/Zeus_Poseidon.h \
    offsets/Zeus_Zeus.h \
    offsets/destruction.h \
    offsets/zeus_achilles.h \
    offsets/zeus_calydonianboar.h \
    offsets/zeus_cerberus.h \
    offsets/zeus_cyclops.h \
    offsets/zeus_dragon.h \
    offsets/zeus_hector.h \
    offsets/zeus_heracles.h \
    offsets/zeus_hydra.h \
    offsets/zeus_jason.h \
    offsets/zeus_kraken.h \
    offsets/zeus_maenads.h \
    offsets/zeus_medusa.h \
    offsets/zeus_minotaur.h \
    offsets/zeus_odysseus.h \
    offsets/zeus_perseus.h \
    offsets/zeus_scylla.h \
    offsets/zeus_talos.h \
    offsets/zeus_theseus.h \
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
    textures/emarbletile.h \
    textures/eparktexture.h \
    textures/estonestodry.h \
    textures/esurroundingterrain.h \
    textures/eterraintextures.h \
    textures/etextureclass.h \
    textures/etextureloader.h \
    textures/etextureloadinghelpers.h \
    textures/etiletotexture.h \
    textures/evaryingsizetex.h \
    textures/ewatercorner.h \
    textures/ewatertobeach.h \
    textures/ewatertodry.h \
    textures/ewatertox.h \
    vec2.h \
    widgets/datawidgets/eadmindatawidget.h \
    widgets/datawidgets/eappealdatawidget.h \
    widgets/datawidgets/eculturedatawidget.h \
    widgets/datawidgets/edatawidget.h \
    widgets/datawidgets/eemploymentdatawidget.h \
    widgets/datawidgets/ehusbandrydatawidget.h \
    widgets/datawidgets/ehygienesafetydatawidget.h \
    widgets/datawidgets/emilitarydatawidget.h \
    widgets/datawidgets/emythologydatawidget.h \
    widgets/datawidgets/eoverviewdatawidget.h \
    widgets/datawidgets/epopulationdatawidget.h \
    widgets/datawidgets/estoragedatawidget.h \
    widgets/datawidgets/eviewmodebutton.h \
    widgets/eacceptbutton.h \
    widgets/eactionlistwidget.h \
    widgets/earmymenu.h \
    widgets/ebasicbutton.h \
    widgets/ebuildbutton.h \
    widgets/ebuildingmode.h \
    widgets/ebuildingstoerase.h \
    widgets/ebuildwidget.h \
    widgets/ebutton.h \
    widgets/ebuttonbase.h \
    widgets/ecancelbutton.h \
    widgets/echeckablebutton.h \
    widgets/echeckbox.h \
    widgets/ecombobox.h \
    widgets/econtextmenu.h \
    widgets/edownbutton.h \
    widgets/eeventwidget.h \
    widgets/eexclamationbutton.h \
    widgets/efilewidget.h \
    widgets/eflatbutton.h \
    widgets/efonts.h \
    widgets/eframedbutton.h \
    widgets/eframedlabel.h \
    widgets/eframedwidget.h \
    widgets/egameloadingwidget.h \
    widgets/egamemainmenu.h \
    widgets/egamemenu.h \
    widgets/egamemenubase.h \
    widgets/egamewidget.h \
    widgets/ehelpbutton.h \
    widgets/elabel.h \
    widgets/elabelbase.h \
    widgets/elabeledwidget.h \
    widgets/elineedit.h \
    widgets/elinewidget.h \
    widgets/eloadingwidget.h \
    widgets/emainmenu.h \
    widgets/emainmenubase.h \
    widgets/emenuloadingwidget.h \
    widgets/emessagebox.h \
    widgets/eminimap.h \
    widgets/emouseevent.h \
    widgets/emultilinelabel.h \
    widgets/eokbutton.h \
    widgets/epainter.h \
    widgets/eprogressbar.h \
    widgets/equestionwidget.h \
    widgets/eresolution.h \
    widgets/escrollbar.h \
    widgets/escrollwidget.h \
    widgets/esettingsmenu.h \
    widgets/espinbox.h \
    widgets/eswitchbutton.h \
    widgets/etabwidget.h \
    widgets/eterraineditmenu.h \
    widgets/etilepainter.h \
    widgets/etopbarwidget.h \
    widgets/eupbutton.h \
    widgets/eviewmode.h \
    widgets/ewidget.h \
    widgets/eworldgoodswidget.h \
    widgets/eworldmapwidget.h \
    widgets/eworldmenu.h \
    widgets/eworldwidget.h \
    widgets/infowidgets/eagorainfowidget.h \
    widgets/infowidgets/ecommonhouseinfowidget.h \
    widgets/infowidgets/eemployingbuildinginfowidget.h \
    widgets/infowidgets/egyminfowidget.h \
    widgets/infowidgets/eheroshallinfowidget.h \
    widgets/infowidgets/einfowidget.h \
    widgets/infowidgets/estorageinfowidget.h \
    widgets/infowidgets/etradepostinfowidget.h

DISTFILES += \
    sanctuaries/artemis.txt \
    sanctuaries/hephaestus.txt \
    text/language.txt
