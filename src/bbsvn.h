/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_SVN_H_
#define _BB_SVN_H_

#include <QProcess>

class BBSvnLog;
class BBSvnInfo;
class BBSvnStatus;

class BBSvn : public QProcess
{
    Q_OBJECT

public:
    BBSvn(QObject *parent = 0);
    ~BBSvn();

public:
    static bool isACheckout();
    static void openFile(const QString& file, bool local);

public:
    void cleanup();
    void addFile(const QString &filename);
    void deleteFile(const QString &filename);

    void status();
    QList<BBSvnStatus*> parseStatus();
    QList<BBSvnStatus*> parseUpdate();

    void remoteInfo(const QString& url);
    void localInfo();
    BBSvnInfo* parseInfo();

    void commit();
    void update();

    void resolveConflict(const QString& file, bool isLocal);

    void checkout(const QString& url,
                  const QString& username,
                  const QString& password);

    void remoteLog(const QString& url);
    QList<BBSvnLog*> parseLog();

    void restoreFile(const QString& file, int revision, const QString& destFile);

    QString errorMessage() { return m_errorMessage; }

Q_SIGNALS:
    void done(bool ok);

private:
    static void openFile(const QString& file, const QString& name);

private:
    QString commitMessage();
    QString userName();

    void start(const QStringList &arguments);

private Q_SLOTS:
    void onFinished(int exitCode, QProcess::ExitStatus extiStatus);

private:
    QString m_errorMessage;
};

#endif
