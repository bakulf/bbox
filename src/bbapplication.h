/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_APPLICATION_H_
#define _BB_APPLICATION_H_

#include <QApplication>
#include <QPointer>
#include <QSystemTrayIcon>

class QMenu;
class QAction;
class BBObserver;
class BBSvnStatus;
class BBSendReceive;

class BBApplication : public QApplication
{
    Q_OBJECT

public:
    BBApplication(int argc, char **argv);
    ~BBApplication();

    static BBApplication *instance();

public:
    void splash();

    BBObserver *observer();

    void addError(const QString& error);
    void commit();
    void update();
    void changes(const QList<BBSvnStatus*>& changes);

private:
    void systemTray();
    void blink(bool enabled);
    void timerEvent(QTimerEvent *event);
    void showMessage(const QString &title, const QString &message, bool isWarning = false);
    void resetLastMessage();

public Q_SLOTS:
    void init();

private Q_SLOTS:
    bool preferences();
    void about();

    void scheduleRemoteAction();

    void onActivated(QSystemTrayIcon::ActivationReason);

    void onCommitTriggered();
    void onOpenTriggered();
    void onLogsTriggered();

    void onSendReceiveDone(bool status);

private:
    QSystemTrayIcon *m_systemTray;

    QAction *m_actionCommit;

    QMenu *m_menuChanges;

    int m_timer;
    bool m_iconBlink;

    QPointer<BBObserver> m_observer;
    QPointer<BBSendReceive> m_sendReceive;

    static QPointer<BBApplication> m_instance;

    QString m_lastMessage;
};

#endif
