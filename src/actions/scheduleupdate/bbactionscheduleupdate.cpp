/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbactionscheduleupdate.h"

#include "bbapplication.h"
#include "bbdebug.h"

BBActionScheduleUpdate::BBActionScheduleUpdate(QObject *parent) :
    BBAction(parent)
{
    BBDEBUG;
}

BBActionScheduleUpdate::~BBActionScheduleUpdate()
{
    BBDEBUG;
}

bool BBActionScheduleUpdate::compare(const BBAction *action)
{
    BBDEBUG;

    const BBActionScheduleUpdate *a = qobject_cast<const BBActionScheduleUpdate *>(action);
    if (!a)
        return false;

    BBDEBUG << "Compare: Yes!";
    return true;
}

void BBActionScheduleUpdate::run()
{
    BBDEBUG;
    BBApplication::instance()->update();
    emit done(true);
}
