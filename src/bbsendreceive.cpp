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
#include "bbdebug.h"

BBSendReceive::BBSendReceive(QObject *parent) :
    QObject(parent)
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

    BBApplication::instance()->observer()->operationOnFileSystemRef();

    m_action = new BBActionLocalChanges(false, this);
    connect(m_action,
            SIGNAL(done(bool)),
            SLOT(onActionLocalChangesDone(bool)));
    connect(m_action,
            SIGNAL(done(bool)),
            SIGNAL(localChangesDone(bool)));

    m_action->run();
}

void BBSendReceive::onActionLocalChangesDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        m_action->deleteLater();
        BBApplication::instance()->observer()->operationOnFileSystemUnref();
        emit done(false);
        return;
    }

    m_action->deleteLater();

    m_action = new BBActionUpdate(this);
    connect(m_action,
            SIGNAL(done(bool)),
            SLOT(onActionUpdateDone(bool)));
    connect(m_action,
            SIGNAL(done(bool)),
            SIGNAL(updateDone(bool)));

    m_action->run();
}

void BBSendReceive::onActionUpdateDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        m_action->deleteLater();
        BBApplication::instance()->observer()->operationOnFileSystemUnref();
        emit done(false);
        return;
    }

    m_action->deleteLater();

    m_action = new BBActionCommit(this);
    connect(m_action,
            SIGNAL(done(bool)),
            SLOT(onActionCommitDone(bool)));
    connect(m_action,
            SIGNAL(done(bool)),
            SIGNAL(commitDone(bool)));

    m_action->run();
}

void BBSendReceive::onActionCommitDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        BBApplication::instance()->observer()->operationOnFileSystemUnref();
        emit done(false);
        return;
    }

    m_action->deleteLater();

    m_action = new BBActionUpdate(this);
    connect(m_action,
            SIGNAL(done(bool)),
            SLOT(onActionRevisionDone(bool)));
    connect(m_action,
            SIGNAL(done(bool)),
            SIGNAL(revisionDone(bool)));

    m_action->run();
}

void BBSendReceive::onActionRevisionDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        BBApplication::instance()->observer()->operationOnFileSystemUnref();
        emit done(false);
        return;
    }

    m_action->deleteLater();

    BBApplication::instance()->observer()->operationOnFileSystemUnref();
    emit done(true);
}

void BBSendReceive::onDone(bool status)
{
    BBDEBUG;
    Q_UNUSED(status);
    m_running = false;
}
