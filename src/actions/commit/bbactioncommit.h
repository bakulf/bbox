#ifndef _BB_ACTION_COMMIT_H_
#define _BB_ACTION_COMMIT_H_

#include "bbaction.h"

class BBSvn;

class BBActionCommit : public BBAction
{
    Q_OBJECT

public:
    BBActionCommit(QObject *parent = 0);
    ~BBActionCommit();

public:
    void run();
    bool compare(const BBAction *action);

private Q_SLOTS:
    void onSvnDone(bool status);

private:
    BBSvn *m_svn;
};

#endif
