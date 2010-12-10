#include "bbsettings.h"

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
            m_settings.contains(BB_SETTINGS_SVN));
}

QString BBSettings::svn() const
{
    BBDEBUG;

    return m_settings.value(BB_SETTINGS_SVN).toString();
}

void BBSettings::setSvn(const QString &svn)
{
    BBDEBUG << svn;

    m_settings.setValue(BB_SETTINGS_SVN, svn);
    emit svnChanged();
}

QString BBSettings::directory() const
{
    BBDEBUG;

    return m_settings.value(BB_SETTINGS_DIRECTORY).toString();
}

void BBSettings::setDirectory(const QString &directory)
{
    BBDEBUG << directory;

    m_settings.setValue(BB_SETTINGS_DIRECTORY, directory);
    emit directoryChanged();
}

uint BBSettings::timerRemoteAction() const
{
    BBDEBUG;

    uint value = m_settings.value(BB_SETTINGS_TIMERREMOTEACTION).toUInt();
    return (!value ? 5 : value);
}

void BBSettings::setTimerRemoteAction(const uint &timerRemoteAction)
{
    BBDEBUG << timerRemoteAction;

    m_settings.setValue(BB_SETTINGS_TIMERREMOTEACTION, timerRemoteAction);
    emit timerRemoteActionChanged();
}

bool BBSettings::autoCommit() const
{
    BBDEBUG;

    return m_settings.value(BB_SETTINGS_AUTOCOMMIT).toBool();
}

void BBSettings::setAutoCommit(const bool &autoCommit)
{
    BBDEBUG << autoCommit;

    m_settings.setValue(BB_SETTINGS_AUTOCOMMIT, autoCommit);
    emit autoCommitChanged();
}
