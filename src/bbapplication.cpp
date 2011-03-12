/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbapplication.h"

#include "bbpreferences.h"
#include "bbabout.h"
#include "bbsettings.h"
#include "bbstartup.h"
#include "bbobserver.h"
#include "bbactionmanager.h"
#include "bboperations.h"
#include "bbsendreceive.h"
#include "bblogs.h"
#include "bbsvn.h"
#include "bbsvnstatus.h"
#include "bbdebug.h"
#include "bbconst.h"

#include <QTime>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QSplashScreen>
#include <QDesktopServices>
#include <QUrl>

#ifdef Q_OS_WIN32
    #include <windows.h>
    #include <winbase.h>
#endif

QPointer<BBApplication> BBApplication::m_instance;

BBApplication::BBApplication(int argc, char **argv) :
    QApplication(argc, argv),
    m_timer(0),
    m_iconBlink(false)
{
    BBDEBUG;

    m_instance = this;

    qsrand(QTime::currentTime().msec());
}

BBApplication::~BBApplication()
{
    BBDEBUG;
    delete m_systemTray;
}

BBApplication* BBApplication::instance()
{
    return m_instance;
}

void BBApplication::splash()
{
    BBDEBUG;
    QPixmap pixmap(BB_LOGO_IMAGE);
    QSplashScreen splash(pixmap);
    splash.show();

    for(int i=0; i<20; ++i) {
        processEvents();
#ifdef Q_OS_WIN32
        Sleep(100);
#else
        usleep(100000);
#endif
    }
}

void BBApplication::init()
{
    BBDEBUG;

    systemTray();

    if (!BBSettings::instance()->isValid() &&
        !preferences()) {
        quit();
        return;
    }

    // Instance of startup created:
    BBStartup::instance();

    m_observer = new BBObserver(this);

    scheduleRemoteAction();
}

void BBApplication::systemTray()
{
    BBDEBUG;

    QMenu *menu = new QMenu();

    QFont font;
    font.setBold(true);

    m_actionCommit = new QAction(tr("&Sync"), this);
    m_actionCommit->setFont(font);
    menu->addAction(m_actionCommit);
    connect (m_actionCommit,
             SIGNAL(triggered()),
             SLOT(onCommitTriggered()));

    QAction *actionChanges = new QAction(tr("&Changes"), this);
    menu->addAction(actionChanges);
    m_menuChanges = new QMenu();
    actionChanges->setMenu(m_menuChanges);
    changes(QList<BBSvnStatus*>());

    menu->addSeparator();

    m_actionCommit = new QAction(tr("See &logs"), this);
    menu->addAction(m_actionCommit);
    connect (m_actionCommit,
             SIGNAL(triggered()),
             SLOT(onLogsTriggered()));

    menu->addSeparator();

    QAction *actionOpen = new QAction(tr("&Open folder"), this);
    menu->addAction(actionOpen);
    connect (actionOpen,
             SIGNAL(triggered()),
             SLOT(onOpenTriggered()));

    menu->addSeparator();

    QAction *preferences = new QAction(tr("&Preferences"), this);
    menu->addAction(preferences);
    connect (preferences,
             SIGNAL(triggered()),
             SLOT(preferences()));

    QAction *about = new QAction(tr("&About"), this);
    menu->addAction(about);
    connect (about,
             SIGNAL(triggered()),
             SLOT(about()));

    menu->addSeparator();

    QAction *actionQuit = new QAction(tr("&Quit"), this);
    menu->addAction(actionQuit);
    connect (actionQuit,
             SIGNAL(triggered()),
             SLOT(quit()));

    m_systemTray = new QSystemTrayIcon();
    m_systemTray->setContextMenu(menu);

    m_systemTray->setIcon(QIcon(BB_ICON_IMAGE));
    m_systemTray->show();

    connect(m_systemTray,
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(onActivated(QSystemTrayIcon::ActivationReason)));
}

bool BBApplication::preferences()
{
    BBDEBUG;

    BBPreferences preferences;
    preferences.exec();

    if (!BBSettings::instance()->isValid()) {
        QMessageBox::critical(0,
                              BBPACKAGE,
                              tr("No configuration, no party."));
        return false;
    }

    return true;
}

void BBApplication::about()
{
    BBDEBUG;

    BBAbout about;
    about.exec();
}

void BBApplication::scheduleRemoteAction()
{
    BBDEBUG;

    m_observer->scheduleRemoteChanges();

    uint time = BBSettings::instance()->timerRemoteAction() * 10;
    int when(qrand() % ((time / 2) * 6) + time); // Random between time/2 and time
    BBDEBUG << "Next: " << when << " milliseconds.";

    QTimer::singleShot(when * 1000,
                       this,
                       SLOT(scheduleRemoteAction()));
}

void BBApplication::addError(const QString& error)
{
    BBDEBUG;

    showMessage(tr("Error!"), error, true);
}

void BBApplication::commit()
{
    BBDEBUG;
    blink(true);

    if (BBSettings::instance()->autoCommit()) {
        if (m_sendReceive.isNull()) {
            m_sendReceive = new BBSendReceive(this);
            connect(m_sendReceive,
                    SIGNAL(done(bool)),
                    SLOT(onSendReceiveDone(bool)));
        }

        if (m_sendReceive->isRunning() == false) {
            m_sendReceive->start();
        }

        resetLastMessage();

    } else {
        showMessage(tr("Something new!"), tr("Click to share your changes to other users"));
    }
}

void BBApplication::update()
{
    BBDEBUG;
    blink(true);

    showMessage(tr("Something new!"), tr("Click to have changes from other users"));
}

void BBApplication::blink(bool enabled)
{
    BBDEBUG << enabled;

    if (enabled == true && !m_timer) {
        m_timer = startTimer(BB_BLINK_TIMEOUT);
    } else if(enabled == false && m_timer) {
        m_systemTray->setIcon(QIcon(BB_ICON_IMAGE));
        killTimer(m_timer);
        m_timer = 0;
    }
}

void BBApplication::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if (m_timer == 0)
        return;

    if (!m_iconBlink)
        m_systemTray->setIcon(QIcon(BB_BLINK_IMAGE));
    else
        m_systemTray->setIcon(QIcon(BB_ICON_IMAGE));

    m_iconBlink = !m_iconBlink;
}

void BBApplication::onCommitTriggered()
{
    BBDEBUG;

    if (m_operations.isNull()) {
        blink(true);

        resetLastMessage();
        m_operations = new BBOperations();
        connect(m_operations.data(),
                SIGNAL(destroyed()),
                SLOT(onOperationsDestroyed()));

    } else {
        m_operations->activateWindow();
    }

    m_operations->show();
}

void BBApplication::onOperationsDestroyed()
{
    BBDEBUG;

    blink(false);
    m_operations = 0;
}

BBObserver* BBApplication::observer()
{
    return m_observer;
}

void BBApplication::onSendReceiveDone(bool status)
{
    BBDEBUG;
    Q_UNUSED(status);

    blink(false);
}

void BBApplication::onOpenTriggered()
{
    BBDEBUG;
    QDesktopServices::openUrl(QUrl::fromLocalFile(BBSettings::instance()->directory()).toString());
}

void BBApplication::onLogsTriggered()
{
    BBDEBUG;
    new BBLogs(this);
}

void BBApplication::onActivated(QSystemTrayIcon::ActivationReason reason)
{
    BBDEBUG << reason;

    if (reason == QSystemTrayIcon::DoubleClick)
        onCommitTriggered();
}

void BBApplication::changes(const QList<BBSvnStatus*>& changes)
{
    BBDEBUG << changes;

    m_menuChanges->clear();

    if (changes.isEmpty()) {
        QFont font;
        font.setStyle(QFont::StyleItalic);

        QAction* emptyAction = new QAction(tr("No changes"), this);
        emptyAction->setFont(font);
        m_menuChanges->addAction(emptyAction);
        return;
    }

    foreach(BBSvnStatus* status, changes) {
        QString message;

        switch(status->status()) {
            case BBSvnStatus::StatusUnknown:
               message = tr("%1 (unknown operation)").arg(status->file().remove(BBSettings::instance()->directory()));
               break;

            case BBSvnStatus::StatusAdded:
               message = tr("%1 added").arg(status->file().remove(BBSettings::instance()->directory()));
               break;

            case BBSvnStatus::StatusConflicted:
               message = tr("%1 conflicted").arg(status->file().remove(BBSettings::instance()->directory()));
               break;

            case BBSvnStatus::StatusDeleted:
               message = tr("%1 deleted").arg(status->file().remove(BBSettings::instance()->directory()));
               break;

            case BBSvnStatus::StatusModified:
               message = tr("%1 modified").arg(status->file().remove(BBSettings::instance()->directory()));
               break;

            case BBSvnStatus::StatusReplaced:
               message = tr("%1 replaced").arg(status->file().remove(BBSettings::instance()->directory()));
               break;

            case BBSvnStatus::StatusNew:
               message = tr("%1 is new").arg(status->file().remove(BBSettings::instance()->directory()));
               break;

            case BBSvnStatus::StatusMissing:
               message = tr("%1 missing").arg(status->file().remove(BBSettings::instance()->directory()));
               break;

            case BBSvnStatus::StatusUpdated:
               message = tr("%1 updated").arg(status->file().remove(BBSettings::instance()->directory()));
               break;

            case BBSvnStatus::StatusMerged:
               message = tr("%1 merged").arg(status->file().remove(BBSettings::instance()->directory()));
               break;

            case BBSvnStatus::StatusExisted:
               message = tr("%1 existed").arg(status->file().remove(BBSettings::instance()->directory()));
               break;

            case BBSvnStatus::StatusObstructed:
               message = tr("%1 obstructed").arg(status->file().remove(BBSettings::instance()->directory()));
               break;
        }

        QAction* action = new QAction(message, this);
        m_menuChanges->addAction(action);
    }
}

void BBApplication::showMessage(const QString &title, const QString &message, bool isWarning)
{
    BBDEBUG << title << message << isWarning;

    if (!m_systemTray->supportsMessages())
        return;

    // The fastest way to have something 'unique':
    QString hash(QString("%1|%2|%3").arg(title).arg(message).arg(isWarning));
    if (hash == m_lastMessage)
        return;

    m_lastMessage = hash;

    m_systemTray->showMessage(title, message, isWarning ? QSystemTrayIcon::Warning :
                                                          QSystemTrayIcon::Information);
}

void BBApplication::resetLastMessage()
{
    m_lastMessage.clear();
}
