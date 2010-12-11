/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbpreferences.h"

#include "bbpreferencesfirstpage.h"
#include "bbpreferencesmiddlepage.h"
#include "bbpreferenceslastpage.h"
#include "bbapplication.h"
#include "bbsettings.h"
#include "bbdebug.h"
#include "bbconst.h"

#include <QCloseEvent>
#include <QMessageBox>

BBPreferences::BBPreferences()
{
    BBDEBUG;

    resize(640, 480);
    setWindowTitle(QString(BBPACKAGE " - %1").arg("Preferences"));
    setWindowIcon(QIcon(BB_ICON_IMAGE));

    addPage(new BBPreferencesFirstPage());
    addPage(new BBPreferencesMiddlePage());
    addPage(new BBPreferencesLastPage());

    setOption(QWizard::NoCancelButton, true);

    setPixmap(QWizard::LogoPixmap,      QPixmap::fromImage(QImage(BB_ICON_IMAGE)));
    setPixmap(QWizard::WatermarkPixmap, QPixmap::fromImage(QImage(BB_LOGO_IMAGE)));

    setModal(true);
    setFocus();
}

BBPreferences::~BBPreferences()
{
    BBDEBUG;
}

void BBPreferences::closeEvent(QCloseEvent *event)
{
    BBDEBUG;

    if (!BBSettings::instance()->isValid()) {
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
