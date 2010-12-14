/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_SVN_LOG_H_
#define _BB_SVN_LOG_H_

#include <QObject>
#include <QMap>

class BBSvnLog : public QObject
{
    Q_OBJECT

public:
    enum Change {
        Added,
        Modified,
        Deleted,
        Restored,
        Unknown
    };

public:
    BBSvnLog(uint revision,
             const QString &user,
             const QString &date,
             QObject *parent);
    ~BBSvnLog();

    void addOperation(const QByteArray& line);

    uint revision() { return m_revision; }
    const QString user() { return m_user; }
    const QString date() { return m_date; }

    QMap<QString, Change>& changes() { return m_changes; }

private:
    uint m_revision;
    QString m_user;
    QString m_date;

    QMap<QString, Change> m_changes;
};

#endif
