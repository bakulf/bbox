/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_ACTION_ADD_H_
#define _BB_ACTION_ADD_H_

#include "bbaction.h"

#include <QStringList>

class BBSvn;
class QFileInfo;

class BBActionAdd : public BBAction
{
    Q_OBJECT

public:
    BBActionAdd(const QString& dirname, QObject *parent = 0);
    ~BBActionAdd();

public:
    void run();
    bool compare(const BBAction *action);

    const QString& dirname() const { return m_dirname; }

private:
    QStringList addDirectory(const QString &dirname);
    void runAdd();
    bool checkExtension(const QFileInfo &info);

private Q_SLOTS:
    void onSvnDone(bool status);

private:
    QString m_dirname;
    QStringList m_files;

    BBSvn *m_svn;
    QString m_currentFile;
};

#endif
