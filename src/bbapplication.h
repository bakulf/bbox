#ifndef _BB_APPLICATION_H_
#define _BB_APPLICATION_H_

#include <QApplication>
#include <QPointer>

class QAction;
class QSystemTrayIcon;

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

private:
    QSystemTrayIcon *m_systemTray;

    QAction *m_actionCounter;
    QAction *m_actionCommit;

    bool m_errorShown;
    int m_timer;
    bool m_iconBlink;

    static QPointer<BBApplication> m_instance;
};

#endif
