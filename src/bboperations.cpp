/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bboperations.h"

#include "bbsendreceive.h"
#include "bbsettings.h"
#include "bbdebug.h"
#include "bbconst.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QTimer>

BBOperations::BBOperations()
{
    BBDEBUG;

    resize(640, 0);
    setWindowTitle(QString(BBPACKAGE " - %1").arg("Operations"));
    setWindowIcon(QIcon(BB_ICON_IMAGE));

    QVBoxLayout *box = new QVBoxLayout();
    setLayout(box);

    QFrame *frame = new QFrame();
    frame->setFrameStyle(QFrame::Panel);
    box->addWidget(frame);

    QGridLayout *layout = new QGridLayout();
    frame->setLayout(layout);

    int row(0);
    {
        QLabel *label = new QLabel(tr("Checking local directories"));
        layout->addWidget(label, row, 0);

        m_localChangesStatus = new QLabel();
        m_localChangesStatus->setAlignment(Qt::AlignRight);
        layout->addWidget(m_localChangesStatus, row, 1);
        updateStatus(m_localChangesStatus, Waiting);
    }

    row++;
    {
        QLabel *label = new QLabel(tr("Commit"));
        layout->addWidget(label, row, 0);

        m_commitStatus = new QLabel();
        m_commitStatus->setAlignment(Qt::AlignRight);
        layout->addWidget(m_commitStatus, row, 1);
        updateStatus(m_commitStatus, Waiting);
    }

    row++;
    {
        QLabel *label = new QLabel(tr("Update"));
        layout->addWidget(label, row, 0);

        m_revisionStatus = new QLabel();
        m_revisionStatus->setAlignment(Qt::AlignRight);
        layout->addWidget(m_revisionStatus, row, 1);
        updateStatus(m_revisionStatus, Waiting);
    }

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    box->addLayout(buttonLayout);

    {
        m_closeBox = new QCheckBox(tr("Close this window when finished"));
        m_closeBox->setCheckState(BBSettings::instance()->operationClosed() == true ? Qt::Checked : Qt::Unchecked);
        buttonLayout->addWidget(m_closeBox, 0, 0);
        connect(m_closeBox,
                SIGNAL(stateChanged(int)),
                SLOT(onClosedStateChanged(int)));
    }

    buttonLayout->addWidget(new QWidget(), 1, 0);

    {
        m_closeButton = new QPushButton(tr("Close"));
        m_closeButton->setEnabled(false);
        buttonLayout->addWidget(m_closeButton, 0, 0);
        connect(m_closeButton, SIGNAL(clicked()), SLOT(accept()));
    }

    setFocus();

    start();
}

BBOperations::~BBOperations()
{
    BBDEBUG;
}

void BBOperations::updateStatus(QLabel *label, Status status)
{
    BBDEBUG << label << status;

    switch(status) {
        case Waiting:
            label->setPixmap(QPixmap::fromImage(QImage(BB_STATE_WAITING_IMAGE)));
            break;
        case Running:
            label->setPixmap(QPixmap::fromImage(QImage(BB_STATE_RUNNING_IMAGE)));
            break;
        case Error:
            label->setPixmap(QPixmap::fromImage(QImage(BB_STATE_ERROR_IMAGE)));
            break;
        case Done:
            label->setPixmap(QPixmap::fromImage(QImage(BB_STATE_COMPLETE_IMAGE)));
            break;
    }
}

void BBOperations::start()
{
    BBDEBUG;

    if (m_sendReceive.isNull())
        m_sendReceive = new BBSendReceive(this);

    connect(m_sendReceive,
            SIGNAL(localChangesDone(bool)),
            SLOT(onLocalChangesDone(bool)));
    connect(m_sendReceive,
            SIGNAL(commitDone(bool)),
            SLOT(onCommitDone(bool)));
    connect(m_sendReceive,
            SIGNAL(revisionDone(bool)),
            SLOT(onRevisionDone(bool)));
    connect(m_sendReceive,
            SIGNAL(done(bool)),
            SLOT(onDone(bool)));

    m_sendReceive->start();
}

void BBOperations::onLocalChangesDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        updateStatus(m_localChangesStatus, Error);
        return;
    }

    updateStatus(m_localChangesStatus, Done);
    updateStatus(m_commitStatus, Running);
}

void BBOperations::onCommitDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        updateStatus(m_commitStatus, Error);
        return;
    }

    updateStatus(m_commitStatus, Done);
    updateStatus(m_revisionStatus, Running);
}

void BBOperations::onRevisionDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        updateStatus(m_commitStatus, Error);
        return;
    }

    updateStatus(m_revisionStatus, Done);
}

void BBOperations::onDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        m_closeButton->setEnabled(true);
        return;
    }

    m_closeButton->setEnabled(true);

    if (BBSettings::instance()->operationClosed() == true)
        QTimer::singleShot(1500, this, SLOT(accept()));
}

void BBOperations::onClosedStateChanged(int state)
{
    BBDEBUG;

    if ((Qt::CheckState)state == Qt::Checked)
        BBSettings::instance()->setOperationClosed(true);
    else
        BBSettings::instance()->setOperationClosed(false);
}
