/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_SETTINGS_H_
#define _BB_SETTINGS_H_

#include <QSettings>

class BBSettings : public QObject
{
    Q_OBJECT

private:
    BBSettings();

public:
    static BBSettings *instance();
    virtual ~BBSettings();

    bool isValid() const;

    QString svn() const;
    void setSvn(const QString &svn);

    QString directory() const;
    void setDirectory( const QString &directory);

    uint timerRemoteAction() const;
    void setTimerRemoteAction(const uint &timerRemoteAction);

    bool autoCommit() const;
    void setAutoCommit(const bool &autoCommit);

    bool runOnStartup() const;
    void setRunOnStartup(const bool &runOnStartup);

Q_SIGNALS:
    void svnChanged();
    void directoryChanged();
    void timerRemoteActionChanged();
    void autoCommitChanged();
    void runOnStartupChanged();

private:
    QSettings m_settings;
};

#endif
