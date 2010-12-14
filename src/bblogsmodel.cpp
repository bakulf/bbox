/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bblogsmodel.h"

#include "bblogsitem.h"
#include "bbsvnlog.h"
#include "bbdebug.h"

BBLogsModel::BBLogsModel(const QList<BBSvnLog*>& logs, QObject *parent) :
    QAbstractItemModel(parent)
{
    BBDEBUG;

    QList<QVariant> rootData;
    rootData << tr("Revision") << tr("User") << tr("Date");
    rootItem = new BBLogsItem(rootData);
    setupModelData(logs, rootItem);
}

BBLogsModel::~BBLogsModel()
{
    BBDEBUG;
    delete rootItem;
}

int BBLogsModel::columnCount(const QModelIndex &parent) const
{
    BBDEBUG;

    if (parent.isValid())
        return static_cast<BBLogsItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant BBLogsModel::data(const QModelIndex &index, int role) const
{
    BBDEBUG;

    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    BBLogsItem *item = static_cast<BBLogsItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags BBLogsModel::flags(const QModelIndex &index) const
{
    BBDEBUG;

    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant BBLogsModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    BBDEBUG;

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex BBLogsModel::index(int row, int column, const QModelIndex &parent)
            const
{
    BBDEBUG;

    if (!hasIndex(row, column, parent))
        return QModelIndex();

    BBLogsItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<BBLogsItem*>(parent.internalPointer());

    BBLogsItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex BBLogsModel::parent(const QModelIndex &index) const
{
    BBDEBUG;

    if (!index.isValid())
        return QModelIndex();

    BBLogsItem *childItem = static_cast<BBLogsItem*>(index.internalPointer());
    BBLogsItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int BBLogsModel::rowCount(const QModelIndex &parent) const
{
    BBDEBUG;

    BBLogsItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<BBLogsItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void BBLogsModel::setupModelData(const QList<BBSvnLog*>& logs, BBLogsItem *parent)
{
    BBDEBUG << logs << parent;

    foreach(BBSvnLog* log, logs) {
        BBLogsItem *item = new BBLogsItem(QVariantList() << log->revision() << log->user() << log->date(),
                                           parent);

        QMap<QString, BBSvnLog::Change>& map = log->changes();
        QList<QString> files = map.keys();

        foreach(QString file, files) {
            QString message;
            switch (map.value(file)) {
               case BBSvnLog::Added:
                   message = tr("%1 added").arg(file);
                   break;

               case BBSvnLog::Modified:
                   message = tr("%1 modified").arg(file);
                   break;

               case BBSvnLog::Deleted:
                   message = tr("%1 deleted").arg(file);
                   break;

               case BBSvnLog::Restored:
                   message = tr("%1 restored").arg(file);
                   break;

               case BBSvnLog::Unknown:
                   message = tr("%1 (unknown operation)").arg(file);
                   break;

            }

            item->appendChild(new BBLogsItem(QVariantList() << message, item));
        }

        parent->appendChild(item);
    }
}
