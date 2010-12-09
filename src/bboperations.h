#ifndef _BB_OPERATIONS_H_
#define _BB_OPERATIONS_H_

#include <QDialog>

class QLabel;
class QPushButton;
class BBAction;

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

    void actionLocalChanges();

private Q_SLOTS:
    void onActionLocalChangesDone(bool status);
    void onActionUpdateDone(bool status);
    void onActionCommitDone(bool status);
    void onActionRevisionDone(bool status);

private:
    QLabel *m_localChangesStatus;
    QLabel *m_updateStatus;
    QLabel *m_commitStatus;
    QLabel *m_revisionStatus;
    QPushButton *m_closeButton;

    BBAction *m_action;
};

#endif
