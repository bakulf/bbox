/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbactionschedulecommit.h"

#include "bbapplication.h"
#include "bbdebug.h"

BBActionScheduleCommit::BBActionScheduleCommit(QObject *parent) :
    BBAction(parent)
{
    BBDEBUG;
}

BBActionScheduleCommit::~BBActionScheduleCommit()
{
    BBDEBUG;
}

bool BBActionScheduleCommit::compare(const BBAction *action)
{
    BBDEBUG;

    const BBActionScheduleCommit *a = qobject_cast<const BBActionScheduleCommit *>(action);
    if (!a)
        return false;

    BBDEBUG << "Compare: Yes!";
    return true;
}

void BBActionScheduleCommit::run()
{
    BBDEBUG;
    BBApplication::instance()->commit();
    emit done(true);
}
