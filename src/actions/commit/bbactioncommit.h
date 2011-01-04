/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_ACTION_COMMIT_H_
#define _BB_ACTION_COMMIT_H_

#include "bbaction.h"

class BBActionCommit : public BBAction
{
    Q_OBJECT

public:
    BBActionCommit(bool withError, QObject *parent = 0);
    ~BBActionCommit();

public:
    void run();
    bool compare(const BBAction *action);

private:
    bool checkObstructedFiles(const QString& dirname, bool *founded);

private Q_SLOTS:
    void onSvnDone(bool status);
    void onLocalChangesDone(bool status);

private:
    bool m_withError;
};

#endif
