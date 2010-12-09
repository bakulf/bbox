#ifndef _BB_ACTION_UPDATE_H_
#define _BB_ACTION_UPDATE_H_

#include "bbaction.h"

class BBSvn;

class BBActionUpdate : public BBAction
{
    Q_OBJECT

public:
    BBActionUpdate(QObject *parent = 0);
    ~BBActionUpdate();

public:
    void run();
    bool compare(const BBAction *action);

private Q_SLOTS:
    void onSvnDone(bool status);

private:
    BBSvn *m_svn;
};

#endif
