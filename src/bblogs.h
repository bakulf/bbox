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
class QPushButton;
class BBSvn;
class BBSvnLog;
class QTreeView;
class QItemSelection;
class QFile;

class BBLogs : public QObject
{
    Q_OBJECT

public:
    BBLogs(QObject *parent = 0);
    virtual ~BBLogs();

private Q_SLOTS:
    void onLocalInfoDone(bool status);
    void onRemoteLogsDone(bool status);
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

    void accept();

    void onOpen();
    void onOpenDone(bool status);

private:
    void createDialog(const QList<BBSvnLog*> logs);

    void createTmpDialog();
    void destroyTmpDialog();

private:
    QString m_url;

    QPointer<QDialog> m_dialog;
    QPointer<QDialog> m_tmpDialog;
    QPointer<BBSvn> m_svn;

    QPointer<QPushButton> m_openButton;

    QPointer<QTreeView> m_treeView;

    QString m_tempFile;
};

#endif
