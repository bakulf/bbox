/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbactioncommit.h"

#include "bbsvn.h"
#include "bbobserver.h"
#include "bbapplication.h"
#include "bbactionlocalchanges.h"
#include "bbdebug.h"

#include <QFile>

BBActionCommit::BBActionCommit(bool withError, QObject *parent) :
    BBAction(parent),
    m_withError(withError)
{
    BBDEBUG;
}

BBActionCommit::~BBActionCommit()
{
    BBDEBUG;
}

bool BBActionCommit::compare(const BBAction *action)
{
    BBDEBUG;

    const BBActionCommit *a = qobject_cast<const BBActionCommit *>(action);
    if (!a)
        return false;

    return true;
}

void BBActionCommit::run()
{
    BBDEBUG;

    BBSvn *svn = new BBSvn(this);
    connect(svn,
            SIGNAL(done(bool)),
            SLOT(onSvnDone(bool)),
            Qt::QueuedConnection);
    connect(svn,
            SIGNAL(done(bool)),
            svn,
            SLOT(deleteLater()),
            Qt::QueuedConnection);

    svn->commit();
}

void BBActionCommit::onSvnDone(bool status)
{
    BBDEBUG << status;

    if (status == false) {
        if (m_withError)
            BBApplication::instance()->addError(tr("Error committing files."));
        emit done(false);
        return;
    }

    // Remove the changes:
    BBApplication::instance()->changes(QList<BBSvnStatus*>());

    // Let's check obstructed files:
    bool found(false);
    if (BBObserver::checkObstructedFiles(&found) == false) {
        if (m_withError)
            BBApplication::instance()->addError(tr("Error committing files."));
        emit done(false);
        return;
    }

    if (found == false) {
        emit done(true);
        return;
    }

    // Another loop of add/delete & C:
    BBActionLocalChanges *action = new BBActionLocalChanges(false, this);
    connect(action,
            SIGNAL(done(bool)),
            SLOT(onLocalChangesDone(bool)));
    connect(action,
            SIGNAL(done(bool)),
            action,
            SLOT(deleteLater()));
    action->run();
}

void BBActionCommit::onLocalChangesDone(bool status)
{
    if (status == false) {
        if (m_withError)
            BBApplication::instance()->addError(tr("Error committing files."));
        emit done(false);
        return;
    }

    // The last commit:
    BBSvn *svn = new BBSvn(this);
    connect(svn,
            SIGNAL(done(bool)),
            SLOT(onSvnDone(bool)),
            Qt::QueuedConnection);
    connect(svn,
            SIGNAL(done(bool)),
            svn,
            SLOT(deleteLater()),
            Qt::QueuedConnection);

    svn->commit();
}
