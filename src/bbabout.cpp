/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbabout.h"

#include "bbdebug.h"
#include "bbconst.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

BBAbout::BBAbout() :
    QDialog(0, Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint|Qt::WindowSystemMenuHint)
{
    BBDEBUG;

    setWindowTitle(QString(BBPACKAGE " - %1").arg(tr("About")));
    setWindowIcon(QIcon(BB_ICON_IMAGE));

    QGridLayout *layout = new QGridLayout();
    setLayout(layout);

    int row(0);
    {
        QLabel *label = new QLabel();
        label->setAlignment(Qt::AlignCenter);
        label->setPixmap(QPixmap::fromImage(QImage(BB_LOGO_IMAGE)));
        layout->addWidget(label, row, 0, 1, 3);
    }

    ++row;
    {
        QLabel *label = new QLabel(QString(BBPACKAGE " " BBVERSION));
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label, row, 0, 1, 3);

        QFont font;
        font.setBold(true);
        label->setFont(font);
    }

    ++row;
    {
        QLabel *label = new QLabel(tr("copyright(c) 2010 Andrea Marchesini"));
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label, row, 0, 1, 3);
    }

    ++row;
    {
        QPushButton *button = new QPushButton(tr("Close"));
        connect(button, SIGNAL(clicked()), SLOT(accept()));

        layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding), row, 0, 1, 1);
        layout->addWidget(button, row, 1);
        layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding), row, 2, 1, 1);
    }

    setFocus();
}

BBAbout::~BBAbout()
{
    BBDEBUG;
}
