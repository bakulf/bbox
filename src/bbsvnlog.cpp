/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbsvnlog.h"

#include "bbdebug.h"

BBSvnLog::BBSvnLog(uint revision, const QString& user, const QString& date, QObject *parent) :
    QObject(parent),
    m_revision(revision),
    m_user(user),
    m_date(date)
{
    BBDEBUG;
}

BBSvnLog::~BBSvnLog()
{
    BBDEBUG;
}

void BBSvnLog::addOperation(const QByteArray& line)
{
    BBDEBUG << line;
    QList<QByteArray> parts = line.trimmed().split(' ');

    if (parts[0][0] == 'A')
        m_changes.insert(QString(parts.last()), Added);
    else if (parts[0][0] == 'M')
        m_changes.insert(QString(parts.last()), Modified);
    else if (parts[0][0] == 'D')
        m_changes.insert(QString(parts.last()), Deleted);
    else if (parts[0][0] == 'R')
        m_changes.insert(QString(parts.last()), Restored);
    else
        m_changes.insert(QString(parts.last()), Unknown);
}
