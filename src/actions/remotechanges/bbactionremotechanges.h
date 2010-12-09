#ifndef _BB_ACTION_REMOTECHANGES_H_
#define _BB_ACTION_REMOTECHANGES_H_

#include "bbaction.h"

class BBSvn;

class BBActionRemoteChanges : public BBAction
{
    Q_OBJECT

public:
    BBActionRemoteChanges(QObject *parent = 0);
    ~BBActionRemoteChanges();

public:
    void run();
    bool compare(const BBAction *action);

private Q_SLOTS:
    void onRemoteSvnDone(bool status);
    void onLocalSvnDone(bool status);

private:
    void check();

private:
    BBSvn *m_svn;
    uint m_localRevision;
    uint m_remoteRevision;
};

#endif
