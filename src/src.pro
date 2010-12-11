TEMPLATE  = app
TARGET    = bbox
QT       += network

MOC_DIR     = .moc
OBJECTS_DIR = .obj

QMAKE_CXXFLAGS *= -Werror

DEFINES = BBPACKAGE=\\\"BBox\\\"

INCLUDEPATH += actions \
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
            bbcheckout.h \
            bbcheckoutfirstpage.h \
            bbcheckoutlastpage.h \
            bbabout.h \
            bboperations.h \
            bbsettings.h \
            bbobserver.h \
            bbsvn.h \
            bbsvnstatus.h \
            bbsvninfo.h \
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
            bbcheckout.cpp \
            bbcheckoutfirstpage.cpp \
            bbcheckoutlastpage.cpp \
            bbabout.cpp \
            bboperations.cpp \
            bbsettings.cpp \
            bbobserver.cpp \
            bbsvn.cpp \
            bbsvnstatus.cpp \
            bbsvninfo.cpp \
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
