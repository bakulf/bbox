/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbapplication.h"

#include "bbdebug.h"

#include <QtGui>

#ifdef BBDEBUG_ENABLED
void debugHandler(QtMsgType type, const char *msg)
{
    QString txt;

    switch (type) {
        case QtDebugMsg:
            txt = QString("Debug: %1").arg(msg);
            break;
        case QtWarningMsg:
            txt = QString("Warning: %1").arg(msg);
            break;
        case QtCriticalMsg:
            txt = QString("Critical: %1").arg(msg);
            break;
        case QtFatalMsg:
            txt = QString("Fatal: %1").arg(msg);
            abort();
    }

    QFile outFile(QDir::home().absoluteFilePath(BBDEBUG_LOGFILE));
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}
#endif

int main(int argc, char **argv)
{
#ifdef BBDEBUG_ENABLED
    QDir::home().remove(BBDEBUG_LOGFILE);
    qInstallMsgHandler(debugHandler);
#endif

    BBApplication app(argc, argv);

    if(!QSystemTrayIcon::isSystemTrayAvailable()) {
       QMessageBox::critical(0,
                             QObject::tr("Systray"),
                             QObject::tr("I cannot detect any system tray on this system."));
       return 1;
    }

    app.setQuitOnLastWindowClosed(false);

    app.splash();

    QTimer::singleShot(0, &app, SLOT(init()));
    return app.exec();
}
