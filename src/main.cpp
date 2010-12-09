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

  app.init();

  return app.exec();
}
