/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_ACTION_SCHEDULECOMMIT_H_
#define _BB_ACTION_SCHEDULECOMMIT_H_

#include "bbaction.h"

class BBSvn;

class BBActionScheduleCommit : public BBAction
{
    Q_OBJECT

public:
    BBActionScheduleCommit(QObject *parent = 0);
    ~BBActionScheduleCommit();

public:
    void run();
    bool compare(const BBAction *action);
};

#endif
