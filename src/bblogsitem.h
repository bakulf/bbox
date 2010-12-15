/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_LOGSITEM_H_
#define _BB_LOGSITEM_H_

#include <QList>
#include <QVariant>

class BBLogsItem
{
public:
    BBLogsItem(const QList<QVariant> &data, BBLogsItem *parent = 0);
    BBLogsItem(const QList<QVariant> &data, const QString& file, int revision, BBLogsItem *parent = 0);
    ~BBLogsItem();

    void appendChild(BBLogsItem *child);

    BBLogsItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    BBLogsItem *parent();

    bool hasFile() { return !m_file.isEmpty(); }
    const QString& file() { return m_file; }
    int revision() { return m_revision; }

private:
    QList<BBLogsItem*> m_childItems;
    QList<QVariant> m_itemData;
    BBLogsItem *m_parentItem;

    QString m_file;
    int m_revision;
};

#endif
