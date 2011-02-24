/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbactionlocalchanges.h"

#include "bbsvn.h"
#include "bbsvnstatus.h"
#include "bbconflict.h"
#include "bbapplication.h"
#include "bbactionmanager.h"
#include "bbactionadd.h"
#include "bbactiondelete.h"
#include "bbactionobstructed.h"
#include "bbconst.h"
#include "bbdebug.h"

BBActionLocalChanges::BBActionLocalChanges(bool commit, QObject *parent) :
    BBAction(parent),
    m_commit(commit)
{
    BBDEBUG;
}

BBActionLocalChanges::~BBActionLocalChanges()
{
    BBDEBUG;
}

void BBActionLocalChanges::run()
{
    BBDEBUG;

    m_svn = new BBSvn(this);
    connect(m_svn,
            SIGNAL(done(bool)),
            SLOT(onSvnDone(bool)),
            Qt::QueuedConnection);

    m_svn->status();
}

void BBActionLocalChanges::onSvnDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        BBApplication::instance()->addError(tr("Error checking the local status."));
    } else {
        QList<BBSvnStatus*> list = m_svn->parseStatus();
        checkStatus(list);
    }

    m_svn->deleteLater();

    runActions(status);
}

void BBActionLocalChanges::runActions(bool status)
{
    BBDEBUG << status << m_actions;

    if (status == false) {
        while (!m_actions.isEmpty())
            m_actions.takeFirst()->deleteLater();

        emit done(status);
        return;
    }

    if (m_actions.isEmpty()) {
        emit done(status);
        return;
    }

    BBAction *action = m_actions.takeFirst();
    connect(action,
            SIGNAL(done(bool)),
            SLOT(runActions(bool)));
    connect(action,
            SIGNAL(done(bool)),
            action,
            SLOT(deleteLater()));

    action->run();
}

void BBActionLocalChanges::checkStatus(const QList<BBSvnStatus *>& list)
{
    BBDEBUG << list;

    if (list.isEmpty())
        return;

    BBConflict::check(list);

    foreach (BBSvnStatus *status, list) {

        QString fileName(QFileInfo(status->file()).fileName());
        if (fileName.startsWith("."))
            continue;

        switch (status->status()) {
            case BBSvnStatus::StatusUnknown:
            case BBSvnStatus::StatusAdded:
                break;

            case BBSvnStatus::StatusConflicted:
                break;

            case BBSvnStatus::StatusDeleted:
            case BBSvnStatus::StatusModified:
            case BBSvnStatus::StatusReplaced:
                break;

            case BBSvnStatus::StatusNew:
                if (!m_commit)
                    m_actions <<  new BBActionAdd(status->file(), this);
                break;

            case BBSvnStatus::StatusMissing:
                if (!m_commit)
                    m_actions <<  new BBActionDelete(status->file(), this);
                break;

            case BBSvnStatus::StatusUpdated:
            case BBSvnStatus::StatusMerged:
            case BBSvnStatus::StatusExisted:
                break;

            case BBSvnStatus::StatusObstructed:
                if (!m_commit)
                    m_actions << new BBActionObstructed(status->file(), this);
                break;
        }
    }

    if (m_commit) {
        BBApplication::instance()->changes(list);
        BBActionManager::instance()->actionScheduleCommit();
    }
}

bool BBActionLocalChanges::compare(const BBAction *action)
{
    BBDEBUG;

    const BBActionLocalChanges *a = qobject_cast<const BBActionLocalChanges *>(action);
    if (!a)
        return false;

    BBDEBUG << m_commit << a->commit();
    return (m_commit == a->commit());
}
