/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbactiondelete.h"

#include "bbsvn.h"
#include "bbapplication.h"
#include "bbdebug.h"

BBActionDelete::BBActionDelete(const QString& filename, QObject *parent) :
    BBAction(parent),
    m_filename(filename)
{
    BBDEBUG;
}

BBActionDelete::~BBActionDelete()
{
    BBDEBUG;
}

bool BBActionDelete::compare(const BBAction *action)
{
    BBDEBUG;

    const BBActionDelete *a = qobject_cast<const BBActionDelete *>(action);
    if (!a)
        return false;

    bool ret(a->filename() == m_filename);
    BBDEBUG << "Compare: " << ret;
    return ret;
}

void BBActionDelete::run()
{
    BBDEBUG;

    m_svn = new BBSvn(this);
    connect(m_svn,
            SIGNAL(done(bool)),
            SLOT(onSvnDone(bool)));

    m_svn->deleteFile(m_filename);
}

void BBActionDelete::onSvnDone(bool status)
{
    BBDEBUG << status;

    if (status == false) {
        BBApplication::instance()->addError(tr("Error deleting file '%1'.").arg(m_filename));
    }

    m_svn->deleteLater();
    emit done(status);
}
