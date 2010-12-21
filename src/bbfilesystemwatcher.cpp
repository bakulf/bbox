/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbfilesystemwatcher.h"

#include "bbdebug.h"

#include <QFileInfo>

BBFileSystemWatcher::BBFileSystemWatcher(QObject *parent) :
    QObject(parent),
    m_dateTime(QDateTime::currentDateTime())
{
    BBDEBUG;

    startTimer(5000);
}

BBFileSystemWatcher::~BBFileSystemWatcher()
{
    BBDEBUG;
}

void BBFileSystemWatcher::timerEvent(QTimerEvent *event)
{
    BBDEBUG;
    Q_UNUSED(event);

    QDateTime now = QDateTime::currentDateTime();

    foreach(QString path, m_paths) {
        QFileInfo info(path);

        if (info.exists() == false ||
            info.lastModified() >= m_dateTime ||
            info.created() >= m_dateTime) {
            emit fileChanged(path);
        }
    }

    m_dateTime = now;
}

void BBFileSystemWatcher::addPath(const QString& path)
{
    BBDEBUG << path;

    if (!m_paths.contains(path))
        m_paths << path;
}

void BBFileSystemWatcher::removePath(const QString& path)
{
    BBDEBUG << path;
    m_paths.removeAll(path);
}
