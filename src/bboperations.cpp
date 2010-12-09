#include "bboperations.h"

#include "bbactionlocalchanges.h"
#include "bbactionupdate.h"
#include "bbactioncommit.h"
#include "bbdebug.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

BBOperations::BBOperations()
{
    BBDEBUG;

    setWindowTitle(QString(BBPACKAGE " - %1").arg("Operations"));
    resize(640, 0);

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
    }

    row++;
    {
        QLabel *label = new QLabel(tr("Update directory"));
        layout->addWidget(label, row, 0);

        m_updateStatus = new QLabel();
        m_updateStatus->setAlignment(Qt::AlignRight);
        layout->addWidget(m_updateStatus, row, 1);
    }

    row++;
    {
        QLabel *label = new QLabel(tr("Commit"));
        layout->addWidget(label, row, 0);

        m_commitStatus = new QLabel();
        m_commitStatus->setAlignment(Qt::AlignRight);
        layout->addWidget(m_commitStatus, row, 1);
    }

    row++;
    {
        QLabel *label = new QLabel(tr("Revision update"));
        layout->addWidget(label, row, 0);

        m_revisionStatus = new QLabel();
        m_revisionStatus->setAlignment(Qt::AlignRight);
        layout->addWidget(m_revisionStatus, row, 1);
    }

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    box->addLayout(buttonLayout);

    buttonLayout->addWidget(new QWidget(), 1, 0);

    {
        m_closeButton = new QPushButton(tr("Close"));
        m_closeButton->setEnabled(false);
        buttonLayout->addWidget(m_closeButton, 0, 0);
        connect(m_closeButton, SIGNAL(clicked()), SLOT(accept()));
    }

    setFocus();

    actionLocalChanges();
}

BBOperations::~BBOperations()
{
    BBDEBUG;
}

void BBOperations::updateStatus(QLabel *label, Status status)
{
    BBDEBUG << label << status;

    switch(status) {
        case Empty:
            label->setText(QString());
            break;
        case Running:
            label->setText(tr("In progress"));
            break;
        case Error:
            label->setText(tr("Error"));
            break;
        case Done:
            label->setText(tr("Done"));
            break;
    }
}

void BBOperations::actionLocalChanges()
{
    BBDEBUG;

    updateStatus(m_localChangesStatus, Running);
    m_action = new BBActionLocalChanges(false, this);
    connect(m_action,
            SIGNAL(done(bool)),
            SLOT(onActionLocalChangesDone(bool)));
    m_action->run();
}

void BBOperations::onActionLocalChangesDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        updateStatus(m_localChangesStatus, Error);
        m_closeButton->setEnabled(true);
        m_action->deleteLater();
        return;
    }

    updateStatus(m_localChangesStatus, Done);
    m_action->deleteLater();

    updateStatus(m_updateStatus, Running);
    m_action = new BBActionUpdate(this);
    connect(m_action,
            SIGNAL(done(bool)),
            SLOT(onActionUpdateDone(bool)));
    m_action->run();
}

void BBOperations::onActionUpdateDone(bool status)
{
    if (status == false) {
        updateStatus(m_updateStatus, Error);
        m_closeButton->setEnabled(true);
        m_action->deleteLater();
        return;
    }

    updateStatus(m_updateStatus, Done);
    m_action->deleteLater();

    updateStatus(m_commitStatus, Running);
    m_action = new BBActionCommit(this);
    connect(m_action,
            SIGNAL(done(bool)),
            SLOT(onActionCommitDone(bool)));
    m_action->run();
}

void BBOperations::onActionCommitDone(bool status)
{
    if (status == false) {
        updateStatus(m_commitStatus, Error);
        m_closeButton->setEnabled(true);
        return;
    }

    updateStatus(m_commitStatus, Done);
    m_action->deleteLater();

    updateStatus(m_revisionStatus, Running);
    m_action = new BBActionUpdate(this);
    connect(m_action,
            SIGNAL(done(bool)),
            SLOT(onActionRevisionDone(bool)));
    m_action->run();
}

void BBOperations::onActionRevisionDone(bool status)
{
    if (status == false) {
        updateStatus(m_commitStatus, Error);
        m_closeButton->setEnabled(true);
        return;
    }

    updateStatus(m_revisionStatus, Done);
    m_action->deleteLater();

    m_closeButton->setEnabled(true);
}
