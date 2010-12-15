/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bblogs.h"

#include "bblogsmodel.h"
#include "bblogsitem.h"
#include "bbsettings.h"
#include "bbsvn.h"
#include "bbsvninfo.h"
#include "bbdebug.h"
#include "bbconst.h"

#include <QDesktopServices>
#include <QModelIndexList>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeView>
#include <QLabel>
#include <QIcon>
#include <QUrl>
#include <QDir>

BBLogs::BBLogs(QObject *parent) :
    QObject(parent)
{
    BBDEBUG;

    m_dialog = new QDialog();

    m_dialog->setWindowTitle(tr(BBPACKAGE " - %1").arg(tr("Please wait...")));
    m_dialog->setWindowIcon(QIcon(BB_ICON_IMAGE));

    QHBoxLayout *layout = new QHBoxLayout();

    {
        QLabel *label = new QLabel();
        label->setAlignment(Qt::AlignCenter);
        label->setPixmap(QPixmap::fromImage(QImage(BB_ICON_IMAGE)));
        layout->addWidget(label);
    }

    {
        QLabel *label = new QLabel(tr("Loading logs..."));
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);

        QFont font;
        font.setBold(true);
        font.setPointSize(font.pointSize() * 2);
        label->setFont(font);
    }

    layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    m_dialog->setLayout(layout);
    m_dialog->show();

    m_svn = new BBSvn(this);
    connect(m_svn,
            SIGNAL(done(bool)),
            SLOT(onLocalInfoDone(bool)));
    m_svn->localInfo();
}

BBLogs::~BBLogs()
{
    BBDEBUG;
}

void BBLogs::onLocalInfoDone(bool status)
{
    BBDEBUG << status;

    if (status == false) {
        m_dialog->hide();

        QMessageBox::warning(0, QString(BBPACKAGE " - %1").arg(tr("Warning")),
                             tr("Error loading local information!"));
        deleteLater();
        return;
    }

    BBSvnInfo *info = m_svn->parseInfo();

    m_svn->disconnect(this);
    connect(m_svn,
            SIGNAL(done(bool)),
            SLOT(onRemoteLogsDone(bool)));

    m_url = info->URL();
    m_svn->remoteLog(m_url);

    info->deleteLater();
}

void BBLogs::onRemoteLogsDone(bool status)
{
    BBDEBUG << status;

    if (status == false) {
        m_dialog->hide();

        QMessageBox::warning(0, QString(BBPACKAGE " - %1").arg(tr("Warning")),
                             tr("Error loading remote information!"));
        deleteLater();
        return;
    }

    m_dialog->hide();

    createDialog(m_svn->parseLog());
}

void BBLogs::createDialog(const QList<BBSvnLog*> logs)
{
    BBDEBUG << logs;

    m_dialog = new QDialog();
    m_dialog->resize(700, 320);
    m_dialog->setWindowTitle(tr(BBPACKAGE " - %1").arg(tr("Logs")));
    m_dialog->setWindowIcon(QIcon(BB_ICON_IMAGE));

    QVBoxLayout *box = new QVBoxLayout();
    m_dialog->setLayout(box);

    {
        QHBoxLayout *layout = new QHBoxLayout();
        box->addLayout(layout);

        {
            QLabel *label = new QLabel();
            label->setAlignment(Qt::AlignCenter);
            label->setPixmap(QPixmap::fromImage(QImage(BB_ICON_IMAGE)));
            layout->addWidget(label);
        }

        {
            QLabel *label = new QLabel(tr("Logs"));
            label->setAlignment(Qt::AlignCenter);
            layout->addWidget(label);

            QFont font;
            font.setBold(true);
            font.setPointSize(font.pointSize() * 2);
            label->setFont(font);
        }

        layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    }

    m_treeView = new QTreeView();
    m_treeView->setModel(new BBLogsModel(logs, this));
    box->addWidget(m_treeView);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    box->addLayout(buttonLayout);

    buttonLayout->addWidget(new QWidget(), 1, 0);

    connect(m_treeView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            SLOT(onSelectionChanged(QItemSelection, QItemSelection)));

    {
        m_openButton = new QPushButton(tr("&Open"));
        m_openButton->setEnabled(false);
        buttonLayout->addWidget(m_openButton, 0, 0);
        connect(m_openButton,
                SIGNAL(clicked()),
                SLOT(onOpen()));
    }

    {
        QPushButton *button = new QPushButton(tr("&Close"));
        buttonLayout->addWidget(button, 0, 0);
        connect(button, SIGNAL(clicked()), SLOT(accept()));
    }

    m_dialog->show();
}

void BBLogs::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    BBDEBUG << selected << deselected;

    QModelIndexList list = selected.indexes();
    if (list.isEmpty()) {
        m_openButton->setEnabled(false);
        return;
    }

    BBLogsItem *item = static_cast<BBLogsItem*>(list[0].internalPointer());
    if (item && item->hasFile()) {
        m_openButton->setEnabled(true);
    } else {
        m_openButton->setEnabled(false);
    }
}

void BBLogs::onOpen()
{
    BBDEBUG;

    QModelIndexList list = m_treeView->selectionModel()->selectedIndexes();
    if (list.isEmpty())
        return;

    BBLogsItem *item = static_cast<BBLogsItem*>(list[0].internalPointer());
    if (!item || !item->hasFile())
        return;

    QDir dir(QDir::tempPath());
    QFileInfo info(item->file());

    m_tempFile = dir.absoluteFilePath(info.fileName());

    createTmpDialog();

    m_svn->disconnect(this);
    connect(m_svn,
            SIGNAL(done(bool)),
            SLOT(onOpenDone(bool)));

    m_svn->restoreFile(QString("%1%2").arg(m_url).arg(item->file()), item->revision(), m_tempFile);
}

void BBLogs::onOpenDone(bool status)
{
    BBDEBUG << status;

    destroyTmpDialog();

    if (status == false) {
        QMessageBox::warning(0, QString(BBPACKAGE " - %1").arg(tr("Warning")),
                             tr("Error loading remote file!"));
        return;
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(m_tempFile));
}

void BBLogs::accept()
{
    BBDEBUG;
    m_dialog->hide();
    deleteLater();
}

void BBLogs::createTmpDialog()
{
    m_tmpDialog = new QDialog();

    m_tmpDialog->setWindowTitle(tr(BBPACKAGE " - %1").arg(tr("Please wait...")));
    m_tmpDialog->setWindowIcon(QIcon(BB_ICON_IMAGE));

    QHBoxLayout *layout = new QHBoxLayout();

    {
        QLabel *label = new QLabel();
        label->setAlignment(Qt::AlignCenter);
        label->setPixmap(QPixmap::fromImage(QImage(BB_ICON_IMAGE)));
        layout->addWidget(label);
    }

    {
        QLabel *label = new QLabel(tr("Loading revisioned file..."));
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);

        QFont font;
        font.setBold(true);
        font.setPointSize(font.pointSize() * 2);
        label->setFont(font);
    }

    layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    m_tmpDialog->setLayout(layout);
    m_tmpDialog->show();
}

void BBLogs::destroyTmpDialog()
{
    m_tmpDialog->hide();
}
