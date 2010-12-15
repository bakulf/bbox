/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbsettings.h"

#include "bbsvn.h"
#include "bbdebug.h"

#include <QFile>
#include <QPointer>
#include <QCoreApplication>

#define BB_SETTINGS_AUTOCOMMIT        "autoCommit"
#define BB_SETTINGS_TIMERREMOTEACTION "timerRemoteAction"
#define BB_SETTINGS_DIRECTORY         "directory"
#define BB_SETTINGS_SVN               "svn"

BBSettings::BBSettings() :
    QObject(QCoreApplication::instance()),
    m_settings(BBPACKAGE, BBPACKAGE)
{
    BBDEBUG;

    if (m_settings.contains(BB_SETTINGS_SVN) == false) {
#ifdef Q_OS_UNIX
        if (QFile::exists("/usr/bin/svn"))
            m_settings.setValue(BB_SETTINGS_SVN, "/usr/bin/svn");
        else if (QFile::exists("/usr/local/bin/svn"))
            m_settings.setValue(BB_SETTINGS_SVN, "/usr/local/bin/svn");
        // FIXME: other OS or other PATHS
#endif
    }
}

BBSettings::~BBSettings()
{
    BBDEBUG;
}

BBSettings* BBSettings::instance()
{
    BBDEBUG;

    static QPointer<BBSettings> instance;
    if (instance.isNull())
        instance = new BBSettings();

    return instance;
}

bool BBSettings::isValid() const
{
    BBDEBUG;

    return (m_settings.contains(BB_SETTINGS_DIRECTORY) &&
            m_settings.contains(BB_SETTINGS_SVN) &&
            BBSvn::isACheckout());
}

QString BBSettings::svn() const
{
    BBDEBUG;

    return m_settings.value(BB_SETTINGS_SVN).toString();
}

void BBSettings::setSvn(const QString &aSvn)
{
    BBDEBUG << aSvn;

    if (aSvn != svn()) {
        m_settings.setValue(BB_SETTINGS_SVN, aSvn);
        emit svnChanged();
    }
}

QString BBSettings::directory() const
{
    BBDEBUG;

    return m_settings.value(BB_SETTINGS_DIRECTORY).toString();
}

void BBSettings::setDirectory(const QString &aDirectory)
{
    BBDEBUG << aDirectory;

    if (aDirectory != directory()) {
        m_settings.setValue(BB_SETTINGS_DIRECTORY, aDirectory);
        emit directoryChanged();
    }
}

uint BBSettings::timerRemoteAction() const
{
    BBDEBUG;

    uint value = m_settings.value(BB_SETTINGS_TIMERREMOTEACTION).toUInt();
    return (!value ? 5 : value);
}

void BBSettings::setTimerRemoteAction(const uint &aTimerRemoteAction)
{
    BBDEBUG << aTimerRemoteAction;

    if (aTimerRemoteAction != timerRemoteAction()) {
        m_settings.setValue(BB_SETTINGS_TIMERREMOTEACTION, aTimerRemoteAction);
        emit timerRemoteActionChanged();
    }
}

bool BBSettings::autoCommit() const
{
    BBDEBUG;

    return m_settings.value(BB_SETTINGS_AUTOCOMMIT).toBool();
}

void BBSettings::setAutoCommit(const bool &aAutoCommit)
{
    BBDEBUG << aAutoCommit;

    if (aAutoCommit != autoCommit()) {
        m_settings.setValue(BB_SETTINGS_AUTOCOMMIT, aAutoCommit);
        emit autoCommitChanged();
    }
}
