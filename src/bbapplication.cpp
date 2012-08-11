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
#else
    #include <unistd.h>
#endif

QPointer<BBApplication> BBApplication::m_instance;

BBApplication::BBApplication(int &argc, char **argv) :
    QApplication(argc, argv, true),
    m_timer(0),
    m_iconBlink(false),
    m_commitFromUI(false)
{
    BBDEBUG;

    m_instance = this;

    setApplicationName(BB_APP_NAME);
    setOrganizationName(BB_ORG_NAME);

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

    m_actionCommit = new QAction(BB_ACTION_MENU_COMMIT, this);
    m_actionCommit->setFont(font);
    menu->addAction(m_actionCommit);
    connect (m_actionCommit,
             SIGNAL(triggered()),
             SLOT(onCommitTriggered()));

    QAction *actionChanges = new QAction(BB_ACTION_MENU_CHANGES, this);
    menu->addAction(actionChanges);
    m_menuChanges = new QMenu();
    actionChanges->setMenu(m_menuChanges);
    changes(QList<BBSvnStatus*>());

    menu->addSeparator();

    QAction *actionLog = new QAction(BB_ACTION_MENU_LOG, this);
    menu->addAction(actionLog);
    connect (actionLog,
             SIGNAL(triggered()),
             SLOT(onLogsTriggered()));

    menu->addSeparator();

    QAction *actionOpen = new QAction(BB_ACTION_MENU_OPEN, this);
    menu->addAction(actionOpen);
    connect (actionOpen,
             SIGNAL(triggered()),
             SLOT(onOpenTriggered()));

    menu->addSeparator();

    QAction *preferences = new QAction(BB_ACTION_MENU_PREFS, this);
    menu->addAction(preferences);
    connect (preferences,
             SIGNAL(triggered()),
             SLOT(preferences()));

    QAction *about = new QAction(BB_ACTION_MENU_ABOUT, this);
    menu->addAction(about);
    connect (about,
             SIGNAL(triggered()),
             SLOT(about()));

    menu->addSeparator();

    QAction *actionQuit = new QAction(BB_ACTION_MENU_QUIT, this);
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
    BBDEBUG << "Next: " << when << " seconds.";

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
        commitStart();
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

void BBApplication::commitStart()
{
    BBDEBUG;

    if (m_sendReceive.isNull()) {
        m_sendReceive = new BBSendReceive(this);
        connect(m_sendReceive,
                SIGNAL(localChangeDone(bool)),
                SLOT(onSendReceiveLocalChangeDone(bool)));
        connect(m_sendReceive,
                SIGNAL(commitDone(bool)),
                SLOT(onSendReceiveCommitDone(bool)));
        connect(m_sendReceive,
                SIGNAL(revisionDone(bool)),
                SLOT(onSendReceiveRevisionDone(bool)));
        connect(m_sendReceive,
                SIGNAL(done(bool)),
                SLOT(onSendReceiveDone(bool)));
    }

    if (m_sendReceive->isRunning() == false) {
        m_sendReceive->start();
    }

    resetLastMessage();
}

void BBApplication::onCommitTriggered()
{
    BBDEBUG;

    // This will enable the notification:
    m_commitFromUI = true;

    commitStart();

    updateStatus(BB_ACTION_MENU_STATUS_FS);
}

BBObserver* BBApplication::observer()
{
    return m_observer;
}

void BBApplication::updateStatus(const QString &string)
{
    BBDEBUG;

    if (m_actionCommit->text() != string)
        m_actionCommit->setText(string);
}

void BBApplication::onSendReceiveLocalChangeDone(bool status)
{
    BBDEBUG;

    if (m_commitFromUI) {
        showMessage(tr("Syncing..."),
                    status ? BB_ACTION_MENU_STATUS_FS_SUCCESS : BB_ACTION_MENU_STATUS_FS_FAILED,
                    !status);
    }

    if (status == false) {
        updateStatus(BB_ACTION_MENU_STATUS_FS_FAILED);
        return;
    }

    updateStatus(BB_ACTION_MENU_STATUS_COMMIT);
}

void BBApplication::onSendReceiveCommitDone(bool status)
{
    BBDEBUG;

    if (m_commitFromUI) {
        showMessage(tr("Syncing..."),
                    status ? BB_ACTION_MENU_STATUS_COMMIT_SUCCESS : BB_ACTION_MENU_STATUS_COMMIT_FAILED,
                    !status);
    }

    if (status == false) {
        updateStatus(BB_ACTION_MENU_STATUS_COMMIT_FAILED);
        return;
    }

    updateStatus(BB_ACTION_MENU_STATUS_REVISION);
}

void BBApplication::onSendReceiveRevisionDone(bool status)
{
    BBDEBUG;

    if (m_commitFromUI) {
        showMessage(tr("Syncing..."),
                    status ? BB_ACTION_MENU_STATUS_REVISION_SUCCESS : BB_ACTION_MENU_STATUS_REVISION_FAILED,
                    !status);
    }

    if (status == false) {
        updateStatus(BB_ACTION_MENU_STATUS_REVISION_FAILED);
        return;
    }
}

void BBApplication::onSendReceiveDone(bool status)
{
    BBDEBUG;

    blink(false);

    if (m_commitFromUI) {
        showMessage(tr("Syncing..."),
                    status ? BB_ACTION_MENU_STATUS_SUCCESS : BB_ACTION_MENU_STATUS_REVISION_FAILED,
                    !status);

        // Disable the notification for the next commit
        m_commitFromUI = false;
    }

    updateStatus(BB_ACTION_MENU_COMMIT);
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
