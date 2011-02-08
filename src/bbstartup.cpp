/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbstartup.h"

#include "bbsettings.h"
#include "bbdebug.h"
#include "bbconst.h"

#include <QDir>
#include <QFile>
#include <QPointer>
#include <QCoreApplication>

BBStartup::BBStartup() :
    QObject(QCoreApplication::instance())
{
    BBDEBUG;

    connect(BBSettings::instance(),
            SIGNAL(runOnStartupChanged()),
            SLOT(reset()));

    reset();
}

BBStartup::~BBStartup()
{
    BBDEBUG;
}

BBStartup* BBStartup::instance()
{
    BBDEBUG;

    static QPointer<BBStartup> instance;
    if (instance.isNull())
        instance = new BBStartup();

    return instance;
}

void BBStartup::reset()
{
    BBDEBUG;

    if (BBSettings::instance()->runOnStartup())
        addStartup();
    else
        removeStartup();
}

#if Q_OS_WIN32
void BBStartup::addStartup()
{
    BBDEBUG;

    QSettings settings(BB_WIN_STARTUP_REGEDIT, QSettings::NativeFormat);
    settings.setValue(BB_WIN_STARTUP_KEY, QCoreApplication::applicationFilePath().replace('/','\\'));
}
#elif Q_OS_MAC
void BBStartup::addStartup()
{
    BBDEBUG;

    QDir dir(QString("%1/%2").arg(QDir::homePath()).arg(BB_MAC_STARTUP_PATH));
    QFile file(dir.absoluteFilePath(BB_MAC_STARTUP_FILE));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    file.write(BB_MAC_STARTUP_CONTENT.arg(QCoreApplication::applicationFilePath()).toUtf8());
    file.close();
}
#else
void BBStartup::addStartup()
{
    BBDEBUG;

    QDir dir(QString("%1/%2").arg(QDir::homePath()).arg(BB_LINUX_STARTUP_PATH));
    QFile file(dir.absoluteFilePath(BB_LINUX_STARTUP_FILE));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    file.write(BB_LINUX_STARTUP_CONTENT.toUtf8());
    file.close();
}
#endif

#if Q_OS_WIN32
void BBStartup::removeStartup()
{
    BBDEBUG;

    QSettings settings(BB_WIN_STARTUP_REGEDIT, QSettings::NativeFormat);
    settings.remove(BB_WIN_STARTUP_KEY);
}
#elif Q_OS_MAC
void BBStartup::removeStartup()
{
    BBDEBUG;

    QDir dir(QString("%1/%2").arg(QDir::homePath()).arg(BB_MAC_STARTUP_PATH));
    dir.remove(BB_MAC_STARTUP_FILE);
}
#else
void BBStartup::removeStartup()
{
    BBDEBUG;

    QDir dir(QString("%1/%2").arg(QDir::homePath()).arg(BB_LINUX_STARTUP_PATH));
    dir.remove(BB_LINUX_STARTUP_FILE);
}
#endif
