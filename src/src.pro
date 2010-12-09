TEMPLATE  = app
TARGET    = bbox
QT       += network

MOC_DIR     = .moc
OBJECTS_DIR = .obj

QMAKE_CXXFLAGS *= -Werror

DEFINES = BBPACKAGE=\\\"BBox\\\"

INCLUDEPATH += actions \
               actions/add \
               actions/delete \
               actions/update \
               actions/commit \
               actions/localchanges \
               actions/remotechanges

DEPENDPATH  += $${INCLUDEPATH}

HEADERS   = bbapplication.h \
            bbpreferences.h \
            bbabout.h \
            bboperations.h \
            bbsettings.h \
            bbobserver.h \
            bbsvn.h \
            bbsvnstatus.h \
            bbsvninfo.h \
            bbconflict.h \
            bbconflictfile.h \
            bbactionmanager.h \
            bbaction.h \
            bbactionadd.h \
            bbactiondelete.h \
            bbactionupdate.h \
            bbactioncommit.h \
            bbactionlocalchanges.h \
            bbactionremotechanges.h

SOURCES   = main.cpp \
            bbapplication.cpp \
            bbabout.cpp \
            bbpreferences.cpp \
            bboperations.cpp \
            bbsettings.cpp \
            bbobserver.cpp \
            bbsvn.cpp \
            bbsvnstatus.cpp \
            bbsvninfo.cpp \
            bbconflict.cpp \
            bbconflictfile.cpp \
            bbactionmanager.cpp \
            bbaction.cpp \
            bbactionadd.cpp \
            bbactiondelete.cpp \
            bbactionupdate.cpp \
            bbactioncommit.cpp \
            bbactionlocalchanges.cpp \
            bbactionremotechanges.cpp

RESOURCES = bbox.qrc
