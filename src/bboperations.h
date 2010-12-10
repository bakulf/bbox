#ifndef _BB_OPERATIONS_H_
#define _BB_OPERATIONS_H_

#include <QDialog>
#include <QPointer>

class QLabel;
class QPushButton;
class BBSendReceive;

class BBOperations : public QDialog
{
    Q_OBJECT

    enum Status {
        Waiting,
        Running,
        Error,
        Done
    };

public:
    BBOperations();
    virtual ~BBOperations();

private:
    void updateStatus(QLabel *label, Status status);

    void start();

private Q_SLOTS:
    void onLocalChangesDone(bool status);
    void onUpdateDone(bool status);
    void onCommitDone(bool status);
    void onRevisionDone(bool status);
    void onDone(bool status);

private:
    QLabel *m_localChangesStatus;
    QLabel *m_updateStatus;
    QLabel *m_commitStatus;
    QLabel *m_revisionStatus;
    QPushButton *m_closeButton;

    QPointer<BBSendReceive> m_sendReceive;
};

#endif
