/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_CONFLICT_H_
#define _BB_CONFLICT_H_

#include <QDialog>
#include <QPointer>

class BBSvn;
class BBSvnStatus;
class BBConflictFile;
class QLabel;

class BBConflict : public QDialog
{
    Q_OBJECT

    enum Status {
        Waiting,
        Running,
        Error,
        Done
    };

public:
    BBConflict(const QList<BBSvnStatus*> list);
    virtual ~BBConflict();

public:
    static void check(QList<BBSvnStatus*> list);

private Q_SLOTS:
    void apply();
    void applyMerge(bool status);

private:
    void updateStatus(QLabel *label, Status status);

private:
    QPointer<BBConflictFile> m_conflictFile;
    QList<BBConflictFile*> m_list;
    BBSvn *m_svn;

public:
    static QPointer<BBConflict> m_dialog;
};

#endif
