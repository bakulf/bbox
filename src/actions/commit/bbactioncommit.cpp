/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbactioncommit.h"

#include "bbsvn.h"
#include "bbapplication.h"
#include "bbdebug.h"

#include <QFile>

BBActionCommit::BBActionCommit(QObject *parent) :
    BBAction(parent)
{
    BBDEBUG;
}

BBActionCommit::~BBActionCommit()
{
    BBDEBUG;
}

bool BBActionCommit::compare(const BBAction *action)
{
    BBDEBUG;

    const BBActionCommit *a = qobject_cast<const BBActionCommit *>(action);
    if (!a)
        return false;

    return true;
}

void BBActionCommit::run()
{
    BBDEBUG;

    m_svn = new BBSvn(this);
    connect(m_svn,
            SIGNAL(done(bool)),
            SLOT(onSvnDone(bool)));

    m_svn->commit();
}

void BBActionCommit::onSvnDone(bool status)
{
    BBDEBUG << status;

    if (status == false) {
        BBApplication::instance()->addError(tr("Error committing files."));
    }

    m_svn->deleteLater();
    emit done(status);
}
