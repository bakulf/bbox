BBVERSION=0.1

message('BBox: $$BBVERSION')

TEMPLATE  = app
TARGET    = bbox
QT       += network

MOC_DIR     = .moc
OBJECTS_DIR = .obj

unix {
    QMAKE_CXXFLAGS *= -Werror
}

win32 {
    LIBS = -lAdvapi32
    RC_FILE = win32.rc
}

macx {
    DEFINES += BBFILESYSTEMWATCHER
}

DEFINES += BBPACKAGE=\\\"BBox\\\" \
           BBVERSION=\\\"$$BBVERSION\\\"

INCLUDEPATH += . \
               actions \
               actions/cleanup \
               actions/add \
               actions/delete \
               actions/update \
               actions/obstructed \
               actions/commit \
               actions/localchanges \
               actions/remotechanges \
               actions/schedulecommit \
               actions/scheduleupdate

DEPENDPATH  += $${INCLUDEPATH}

HEADERS   = bbconst.h \
            bbapplication.h \
            bbpreferences.h \
            bbpreferencesfirstpage.h \
            bbpreferencesmiddlepage.h \
            bbpreferenceslastpage.h \
            bbabout.h \
            bblogs.h \
            bblogsitem.h \
            bblogsmodel.h \
            bboperations.h \
            bbsettings.h \
            bbobserver.h \
            bbfilesystemwatcher.h \
            bbsvn.h \
            bbsvnmanager.h \
            bbsvnstatus.h \
            bbsvninfo.h \
            bbsvnlog.h \
            bbconflict.h \
            bbconflictfile.h \
            bbsendreceive.h \
            bbactionmanager.h \
            bbaction.h \
            bbactioncleanup.h \
            bbactionadd.h \
            bbactiondelete.h \
            bbactionupdate.h \
            bbactionobstructed.h \
            bbactioncommit.h \
            bbactionlocalchanges.h \
            bbactionremotechanges.h \
            bbactionschedulecommit.h \
            bbactionscheduleupdate.h

SOURCES   = main.cpp \
            bbapplication.cpp \
            bbpreferences.cpp \
            bbpreferencesfirstpage.cpp \
            bbpreferencesmiddlepage.cpp \
            bbpreferenceslastpage.cpp \
            bbabout.cpp \
            bblogs.cpp \
            bblogsitem.cpp \
            bblogsmodel.cpp \
            bboperations.cpp \
            bbsettings.cpp \
            bbobserver.cpp \
            bbfilesystemwatcher.cpp \
            bbsvn.cpp \
            bbsvnmanager.cpp \
            bbsvnstatus.cpp \
            bbsvninfo.cpp \
            bbsvnlog.cpp \
            bbconflict.cpp \
            bbconflictfile.cpp \
            bbsendreceive.cpp \
            bbactionmanager.cpp \
            bbaction.cpp \
            bbactioncleanup.cpp \
            bbactionadd.cpp \
            bbactiondelete.cpp \
            bbactionupdate.cpp \
            bbactionobstructed.cpp \
            bbactioncommit.cpp \
            bbactionlocalchanges.cpp \
            bbactionremotechanges.cpp \
            bbactionschedulecommit.cpp \
            bbactionscheduleupdate.cpp

RESOURCES = bbox.qrc

unix {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }

    BINDIR = $$PREFIX/bin
    DATADIR =$$PREFIX/share

    target.path =$$BINDIR

    desktop.path = $$DATADIR/applications
    desktop.files += ../utils/$${TARGET}.desktop

    icon.path = $$DATADIR/icons
    icon.files += ../utils/$${TARGET}.xpm

    INSTALLS += target desktop icon
}
