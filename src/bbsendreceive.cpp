/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbsendreceive.h"

#include "bbactionlocalchanges.h"
#include "bbactionupdate.h"
#include "bbactioncommit.h"
#include "bbapplication.h"
#include "bbobserver.h"
#include "bbconst.h"
#include "bbdebug.h"

#include <QTimer>

BBSendReceive::BBSendReceive(QObject *parent) :
    QObject(parent),
    m_running(false)
{
    BBDEBUG;

    connect(this,
            SIGNAL(done(bool)),
            SLOT(onDone(bool)));
}

BBSendReceive::~BBSendReceive()
{
    BBDEBUG;
}

void BBSendReceive::start()
{
    BBDEBUG;

    m_running = true;
    m_commitStatus = BBCommitUnknown;

    BBApplication::instance()->observer()->operationOnFileSystemRef();

    checkCommitStatus();
}

void BBSendReceive::checkCommitStatus()
{
    BBDEBUG << m_commitStatus;

    if (m_commitStatus == BBCommitTooMuch) {
        emit commitDone(false);
        emit done(false);
        return;
    }

    m_commitStatus = (BBCommitStatus) ((int)m_commitStatus + 1);

    switch (m_commitStatus) {
        case BBCommitUnknown:
        case BBCommitOk:
        case BBCommitFailed1:
            onCommitTimeout();
            break;

        default:
            QTimer::singleShot(BB_COMMIT_TIMEOUT, this, SLOT(onCommitTimeout()));
            break;
    }
}

void BBSendReceive::onCommitTimeout()
{
    BBDEBUG;

    BBActionLocalChanges *action = new BBActionLocalChanges(false, this);
    connect(action,
            SIGNAL(done(bool)),
            SLOT(onActionLocalChangesDone(bool)));
    connect(action,
            SIGNAL(done(bool)),
            SIGNAL(localChangesDone(bool)));
    connect(action,
            SIGNAL(done(bool)),
            action,
            SLOT(deleteLater()));

    action->run();
}

void BBSendReceive::onActionLocalChangesDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        emit done(false);
        return;
    }

    BBAction *action;
    if (m_commitStatus == BBCommitOk) {
        action = new BBActionCommit(false, this);
        connect(action,
                SIGNAL(done(bool)),
                SLOT(onActionCommitDone(bool)));
    } else {
        action = new BBActionUpdate((m_commitStatus == BBCommitLast), this);
        connect(action,
                SIGNAL(done(bool)),
                SLOT(onActionUpdateDone(bool)));
    }

    connect(action,
            SIGNAL(done(bool)),
            action,
            SLOT(deleteLater()));

    action->run();
}

void BBSendReceive::onActionCommitDone(bool status)
{
    BBDEBUG;

    // If the commit fails, we need to check the status:
    if (status == false) {
        checkCommitStatus();
        return;
    }

    emit commitDone(true);

    BBActionUpdate *action = new BBActionUpdate((m_commitStatus == BBCommitLast), this);
    connect(action,
            SIGNAL(done(bool)),
            SLOT(onActionRevisionDone(bool)));
    connect(action,
            SIGNAL(done(bool)),
            SIGNAL(revisionDone(bool)));
    connect(action,
            SIGNAL(done(bool)),
            action,
            SLOT(deleteLater()));

    action->run();
}

// If this method is called, we need to procede again with the 'commit'
void BBSendReceive::onActionUpdateDone(bool status)
{
    BBDEBUG;

    // If the commit fails, we need to check the status:
    if (status == false) {
        checkCommitStatus();
        return;
    }

    BBActionCommit *action = new BBActionCommit((m_commitStatus == BBCommitLast), this);
    connect(action,
            SIGNAL(done(bool)),
            SLOT(onActionCommitDone(bool)));
    connect(action,
            SIGNAL(done(bool)),
            action,
            SLOT(deleteLater()));

    action->run();
}

void BBSendReceive::onActionRevisionDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        emit done(false);
        return;
    }

    emit done(true);
}

void BBSendReceive::onDone(bool status)
{
    BBDEBUG << status;
    Q_UNUSED(status);

    m_running = false;
    BBApplication::instance()->observer()->operationOnFileSystemUnref();
}
