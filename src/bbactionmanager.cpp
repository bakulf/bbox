/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbactionmanager.h"

#include "bbactioncleanup.h"
#include "bbactionadd.h"
#include "bbactiondelete.h"
#include "bbactionlocalchanges.h"
#include "bbactionremotechanges.h"
#include "bbactionschedulecommit.h"
#include "bbactionscheduleupdate.h"
#include "bbdebug.h"

#include <QPointer>
#include <QCoreApplication>

BBActionManager::BBActionManager() :
    QObject(QCoreApplication::instance())
{
    BBDEBUG;
    startTimer(500);
}

BBActionManager::~BBActionManager()
{
    BBDEBUG;
}

BBActionManager* BBActionManager::instance()
{
    BBDEBUG;

    static QPointer<BBActionManager> instance;
    if (instance.isNull())
        instance = new BBActionManager();

    return instance;
}

void BBActionManager::actionCleanup()
{
    BBDEBUG;
    addAction(new BBActionCleanup(this));
}

void BBActionManager::actionLocalChanges()
{
    BBDEBUG;
    addAction(new BBActionLocalChanges(true, this));
}

void BBActionManager::actionRemoteChanges()
{
    BBDEBUG;
    addAction(new BBActionRemoteChanges(this));
}

void BBActionManager::actionScheduleCommit()
{
    BBDEBUG;
    addAction(new BBActionScheduleCommit(this));
}

void BBActionManager::actionScheduleUpdate()
{
    BBDEBUG;
    addAction(new BBActionScheduleUpdate(this));
}

void BBActionManager::addAction(BBAction *action)
{
    BBDEBUG << action;

    foreach (BBAction *eAction, m_actions) {
        if (eAction->compare(action)) {
            action->deleteLater();
            return;
        }
    }

    m_actions << action;
}

void BBActionManager::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (!m_currentAction.isNull())
        return;

    if (m_actions.isEmpty())
        return;

    m_currentAction = m_actions.takeFirst();

    connect(m_currentAction,
            SIGNAL(done(bool)),
            SLOT(onActionDone(bool)));

    m_currentAction->run();
}

void BBActionManager::onActionDone(bool status)
{
    BBDEBUG;
    Q_UNUSED(status)

    if (m_currentAction != (BBAction *)sender())
        return;

    m_currentAction->deleteLater();
    m_currentAction = 0;
}
