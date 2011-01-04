/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_ACTION_UPDATE_H_
#define _BB_ACTION_UPDATE_H_

#include "bbaction.h"

class BBSvn;

class BBActionUpdate : public BBAction
{
    Q_OBJECT

public:
    BBActionUpdate(bool withError, QObject *parent = 0);
    ~BBActionUpdate();

public:
    void run();
    bool compare(const BBAction *action);

private Q_SLOTS:
    void onSvnDone(bool status);

private:
    BBSvn *m_svn;
    bool m_withError;
};

#endif
