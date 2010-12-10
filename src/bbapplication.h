#ifndef _BB_APPLICATION_H_
#define _BB_APPLICATION_H_

#include <QApplication>
#include <QPointer>

class QAction;
class QSystemTrayIcon;
class BBObserver;
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
    void init();

    BBObserver *observer();

    void addError(const QString& error);
    void commit();
    void update();

private:
    void systemTray();
    void blink(bool enabled);
    void timerEvent(QTimerEvent *event);

private Q_SLOTS:
    bool preferences();
    void about();

    void scheduleRemoteAction();
    void onActionsQueued(int counter);

    void onMessageClicked();

    void onCommitTriggered();

    void onSendReceiveDone(bool status);

private:
    QSystemTrayIcon *m_systemTray;

    QAction *m_actionCounter;
    QAction *m_actionCommit;

    bool m_errorShown;
    int m_timer;
    bool m_iconBlink;

    QPointer<BBObserver> m_observer;
    QPointer<BBSendReceive> m_sendReceive;

    static QPointer<BBApplication> m_instance;
};

#endif
