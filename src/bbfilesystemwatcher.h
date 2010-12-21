/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_FILESYSTEMWATCHER_H_
#define _BB_FILESYSTEMWATCHER_H_

#include <QObject>
#include <QDateTime>

class BBFileSystemWatcher : public QObject
{
    Q_OBJECT

public:
    BBFileSystemWatcher(QObject *parent = 0);
    ~BBFileSystemWatcher();

public:
    void addPath(const QString& path);
    void removePath(const QString& path);

Q_SIGNALS:
    void fileChanged(const QString& path);

private:
    void timerEvent(QTimerEvent *event);

private:
    QList<QString> m_paths;
    QDateTime m_dateTime;
};

#endif
