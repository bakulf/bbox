/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_LOGS_H_
#define _BB_LOGS_H_

#include <QObject>
#include <QPointer>

class QDialog;
class BBSvn;
class BBSvnLog;

class BBLogs : public QObject
{
    Q_OBJECT

public:
    BBLogs(QObject *parent = 0);
    virtual ~BBLogs();

private Q_SLOTS:
    void onLocalInfoDone(bool status);
    void onRemoteLogsDone(bool status);

    void accept();

private:
    void createDialog(const QList<BBSvnLog*> logs);

private:
    QPointer<QDialog> m_dialog;
    QPointer<BBSvn> m_svn;
};

#endif
