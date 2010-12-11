/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbcheckout.h"

#include "bbcheckoutfirstpage.h"
#include "bbcheckoutlastpage.h"
#include "bbapplication.h"
#include "bbdebug.h"
#include "bbconst.h"

#include <QCloseEvent>
#include <QMessageBox>

BBCheckout::BBCheckout()
{
    BBDEBUG;

    resize(640, 480);
    setWindowTitle(QString(BBPACKAGE " - %1").arg("Checkout Wizard"));

    addPage(new BBCheckoutFirstPage());
    addPage(new BBCheckoutLastPage());

    setOption(QWizard::NoCancelButton, true);

    setPixmap(QWizard::LogoPixmap,      QPixmap::fromImage(QImage(BB_ICON_IMAGE)));
    setPixmap(QWizard::WatermarkPixmap, QPixmap::fromImage(QImage(BB_LOGO_IMAGE)));

    setModal(true);
    setFocus();

    connect(this,
            SIGNAL(rejected()),
            SLOT(onRejected()));
}

BBCheckout::~BBCheckout()
{
    BBDEBUG;
}

void BBCheckout::closeEvent(QCloseEvent *event)
{
    BBDEBUG;

    if (currentId() != pageIds().last() ||
        !currentPage()->isComplete()) {
        event->ignore();

        if (QMessageBox::warning(this,
                                 QString(BBPACKAGE " - %1").arg(tr("Warning")),
                                 tr("Do you want to quit?"),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
            BBApplication::instance()->quit();
        }
        
    } else {
        event->accept();
    }
}

void BBCheckout::onRejected()
{
    BBDEBUG;

    if (!page(pageIds().last())->isComplete())
        BBApplication::instance()->quit();
}
