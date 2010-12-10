#ifndef _BB_ACTION_CLEANUP_H_
#define _BB_ACTION_CLEANUP_H_

#include "bbaction.h"

class BBSvn;

class BBActionCleanup : public BBAction
{
    Q_OBJECT

public:
    BBActionCleanup(QObject *parent = 0);
    ~BBActionCleanup();

public:
    void run();
    bool compare(const BBAction *action);

private Q_SLOTS:
    void onSvnDone(bool status);

private:
    BBSvn *m_svn;
};

#endif
