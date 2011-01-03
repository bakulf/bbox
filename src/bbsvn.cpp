/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbsvn.h"

#include "bbsvnstatus.h"
#include "bbsvninfo.h"
#include "bbsvnlog.h"
#include "bbsettings.h"
#include "bbdebug.h"

#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

#ifdef Q_OS_WIN32
    #include <windows.h>
    #include <winbase.h>
    #include <Lmcons.h>
#endif

BBSvn::BBSvn(QObject *parent) :
    QProcess(parent)
{
    BBDEBUG;
    connect(this,
            SIGNAL(finished(int, QProcess::ExitStatus)),
            SLOT(onFinished(int, QProcess::ExitStatus)));
}

BBSvn::~BBSvn()
{
    BBDEBUG;
}

void BBSvn::start(const QStringList &arguments)
{
    BBDEBUG << arguments;
    QProcess::start(BBSettings::instance()->svn(), arguments, QIODevice::ReadOnly);
}

void BBSvn::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    m_errorMessage = readAllStandardError().trimmed();
    BBDEBUG << exitCode << exitStatus << m_errorMessage;

    bool status(true);
    if (exitCode || exitStatus != QProcess::NormalExit)
        status = false;

    emit done(status);
}

void BBSvn::cleanup()
{
    BBDEBUG;
    start(QStringList() << "cleanup" << "--non-interactive" << BBSettings::instance()->directory());
}

void BBSvn::addFile(const QStringList &filenames)
{
    BBDEBUG << filenames;
    start(QStringList() << "add" << filenames);
}

void BBSvn::deleteFile(const QString &filename)
{
    BBDEBUG << filename;
    start(QStringList() << "delete" << "--keep-local" << filename);
}

void BBSvn::status()
{
    BBDEBUG;
    start(QStringList() << "status" << BBSettings::instance()->directory());
}

QList<BBSvnStatus*> BBSvn::parseStatus()
{
    BBDEBUG;

    QByteArray output = readAllStandardOutput();
    QList<QByteArray> files = output.split('\n');

    QList<BBSvnStatus*> list;
    foreach(QByteArray file, files) {
        BBSvnStatus *status = new BBSvnStatus(this);

        switch (file[0]) {
            case 'A': // Added
                status->setStatus(BBSvnStatus::StatusAdded);
                break;

            case 'C': // Conflicted
                status->setStatus(BBSvnStatus::StatusConflicted);
                break;

            case 'D': // Deleted
                status->setStatus(BBSvnStatus::StatusDeleted);
                break;

            case 'I': // Ignored
                break;

            case 'M': // Modified
                status->setStatus(BBSvnStatus::StatusModified);
                break;

            case 'R': // Replaced
                status->setStatus(BBSvnStatus::StatusReplaced);
                break;

            case 'X': // an unversioned directory created by an externals definition
                break;

            case '?': // item is not under version control
                break;

            case '!': // item is missing (removed by non-svn command) or incomplete
                status->setStatus(BBSvnStatus::StatusMissing);
                break;

            case '~': // versioned item obstructed by some item of a different kind
                break;
        }

        // Second column is property related

        // Third column: lock
        if (file[2] == 'L')
            status->setLocked(true);

        status->setFile(QString(file.remove(0, 8)).trimmed());

        if (!status->isValid()) {
            status->deleteLater();
        } else {
            list << status;
        }
    }

    return list;
}

void BBSvn::remoteInfo(const QString& url)
{
    BBDEBUG;
    start(QStringList() << "info" << "--non-interactive" << url);
}

void BBSvn::localInfo()
{
    BBDEBUG;
    start(QStringList() << "info" << "--non-interactive" << BBSettings::instance()->directory());
}

BBSvnInfo* BBSvn::parseInfo()
{
    BBDEBUG;

    QByteArray output = readAllStandardOutput();
    QList<QByteArray> lines = output.split('\n');

    BBSvnInfo *info = new BBSvnInfo(this);

    foreach (QByteArray line, lines) {
        if (line.startsWith("URL: "))
            info->setURL(QString(line.remove(0, 5)).trimmed());
        else if(line.startsWith("Revision: "))
            info->setRevision(QString(line.remove(0, 10)).toUInt());
    }

    return info;
}

void BBSvn::commit()
{
    BBDEBUG;
    start(QStringList() << "commit" << "--non-interactive"
                        << "--trust-server-cert" << "-m" << commitMessage()
                        << BBSettings::instance()->directory());
}

QString BBSvn::userName() {
#ifdef Q_OS_WIN32
    wchar_t winUserName[UNLEN + 1]; // UNLEN is defined in LMCONS.H
    DWORD winUserNameSize = sizeof(winUserName);
    GetUserNameW( winUserName, &winUserNameSize );
    return QString::fromWCharArray( winUserName );
#endif

#ifdef Q_OS_UNIX
    return getenv("USER");
#endif

    return QString();
}

QString BBSvn::commitMessage()
{
    BBDEBUG;

    return tr("Update by %1 (user: %2, date: %3)").arg(BBPACKAGE)
                                                  .arg(userName())
                                                  .arg(QDateTime::currentDateTime().toString());
}

void BBSvn::update()
{
    BBDEBUG;
    start(QStringList() << "update" << "--non-interactive" << "--accept" << "postpone" << BBSettings::instance()->directory());
}

QList<BBSvnStatus*> BBSvn::parseUpdate()
{
    BBDEBUG;

    QByteArray output = readAllStandardOutput();
    QList<QByteArray> files = output.split('\n');

    QList<BBSvnStatus*> list;
    foreach(QByteArray file, files) {
        BBSvnStatus *status = new BBSvnStatus(this);

        switch (file[0]) {
            case 'A': // Added
                status->setStatus(BBSvnStatus::StatusAdded);
                break;

            case 'D': // Deleted
                status->setStatus(BBSvnStatus::StatusDeleted);
                break;

            case 'U': // Updated
                status->setStatus(BBSvnStatus::StatusUpdated);
                break;

            case 'C': // Conflicted
                status->setStatus(BBSvnStatus::StatusConflicted);
                break;

            case 'G': // Merged
                status->setStatus(BBSvnStatus::StatusMerged);
                break;

            case 'E': // Existed
                status->setStatus(BBSvnStatus::StatusExisted);
                break;
        }

        status->setFile(QString(file.remove(0, 5)).trimmed());

        if (!status->isValid()) {
            status->deleteLater();
        } else {
            list << status;
        }
    }

    return list;
}

void BBSvn::resolveConflict(const QString& file, bool isLocal)
{
    BBDEBUG << file << isLocal;
    start(QStringList() << "resolve" << "--non-interactive" << "--accept" << (isLocal ? "mine-full" : "theirs-full") << file);
}

bool BBSvn::isACheckout()
{
    QDir dir(BBSettings::instance()->directory());
    QFileInfo info(dir.absoluteFilePath(".svn"));

    return info.exists();
}

void BBSvn::checkout(const QString& url, const QString& username, const QString& password)
{
    BBDEBUG << url << username << password;

    QStringList list;
    list << "checkout" << "--non-interactive";

    if (!username.isEmpty())
       list << "--username" << username;
    if (!password.isEmpty())
       list << "--password" << password;

    list << url << BBSettings::instance()->directory();

    start(list);
}

void BBSvn::remoteLog(const QString& url)
{
    BBDEBUG << url;
    start(QStringList() << "log" << "--non-interactive" << "-v" << url);
}

QList<BBSvnLog*> BBSvn::parseLog()
{
    BBDEBUG;

    QByteArray output = readAllStandardOutput();
    QList<QByteArray> lines = output.split('\n');

    QList<BBSvnLog*> list;

    while(!lines.isEmpty()) {
        QByteArray line;

        line = lines.takeFirst();
        if (!line.startsWith("----------------"))
            continue;

        line = lines.takeFirst();
        if (!line.startsWith("r"))
            break;

        QList<QByteArray> parts = line.split('|');
        if (parts.size() < 3)
            break;

        BBSvnLog *log = new BBSvnLog(parts[0].remove(0,1).trimmed().toUInt(),
                                     parts[1].trimmed(),
                                     parts[2].trimmed(),
                                     this);

        line = lines.takeFirst();
        if (!line.startsWith("Changed paths:"))
            break;

        while (1) {
            line = lines.takeFirst();
            if (line.isEmpty() || line.trimmed().isEmpty())
                break;

            log->addOperation(line);
        }

        list << log;
    }

    return list;
}

void BBSvn::restoreFile(const QString& file, int revision, const QString& destFile)
{
    BBDEBUG << file << revision << destFile;
    start(QStringList() << "export" << "--non-interactive"
                        << QString("%1@%2").arg(file).arg(revision)
                        << destFile);
}

void BBSvn::openFile(const QString& file, bool local)
{
    BBDEBUG << file << local;

    QFileInfo info(file);
    QString filename(info.fileName());

    if (local) {
        if (QFile::exists(QString("%1.mine").arg(file)))
            openFile(QString("%1.mine").arg(file), filename);
        else {
            QMessageBox::warning(0,
                                 QString(BBPACKAGE " - %1").arg(tr("Warning")),
                                 tr("The local version seems missing."));
        }
    } else {
        QDir dir(info.dir());
        uint maxValue(0);

        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        foreach (QFileInfo info, list) {
            if (!info.fileName().startsWith(filename))
                continue;

            QString ext = info.completeSuffix();
            if (!ext.startsWith("r"))
                continue;

            ext.remove(0, 1);
            uint value(ext.toUInt());
            if (maxValue < value)
                maxValue = value;
        }

        if (maxValue == 0) {
            QMessageBox::warning(0,
                                 QString(BBPACKAGE " - %1").arg(tr("Warning")),
                                 tr("The remove version seems missing."));
        } else {
            openFile(QString("%1.r%2").arg(file).arg(maxValue), filename);
        }
    }
}

void BBSvn::openFile(const QString& file, const QString& name)
{
    BBDEBUG << file << name;

    QString newFile(QDir::temp().absoluteFilePath(name));

    if (QFile::exists(newFile))
        QFile::remove(newFile);

    QFile::copy(file, newFile);

    QDesktopServices::openUrl(QUrl::fromLocalFile(newFile));
}
