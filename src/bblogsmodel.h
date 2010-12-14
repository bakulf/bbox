/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_LOGSMODEL_H_
#define _BB_LOGSMODEL_H_

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class BBSvnLog;
class BBLogsItem;

class BBLogsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    BBLogsModel(const QList<BBSvnLog*> &logs, QObject *parent = 0);
    ~BBLogsModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
    void setupModelData(const QList<BBSvnLog*> &logs, BBLogsItem *parent);

    BBLogsItem *rootItem;
};

#endif
