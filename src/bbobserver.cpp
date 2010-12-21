/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbobserver.h"

#include "bbsettings.h"
#include "bbactionmanager.h"
#include "bbdebug.h"

#ifdef Q_OS_MAC
#include "bbfilesystemwatcher.h"
#else
#include <QFileSystemWatcher>
#endif

#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

BBObserver::BBObserver(QObject *parent) :
    QObject(parent),
    m_operationOnFs(0)
{
    BBDEBUG;

    connect(BBSettings::instance(),
            SIGNAL(directoryChanged()),
            SLOT(directoryChanged()));

    directoryChanged();

    connect(QCoreApplication::instance(),
            SIGNAL(aboutToQuit()),
            SLOT(onAboutToQuit()));

    startTimer(1000);
}

BBObserver::~BBObserver()
{
    BBDEBUG;
}

void BBObserver::directoryChanged()
{
    BBDEBUG;

    BBActionManager::instance()->actionCleanup();
    BBActionManager::instance()->actionAdd(BBSettings::instance()->directory());
    BBActionManager::instance()->actionLocalChanges();

    if (!m_watcher.isNull())
        m_watcher->deleteLater();

#ifdef Q_OS_MAC
    m_watcher = new BBFileSystemWatcher(this);
#else
    m_watcher = new QFileSystemWatcher(this);
#endif

    connect(m_watcher,
            SIGNAL(directoryChanged(QString)),
            SLOT(onSomethingChanged(QString)));
    connect(m_watcher,
            SIGNAL(fileChanged(QString)),
            SLOT(onSomethingChanged(QString)));

    QString dir = BBSettings::instance()->directory();

    QFileInfo info(dir);
    if (!info.exists() || !info.isDir()) {
        QMessageBox::critical(0,
                              BBPACKAGE,
                              tr("The directory '%1' doesn't exist. Please, change it in the preferences!").arg(dir));
        return;
    }

    addDirectory(dir);
}

void BBObserver::addDirectory(const QString& dirname)
{
    BBDEBUG << dirname;

    m_watcher->addPath(dirname);

    QDir dir(dirname);
    QFileInfoList list = dir.entryInfoList();
    foreach (QFileInfo info, list) {
        if (info.fileName().startsWith("."))
            continue;

        addDirectory(info.absoluteFilePath());
    }
}

void BBObserver::onSomethingChanged(const QString &filename)
{
    BBDEBUG << filename << m_operationOnFs;

    if (m_operationOnFs)
        return;

    QFileInfo info(filename);

    if (!info.exists())
        m_watcher->removePath(filename);
    else if (info.isDir())
        addDirectory(filename);

    if (!m_changes.contains(filename))
        m_changes << filename;
}

void BBObserver::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (m_changes.isEmpty())
        return;

    foreach(QString filename, m_changes) {
        BBActionManager::instance()->actionAdd(filename);
        BBActionManager::instance()->actionLocalChanges();
    }

    m_changes.clear();
}

void BBObserver::operationOnFileSystemRef()
{
    BBDEBUG << m_operationOnFs;
    m_operationOnFs++;
}

void BBObserver::operationOnFileSystemUnref()
{
    BBDEBUG << m_operationOnFs;

    if (m_operationOnFs > 0)
        m_operationOnFs--;

    if (m_operationOnFs == 0) {
        onSomethingChanged(BBSettings::instance()->directory());
    }
}

void BBObserver::onAboutToQuit()
{
    BBDEBUG;
    if (!m_watcher.isNull())
        delete m_watcher;
}
