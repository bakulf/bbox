/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_SVN_STATUS_H_
#define _BB_SVN_STATUS_H_

#include <QObject>

class BBSvnStatus : public QObject
{
    Q_OBJECT

public:
    enum Status {
        StatusUnknown,
        StatusAdded,
        StatusConflicted,
        StatusDeleted,
        StatusModified,
        StatusReplaced,
        StatusMissing,
        StatusUpdated,
        StatusMerged,
        StatusExisted,
    };

public:
    BBSvnStatus(QObject *parent = 0);
    ~BBSvnStatus();

public:
    void setStatus(BBSvnStatus::Status status);
    Status status() const;

    void setLocked(bool locked);
    bool locked() const;

    void setFile(const QString& filename);
    const QString& file() const;

    bool isValid();

private:
    Status m_status;
    bool m_locked;
    QString m_filename;
};

#endif
