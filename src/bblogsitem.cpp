/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bblogsitem.h"

BBLogsItem::BBLogsItem(const QList<QVariant> &data, BBLogsItem *parent)
{
    parentItem = parent;
    itemData = data;
}

BBLogsItem::~BBLogsItem()
{
    qDeleteAll(childItems);
}

void BBLogsItem::appendChild(BBLogsItem *item)
{
    childItems.append(item);
}

BBLogsItem *BBLogsItem::child(int row)
{
    return childItems.value(row);
}

int BBLogsItem::childCount() const
{
    return childItems.count();
}

int BBLogsItem::columnCount() const
{
    return itemData.count();
}

QVariant BBLogsItem::data(int column) const
{
    return itemData.value(column);
}

BBLogsItem *BBLogsItem::parent()
{
    return parentItem;
}

int BBLogsItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<BBLogsItem*>(this));

    return 0;
}
