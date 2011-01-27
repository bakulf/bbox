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

    if (QFile::exists(m_dirname) == false) {
        emit done(false);
        return;
    }

#ifdef Q_OS_WIN32
    QString newName(QString("%1.%2").arg(BBPACKAGE).arg(info.fileName()));
#else
    QString newName(QString(".%1.%2").arg(BBPACKAGE).arg(info.fileName()));
#endif

    if (dir.exists(newName)) {
        // THIS IS SEEEEEED....

        QFileInfo info(m_dirname);
        if (removeDir(info.dir(), newName) == false) {
            BBApplication::instance()->addError(tr("Error renaming files/directories."));
            emit done(false);
            return;
        }
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

bool BBActionObstructed::removeDir(const QDir &dir, const QString& dirname)
{
    BBDEBUG << dir << dirname;

    QDir child(dir.absoluteFilePath(dirname));

    bool result(true);

    QFileInfoList files = child.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);
    foreach (QFileInfo file, files) {
        if (file.isDir())
            result = removeDir(child, file.fileName());
        else {
            // For windows we need the permission to delete this file:
            QFile f(file.absoluteFilePath());
            f.setPermissions(QFile::WriteUser | QFile::ReadUser | QFile::ReadOwner | QFile::WriteOwner);
            result =f.remove();
        }

        if (result == false)
            return result;
    }

    return dir.rmdir(dirname);
}
