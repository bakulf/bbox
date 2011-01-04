/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbactionobstructed.h"

#include "bbapplication.h"
#include "bbsvn.h"
#include "bbdebug.h"

#include <QFileInfo>
#include <QDir>

BBActionObstructed::BBActionObstructed(const QString& dirname, QObject *parent) :
    BBAction(parent),
    m_dirname(dirname)
{
    BBDEBUG;
}

BBActionObstructed::~BBActionObstructed()
{
    BBDEBUG;
}

bool BBActionObstructed::compare(const BBAction *action)
{
    BBDEBUG;

    const BBActionObstructed *a = qobject_cast<const BBActionObstructed *>(action);
    if (!a)
        return false;

    bool ret(a->dirname() == m_dirname);
    BBDEBUG << "Compare: " << ret;
    return ret;
}

void BBActionObstructed::run()
{
    BBDEBUG;

    QFileInfo info(m_dirname);
    QDir dir(info.dir());

    QString newName(QString(".%1.%2").arg(BBPACKAGE).arg(info.fileName()));
    if (dir.exists(newName)) {
        BBApplication::instance()->addError(tr("The filesystem is not clean"));
        emit done(false);
        return;
    }

    if (dir.rename(info.fileName(), newName) == false) {
        BBApplication::instance()->addError(tr("Error renaming files/directories."));
        emit done(false);
        return;
    }

    BBSvn *svn = new BBSvn(this);
    connect(svn,
            SIGNAL(done(bool)),
            SLOT(onSvnDone(bool)));
    connect(svn,
            SIGNAL(done(bool)),
            svn,
            SLOT(deleteLater()));

    svn->deleteFile(m_dirname);
}

void BBActionObstructed::onSvnDone(bool status)
{
    if (status == false) {
        BBApplication::instance()->addError(tr("Error renaming files."));
    }

    emit done(status);
}
