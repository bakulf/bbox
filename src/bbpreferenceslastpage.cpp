/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbpreferenceslastpage.h"

#include "bbsettings.h"
#include "bbsvn.h"
#include "bbdebug.h"
#include "bbconst.h"

#include <QMovie>
#include <QLabel>
#include <QListWidget>
#include <QSpacerItem>
#include <QVBoxLayout>

BBPreferencesLastPage::BBPreferencesLastPage() :
    m_complete(false),
    m_counter(0)
{
    BBDEBUG;

    setTitle(tr("Checking out...."));
    setSubTitle(tr("Please, wait. The remote content is moved to your directory."));
    setFinalPage(true);
    setCommitPage(true);

    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

    {
        QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addItem(spacer);
    }

    {
        QHBoxLayout *box = new QHBoxLayout();
        layout->addLayout(box);

        m_counterLabel = new QLabel();
        box->setStretch(1,1);
        box->addWidget(m_counterLabel);

        m_loaderLabel = new QLabel();
        m_loaderLabel->setAlignment(Qt::AlignRight);
        box->addWidget(m_loaderLabel);
        QMovie *movie = new QMovie(":images/loader.gif");
        m_loaderLabel->setMovie(movie);
        movie->start();

        m_listWidget = new QListWidget();
        m_listWidget->setSelectionMode(QAbstractItemView::NoSelection);
        layout->addWidget(m_listWidget);
    }

    {
        QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addItem(spacer);
    }

    {
        m_errorLabel = new QLabel();
        m_errorLabel->setWordWrap(true);
        layout->addWidget(m_errorLabel);
    }

    {
        QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addItem(spacer);
    }
}

BBPreferencesLastPage::~BBPreferencesLastPage()
{
    BBDEBUG;
}

void BBPreferencesLastPage::initializePage()
{
    BBDEBUG;

    wizard()->setOption(QWizard::NoBackButtonOnLastPage, true);

    m_counterLabel->setText(tr("Waiting..."));
    m_errorLabel->setText(QString());
    m_listWidget->clear();
    m_counter = 0;

    if (m_svn.isNull()) {
        m_svn = new BBSvn(this);
        connect(m_svn,
                SIGNAL(done(bool)),
                SLOT(onSvnDone(bool)));
        connect(m_svn,
                SIGNAL(readyReadStandardOutput()),
                SLOT(onSvnRead()));
    }

    m_svn->checkout(field("urlField").toString(),
                    field("usernameField").toString(),
                    field("passwordField").toString());
}

void BBPreferencesLastPage::onSvnDone(bool status)
{
    BBDEBUG;

    if (status == false) {
        m_errorLabel->setText(tr("Checkout error: <b>%1</b>").arg(m_svn->errorMessage()).replace("svn:", "").trimmed());
        wizard()->setOption(QWizard::NoBackButtonOnLastPage, false);
        return;
    }

    m_counterLabel->setText(tr("Downloaded files: %1").arg(m_counter));
    m_errorLabel->setText(tr("Checkout completed!"));
    m_loaderLabel->setVisible(false);

    m_complete = true;
    emit completeChanged();
}

void BBPreferencesLastPage::onSvnRead()
{
    QString path = BBSettings::instance()->directory();

    while (1) {
        QByteArray array = m_svn->readLine();
        if (array.isEmpty())
            break;

        if (!array.startsWith("A "))
            continue;

        m_listWidget->addItem(QString(array.remove(0, 1)).remove(path).trimmed());

        ++m_counter;
        m_counterLabel->setText(tr("Downloading files: %1").arg(m_counter));
    }
}

bool BBPreferencesLastPage::isComplete() const
{
    BBDEBUG << m_complete;
    return m_complete;
}
