/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_SVNMANAGER_H_
#define _BB_SVNMANAGER_H_

#include <QObject>
#include <QPointer>

class BBSvn;

class BBSvnManager : public QObject
{
    Q_OBJECT

private:
    BBSvnManager();

public:
    static BBSvnManager *instance();
    virtual ~BBSvnManager();

public:
    void registerForSchedule(BBSvn *svn);

private:
    void schedule(BBSvn *svn);

private Q_SLOTS:
    void unregister(QObject *obj);
    void onScheduledSvnDestroyed(QObject *obj);

    void onSvnDone();

private:
    QList <BBSvn*> m_svnList;
    QPointer<BBSvn> m_svnScheduled;
};

#endif
