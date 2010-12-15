/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bblogsitem.h"

BBLogsItem::BBLogsItem(const QList<QVariant> &data, BBLogsItem *parent) :
    m_itemData(data),
    m_parentItem(parent)
{
}

BBLogsItem::BBLogsItem(const QList<QVariant> &data, const QString& file, int revision, BBLogsItem *parent) :
    m_itemData(data),
    m_parentItem(parent),
    m_file(file),
    m_revision(revision)
{
}

BBLogsItem::~BBLogsItem()
{
    qDeleteAll(m_childItems);
}

void BBLogsItem::appendChild(BBLogsItem *item)
{
    m_childItems.append(item);
}

BBLogsItem *BBLogsItem::child(int row)
{
    return m_childItems.value(row);
}

int BBLogsItem::childCount() const
{
    return m_childItems.count();
}

int BBLogsItem::columnCount() const
{
    return m_itemData.count();
}

QVariant BBLogsItem::data(int column) const
{
    return m_itemData.value(column);
}

BBLogsItem *BBLogsItem::parent()
{
    return m_parentItem;
}

int BBLogsItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<BBLogsItem*>(this));

    return 0;
}
