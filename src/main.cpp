/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbapplication.h"

#include <QtGui>

int main(int argc, char **argv)
{
  BBApplication app(argc, argv);

  if(!QSystemTrayIcon::isSystemTrayAvailable()) {
    QMessageBox::critical(0,
                          QObject::tr("Systray"),
                          QObject::tr("I cannot detect any system tray on this system."));
    return 1;
  }

  app.setQuitOnLastWindowClosed(false);

  app.splash();
  app.init();

  return app.exec();
}
