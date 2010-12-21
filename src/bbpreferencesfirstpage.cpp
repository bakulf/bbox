/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbpreferencesfirstpage.h"

#include "bbsettings.h"
#include "bbsvn.h"
#include "bbdebug.h"
#include "bbconst.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>

BBPreferencesFirstPage::BBPreferencesFirstPage()
{
    BBDEBUG;

    setTitle(tr("Preferences"));
    setSubTitle(tr("Use this wizard to configure %1 and make the first checkout").arg(BBPACKAGE));
    setButtonText(QWizard::NextButton, tr("&Save"));

    QGridLayout *layout = new QGridLayout();
    setLayout(layout);

    int row(0);
    {
        QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addItem(spacer, row, 0, 1, 2);
    }

    row++;
    {
        QLabel *label = new QLabel(tr("Choose the directory for your checkout. This will be keet in sync by bbox."));
        layout->addWidget(label, row, 0, 1, 2);
    }

    row++;
    {
        QLabel *label = new QLabel(tr("Directory:"));
        layout->addWidget(label, row, 0);

        QHBoxLayout *box = new QHBoxLayout();
        layout->addLayout(box, row, 1);

        m_directoryWidget = new QLineEdit();
        registerField("directoryField*", m_directoryWidget);
        box->addWidget(m_directoryWidget);

        QPushButton *button = new QPushButton(tr("Browse"));
        box->addWidget(button);
        connect(button,
                SIGNAL(clicked()),
                SLOT(onDirectorySearchClicked()));
    }

    row++;
    {
        QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addItem(spacer, row, 0, 1, 2);
    }

    row++;
    {
        QLabel *label = new QLabel(tr("Advanced configuration fields: change only if you know what you are doing."));
        layout->addWidget(label, row, 0, 1, 2);
    }

    row++;
    {
        QLabel *label = new QLabel(tr("Subversion:"));
        layout->addWidget(label, row, 0);

        QHBoxLayout *box = new QHBoxLayout();
        layout->addLayout(box, row, 1);

        m_svnWidget = new QLineEdit();
        registerField("svnField*", m_svnWidget);
        box->addWidget(m_svnWidget);

        QPushButton *button = new QPushButton(tr("Browse"));
        box->addWidget(button);
        connect(button,
                SIGNAL(clicked()),
                SLOT(onSVNSearchClicked()));
    }

    row++;
    {
        QLabel *label = new QLabel(tr("Timer for remote check:"));
        layout->addWidget(label, row, 0);

        m_timerWidget = new QComboBox();
        m_timerWidget->setEditable(false);
        layout->addWidget(m_timerWidget, row, 1);

        int index(0);
        m_timerWidget->insertItem(index++, tr("1 minute"),    1);
        m_timerWidget->insertItem(index++, tr("2 minutes"),   2);
        m_timerWidget->insertItem(index++, tr("5 minutes"),   5);
        m_timerWidget->insertItem(index++, tr("10 minutes"), 10);
        m_timerWidget->insertItem(index++, tr("20 minutes"), 20);
        m_timerWidget->insertItem(index++, tr("60 minutes"), 60);
    }

    row++;
    {
        QLabel *label = new QLabel(tr("AutoCommit on changes:"));
        layout->addWidget(label, row, 0);

        m_autocommitWidget = new QCheckBox(tr("Enabled"));
        layout->addWidget(m_autocommitWidget, row, 1);
    }

    row++;
    {
        QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addItem(spacer, row, 0, 1, 2);
    }
}

BBPreferencesFirstPage::~BBPreferencesFirstPage()
{
    BBDEBUG;
}

void BBPreferencesFirstPage::onSVNSearchClicked()
{
    BBDEBUG;

    if (m_svnDialog.isNull()) {
        m_svnDialog = new QFileDialog(this);
        m_svnDialog->setAcceptMode(QFileDialog::AcceptOpen);
        m_svnDialog->setFileMode(QFileDialog::ExistingFile);
        m_svnDialog->setWindowTitle(tr("Select the 'subversion' tool."));
        m_svnDialog->setWindowIcon(QIcon(BB_ICON_IMAGE));
        m_svnDialog->setModal(true);

        connect(m_svnDialog,
                SIGNAL(fileSelected(QString)),
                m_svnWidget,
                SLOT(setText(QString)));
    }

    m_svnDialog->show();
    m_svnDialog->setFocus();
}

void BBPreferencesFirstPage::onDirectorySearchClicked()
{
    BBDEBUG;

    if (m_directoryDialog.isNull()) {
        m_directoryDialog = new QFileDialog(this);
        m_directoryDialog->setAcceptMode(QFileDialog::AcceptOpen);
        m_directoryDialog->setFileMode(QFileDialog::Directory);
        m_directoryDialog->setWindowTitle(tr("Select the subversion directory."));
        m_directoryDialog->setWindowIcon(QIcon(BB_ICON_IMAGE));
        m_directoryDialog->setModal(true);

        connect(m_directoryDialog,
                SIGNAL(fileSelected(QString)),
                m_directoryWidget,
                SLOT(setText(QString)));
    }

    m_directoryDialog->show();
    m_directoryDialog->setFocus();
}

void BBPreferencesFirstPage::initializePage()
{
    BBDEBUG;

    m_directoryWidget->setText(BBSettings::instance()->directory());
    m_svnWidget->setText(BBSettings::instance()->svn());

    uint timer = BBSettings::instance()->timerRemoteAction();
    if (timer == 1)
        m_timerWidget->setCurrentIndex(0);
    else if (timer == 2)
        m_timerWidget->setCurrentIndex(1);
    else if (timer == 5)
        m_timerWidget->setCurrentIndex(2);
    else if (timer == 10)
        m_timerWidget->setCurrentIndex(3);
    else if (timer == 20)
        m_timerWidget->setCurrentIndex(4);
    else if (timer == 60)
        m_timerWidget->setCurrentIndex(5);

    m_autocommitWidget->setChecked(BBSettings::instance()->autoCommit());
}

bool BBPreferencesFirstPage::validatePage()
{
    BBDEBUG;

    BBSettings::instance()->setSvn(m_svnWidget->text());
    BBSettings::instance()->setDirectory(m_directoryWidget->text());
    BBSettings::instance()->setTimerRemoteAction(m_timerWidget->itemData(m_timerWidget->currentIndex()).toUInt());
    BBSettings::instance()->setAutoCommit(m_autocommitWidget->checkState() == Qt::Checked);

    if (BBSvn::isACheckout()) {
        wizard()->reject();
        return false;
    }

    return true;
}
