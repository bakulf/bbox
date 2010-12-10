/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_ACTION_DELETE_H_
#define _BB_ACTION_DELETE_H_

#include "bbaction.h"

class BBSvn;

class BBActionDelete : public BBAction
{
    Q_OBJECT

public:
    BBActionDelete(const QString& filename, QObject *parent = 0);
    ~BBActionDelete();

public:
    void run();
    bool compare(const BBAction *action);

    const QString& filename() const { return m_filename; }

private Q_SLOTS:
    void onSvnDone(bool status);

private:
    QString m_filename;
    BBSvn *m_svn;
};

#endif
