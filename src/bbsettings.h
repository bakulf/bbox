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
    void setTimerRemoteAction( const uint &timerRemoteAction);

Q_SIGNALS:
    void svnChanged();
    void directoryChanged();
    void timerRemoteActionChanged();

private:
    QSettings m_settings;
};

#endif
