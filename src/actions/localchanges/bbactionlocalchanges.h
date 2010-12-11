/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

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
    bool commit() const { return m_commit; }

private:
    void checkStatus(const QList<BBSvnStatus*>& list);

private Q_SLOTS:
    void onSvnDone(bool status);

private:
    BBSvn *m_svn;
    bool m_commit;
};

#endif
