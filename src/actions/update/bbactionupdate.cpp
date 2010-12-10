/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbactionupdate.h"

#include "bbsvn.h"
#include "bbsvnstatus.h"
#include "bbconflict.h"
#include "bbapplication.h"
#include "bbdebug.h"

BBActionUpdate::BBActionUpdate(QObject *parent) :
    BBAction(parent)
{
    BBDEBUG;
}

BBActionUpdate::~BBActionUpdate()
{
    BBDEBUG;
}

bool BBActionUpdate::compare(const BBAction *action)
{
    BBDEBUG;

    const BBActionUpdate *a = qobject_cast<const BBActionUpdate *>(action);
    if (!a)
        return false;

    return true;
}

void BBActionUpdate::run()
{
    BBDEBUG;

    m_svn = new BBSvn(this);
    connect(m_svn,
            SIGNAL(done(bool)),
            SLOT(onSvnDone(bool)));

    m_svn->update();
}

void BBActionUpdate::onSvnDone(bool status)
{
    BBDEBUG << status;

    if (status == false) {
        BBApplication::instance()->addError(tr("Error updating the directory."));
    }

    BBConflict::check(m_svn->parseUpdate());

    m_svn->deleteLater();
    emit done(status);
}
