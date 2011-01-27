/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_ACTION_OBSTRCUTED_H_
#define _BB_ACTION_OBSTRCUTED_H_

#include "bbaction.h"

class QDir;

class BBActionObstructed : public BBAction
{
    Q_OBJECT

public:
    BBActionObstructed(const QString &dirname, QObject *parent = 0);
    ~BBActionObstructed();

public:
    void run();
    bool compare(const BBAction *action);

    const QString& dirname() const { return m_dirname; }

private:
    bool removeDir(const QDir &dir, const QString &dirname);

private Q_SLOTS:
    void onSvnDone(bool status);

private:
    QString m_dirname;
};

#endif
