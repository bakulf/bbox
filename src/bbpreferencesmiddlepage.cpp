/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbpreferencesmiddlepage.h"

#include "bbsettings.h"
#include "bbdebug.h"
#include "bbconst.h"

#include <QSpacerItem>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>

BBPreferencesMiddlePage::BBPreferencesMiddlePage()
{
    BBDEBUG;

    setTitle(tr("Checkout helper"));
    setSubTitle(
        tr(
           "The directory '<b>%1</b>' is not a subversion directory.<br />"
           "This wizard will help you to make the first checkout."
          ).arg(BBSettings::instance()->directory()));

    QGridLayout *layout = new QGridLayout();
    setLayout(layout);

    int row(0);
    {
        QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addItem(spacer, row, 0, 1, 2);
    }

    ++row;
    {
        QLabel *label = new QLabel(tr("URL:"));
        layout->addWidget(label, row, 0);

        QLineEdit *urlWidget = new QLineEdit();
        layout->addWidget(urlWidget, row, 1);
        registerField("urlField*", urlWidget);
    }

    ++row;
    {
        QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addItem(spacer, row, 0, 1, 2);
    }

    ++row;
    {
        QLabel *label = new QLabel(tr("The next fields are optional but probably you must add them too."));
        layout->addWidget(label, row, 0, 1, 2);
    }

    ++row;
    {
        QLabel *label = new QLabel(tr("Username:"));
        layout->addWidget(label, row, 0);

        QLineEdit *usernameWidget = new QLineEdit();
        layout->addWidget(usernameWidget, row, 1);
        registerField("usernameField", usernameWidget);
    }

    ++row;
    {
        QLabel *label = new QLabel(tr("Password:"));
        layout->addWidget(label, row, 0);

        QLineEdit *passwordWidget = new QLineEdit();
        passwordWidget->setEchoMode(QLineEdit::Password);
        layout->addWidget(passwordWidget, row, 1);
        registerField("passwordField", passwordWidget);
    }

    ++row;
    {
        QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addItem(spacer, row, 0, 1, 2);
    }
}

BBPreferencesMiddlePage::~BBPreferencesMiddlePage()
{
    BBDEBUG;
}
