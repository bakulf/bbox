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
#include "bbconst.h"
#include "bbdebug.h"

#ifdef BBFILESYSTEMWATCHER
    #include "bbfilesystemwatcher.h"
#else
    #include <QFileSystemWatcher>
#endif

#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

#ifdef Q_OS_WIN32
    #include <windows.h>
    #include <winbase.h>
#endif

BBObserver::BBObserver(QObject *parent) :
    QObject(parent),
    m_operationOnFs(0),
    m_remoteChangesScheduled(false)
{
    BBDEBUG;

    connect(BBSettings::instance(),
            SIGNAL(directoryChanged()),
            SLOT(directoryChanged()));

    // Maybe some crash:
    checkObstructedFiles(0);

    directoryChanged();

    connect(QCoreApplication::instance(),
            SIGNAL(aboutToQuit()),
            SLOT(onAboutToQuit()));

    m_timer.setInterval(BB_OBSERVER_TIMEOUT);
    m_timer.setSingleShot(false);
    connect(&m_timer,
            SIGNAL(timeout()),
            SLOT(onTimeout()));
}

BBObserver::~BBObserver()
{
    BBDEBUG;
}

void BBObserver::directoryChanged()
{
    BBDEBUG;

    BBActionManager::instance()->actionCleanup();
    BBActionManager::instance()->actionLocalChanges();

    if (!m_watcher.isNull())
        m_watcher->deleteLater();

#ifdef BBFILESYSTEMWATCHER
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
    QFileInfoList list = dir.entryInfoList(QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot);
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

    if (!m_changes.contains(filename))
        m_changes << filename;

    m_timer.start();
}

void BBObserver::onTimeout()
{
    BBDEBUG;

    if (m_changes.isEmpty()) {
        remoteChanges();
        return;
    }

    while (!m_changes.isEmpty()) {
        QString filename = m_changes.takeFirst();
        QFileInfo info(filename);

        if (!info.exists())
            m_watcher->removePath(filename);
        else if (info.isDir())
            addDirectory(filename);
    }

    remoteChanges();

    BBActionManager::instance()->actionLocalChanges();
}

void BBObserver::remoteChanges()
{
    if (m_remoteChangesScheduled) {
        m_remoteChangesScheduled = false;
        BBActionManager::instance()->actionRemoteChanges();
    }
}

void BBObserver::operationOnFileSystemRef()
{
    BBDEBUG << m_operationOnFs;
    ++m_operationOnFs;
}

void BBObserver::operationOnFileSystemUnref()
{
    BBDEBUG << m_operationOnFs;

    if (m_operationOnFs > 0)
        --m_operationOnFs;

    if (m_operationOnFs == 0) {
        onSomethingChanged(BBSettings::instance()->directory());
    }
}

void BBObserver::scheduleRemoteChanges()
{
    BBDEBUG;
    m_remoteChangesScheduled = true;

    if (m_timer.isActive() == false)
        m_timer.start();
}

void BBObserver::onAboutToQuit()
{
    BBDEBUG;
    if (!m_watcher.isNull())
        delete m_watcher;
}

bool BBObserver::checkObstructedFiles(bool *found)
{
    BBDEBUG;
    return checkObstructedFiles(BBSettings::instance()->directory(), found);
}

bool BBObserver::checkObstructedFiles(const QString& dirname, bool *found)
{
    BBDEBUG << dirname;

    QDir dir(dirname);
    QFileInfoList list = dir.entryInfoList(QDir::AllEntries | QDir::NoSymLinks | QDir::Hidden | QDir::NoDotAndDotDot);
    foreach (QFileInfo info, list) {
        if (info.fileName().startsWith(QString(".%1.").arg(BBPACKAGE))) {
            if (found)
                *found = true;

            QString oldFile(info.fileName());
            QString newFile(info.fileName().remove(QString(".%1.").arg(BBPACKAGE)));

            if (dir.rename(oldFile, newFile) == false)
                return false;
        }

        if (info.isDir() && checkObstructedFiles(info.absoluteFilePath(), found) == false)
            return false;
    }

    return true;
}
