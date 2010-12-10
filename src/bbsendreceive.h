/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_SENDRECEIVE_H_
#define _BB_SENDRECEIVE_H_

#include <QObject>

class BBAction;

class BBSendReceive : public QObject
{
    Q_OBJECT

public:
    BBSendReceive(QObject *parent = 0);
    virtual ~BBSendReceive();

public:
    void start();
    bool isRunning() { return m_running; }

Q_SIGNALS:
    // The order of this signal is:
    // localChanges -> update -> commit -> revision -> done
    // if something goes wrong, done is emitted with false
    void localChangesDone(bool status);
    void updateDone(bool status);
    void commitDone(bool status);
    void revisionDone(bool status);
    void done(bool status);

private Q_SLOTS:
    void onActionLocalChangesDone(bool status);
    void onActionUpdateDone(bool status);
    void onActionCommitDone(bool status);
    void onActionRevisionDone(bool status);
    void onDone(bool status);

private:
    BBAction *m_action;

    bool m_running;
};

#endif
