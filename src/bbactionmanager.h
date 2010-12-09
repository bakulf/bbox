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
    void actionAdd(const QString &dirname);
    void actionDelete(const QString &filename);
    void actionLocalChanges();
    void actionRemoteChanges();

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
