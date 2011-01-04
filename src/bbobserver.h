/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_OBSERVER_H_
#define _BB_OBSERVER_H_

#include <QObject>
#include <QPointer>
#include <QStringList>
#include <QTimer>

#ifdef BBFILESYSTEMWATCHER
class BBFileSystemWatcher;
#else
class QFileSystemWatcher;
#endif

class BBObserver : public QObject
{
    Q_OBJECT

public:
    BBObserver(QObject *parent = 0);
    ~BBObserver();

public:
    static bool checkObstructedFiles(bool *found);

private:
    static bool checkObstructedFiles(const QString& dirname, bool *found);

public:
    void operationOnFileSystemRef();
    void operationOnFileSystemUnref();

    void scheduleRemoteChanges();

private:
    void addDirectory(const QString &dirname);
    void checkEmptyDirectory(const QString &dirname);

private Q_SLOTS:
    void directoryChanged();
    void onSomethingChanged(const QString &filename);
    void onAboutToQuit();
    void onTimeout();

private:
#ifdef BBFILESYSTEMWATCHER
    QPointer<BBFileSystemWatcher> m_watcher;
#else
    QPointer<QFileSystemWatcher> m_watcher;
#endif

    uint m_operationOnFs;
    QStringList m_changes;
    QTimer m_timer;

    bool m_remoteChangesScheduled;
};

#endif
