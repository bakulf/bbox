BBVERSION=0.1

message('BBox: $$BBVERSION')
message("Qt version: $$[QT_VERSION]")

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
            bbactioncommit.cpp \
            bbactionlocalchanges.cpp \
            bbactionremotechanges.cpp \
            bbactionschedulecommit.cpp \
            bbactionscheduleupdate.cpp

RESOURCES = bbox.qrc

!isEmpty(INSTALL_PREFIX) {
    message("Target path will be $$INSTALL_PREFIX")
    target.path = $$INSTALL_PREFIX
} else {
    unix {
        contains($$system(id -u), 0) {
                message("installing as root. Target path will be /usr/local/bin/")
                target.path = /usr/local/bin
        } else {
                message("installing as user. Target path will be ~/bin/")
                target.path = ~/bin
        }
    }
}

INSTALLS += target
