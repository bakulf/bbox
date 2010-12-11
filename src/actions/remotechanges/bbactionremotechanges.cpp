/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbactionremotechanges.h"

#include "bbapplication.h"
#include "bbactionmanager.h"
#include "bbsvn.h"
#include "bbsvninfo.h"
#include "bbdebug.h"

BBActionRemoteChanges::BBActionRemoteChanges(QObject *parent) :
    BBAction(parent),
    m_localRevision(0),
    m_remoteRevision(0)
{
    BBDEBUG;
}

BBActionRemoteChanges::~BBActionRemoteChanges()
{
    BBDEBUG;
}

void BBActionRemoteChanges::run()
{
    BBDEBUG;

    m_svn = new BBSvn(this);
    connect(m_svn,
            SIGNAL(done(bool)),
            SLOT(onLocalSvnDone(bool)));

    m_svn->localInfo();
}

void BBActionRemoteChanges::onLocalSvnDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        BBApplication::instance()->addError(tr("Error checking the local status."));
    }

    BBSvnInfo* info = m_svn->parseInfo();
    m_localRevision = info->revision();

    m_svn->deleteLater();

    m_svn = new BBSvn(this);
    connect(m_svn,
            SIGNAL(done(bool)),
            SLOT(onRemoteSvnDone(bool)));

    m_svn->remoteInfo(info->URL());
}

void BBActionRemoteChanges::onRemoteSvnDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        BBApplication::instance()->addError(tr("Error checking the remote status."));
    }

    BBSvnInfo* info = m_svn->parseInfo();
    m_remoteRevision = info->revision();

    m_svn->deleteLater();

    check();
}

void BBActionRemoteChanges::check()
{
    if (m_remoteRevision > m_localRevision)
        BBActionManager::instance()->actionScheduleUpdate();

    emit done(true);
}

bool BBActionRemoteChanges::compare(const BBAction *action)
{
    BBDEBUG;

    const BBActionRemoteChanges *a = qobject_cast<const BBActionRemoteChanges *>(action);
    if (!a)
        return false;

    BBDEBUG << "Yes!";
    return true;
}
