#ifndef _BB_ACTION_LOCALCHANGES_H_
#define _BB_ACTION_LOCALCHANGES_H_

#include "bbaction.h"

class BBSvn;
class BBSvnStatus;

class BBActionLocalChanges : public BBAction
{
    Q_OBJECT

public:
    BBActionLocalChanges(bool commit, QObject *parent = 0);
    ~BBActionLocalChanges();

public:
    void run();
    bool compare(const BBAction *action);

private:
    void checkStatus(const QList<BBSvnStatus*>& list);

private Q_SLOTS:
    void onSvnDone(bool status);

private:
    BBSvn *m_svn;
    bool m_commit;
};

#endif
