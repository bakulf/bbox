/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbsvnmanager.h"

#include "bbsvn.h"
#include "bbdebug.h"

#include <QCoreApplication>

BBSvnManager::BBSvnManager() :
    QObject(QCoreApplication::instance())
{
    BBDEBUG;
}

BBSvnManager::~BBSvnManager()
{
    BBDEBUG;
}

BBSvnManager* BBSvnManager::instance()
{
    BBDEBUG;

    static QPointer<BBSvnManager> instance;
    if (instance.isNull())
        instance = new BBSvnManager();

    return instance;
}

void BBSvnManager::registerForSchedule(BBSvn *svn)
{
    BBDEBUG << svn;

    if (m_svnScheduled.isNull() && m_svnList.isEmpty()) {
        schedule(svn);
        return;
    }

    connect(svn,
            SIGNAL(destroyed(QObject*)),
            SLOT(unregister(QObject*)));

    m_svnList << svn;
}

void BBSvnManager::unregister(QObject *obj)
{
    BBDEBUG;
    BBSvn *svn = (BBSvn*) obj;
    m_svnList.removeAll(svn);
}

void BBSvnManager::schedule(BBSvn *svn)
{
    BBDEBUG << svn;

    svn->disconnect(this);
    connect(svn,
            SIGNAL(done(bool)),
            SLOT(onSvnDone()));
    connect(svn,
            SIGNAL(destroyed(QObject*)),
            SLOT(onScheduledSvnDestroyed(QObject*)));

    m_svnScheduled = svn;
    svn->schedule();
}

void BBSvnManager::onSvnDone()
{
    BBDEBUG;
    BBSvn *svn = qobject_cast<BBSvn*>(sender());

    if (!svn || svn != m_svnScheduled)
        return;

    m_svnScheduled = 0;
    svn->disconnect(this);

    if (!m_svnList.isEmpty()) {
        BBSvn *svn = m_svnList.takeFirst();
        schedule(svn);
    }
}

void BBSvnManager::onScheduledSvnDestroyed(QObject *object)
{
    BBDEBUG;
    BBSvn *svn = (BBSvn*) object;
    if (svn == m_svnScheduled) {
        m_svnScheduled = 0;
    }

    if (!m_svnList.isEmpty()) {
        BBSvn *svn = m_svnList.takeFirst();
        schedule(svn);
    }
}
