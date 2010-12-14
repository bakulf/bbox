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
    ~BBLogsItem();

    void appendChild(BBLogsItem *child);

    BBLogsItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    BBLogsItem *parent();

private:
    QList<BBLogsItem*> childItems;
    QList<QVariant> itemData;
    BBLogsItem *parentItem;
};

#endif
