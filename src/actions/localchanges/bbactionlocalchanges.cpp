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
            SLOT(onSvnDone(bool)));

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
    emit done(status);
}

void BBActionLocalChanges::checkStatus(const QList<BBSvnStatus *>& list)
{
    BBDEBUG << list;

    if (list.isEmpty())
        return;

    BBConflict::check(list);

    foreach (BBSvnStatus *status, list) {
        if (status->locked() == true) {
            BBApplication::instance()->addError(tr("Directory is locked."));
            return;
        }

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

            case BBSvnStatus::StatusMissing:
                BBActionManager::instance()->actionDelete(status->file());
                break;

            case BBSvnStatus::StatusUpdated:
            case BBSvnStatus::StatusMerged:
            case BBSvnStatus::StatusExisted:
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
