/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bblogs.h"

#include "bblogsmodel.h"
#include "bbsvn.h"
#include "bbsvninfo.h"
#include "bbdebug.h"
#include "bbconst.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeView>
#include <QLabel>
#include <QIcon>

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

    m_svn->remoteLog(info->URL());
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

    QTreeView *tree = new QTreeView();
    tree->setModel(new BBLogsModel(logs, this));
    box->addWidget(tree);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    box->addLayout(buttonLayout);

    buttonLayout->addWidget(new QWidget(), 1, 0);

    {
        QPushButton *button = new QPushButton(tr("&Close"));
        buttonLayout->addWidget(button, 0, 0);
        connect(button, SIGNAL(clicked()), SLOT(accept()));
    }

    m_dialog->show();
}

void BBLogs::accept()
{
    BBDEBUG;
    m_dialog->hide();
    deleteLater();
}
