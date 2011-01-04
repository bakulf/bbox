/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_ACTIONMANAGER_H_
#define _BB_ACTIONMANAGER_H_

#include <QObject>
#include <QPointer>

class BBAction;

class BBActionManager : public QObject
{
    Q_OBJECT

private:
    BBActionManager();

public:
    static BBActionManager *instance();
    virtual ~BBActionManager();

public Q_SLOTS:
    void actionCleanup();
    void actionLocalChanges();
    void actionRemoteChanges();
    void actionScheduleCommit();
    void actionScheduleUpdate();

Q_SIGNALS:
    void actionsQueued(int actions);

private Q_SLOTS:
    void onActionDone(bool status);

protected:
     void timerEvent(QTimerEvent *event);

private:
    void addAction(BBAction *action);
    void emitActionsQueued();

private:
    QList<BBAction *> m_actions;
    QPointer<BBAction> m_currentAction;
};

#endif
