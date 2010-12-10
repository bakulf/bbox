#include "bbapplication.h"

#include "bbpreferences.h"
#include "bbabout.h"
#include "bbsettings.h"
#include "bbobserver.h"
#include "bbactionmanager.h"
#include "bboperations.h"
#include "bbsendreceive.h"
#include "bbdebug.h"

#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QSplashScreen>

QPointer<BBApplication> BBApplication::m_instance;

BBApplication::BBApplication(int argc, char **argv) :
    QApplication(argc, argv),
    m_errorShown(false),
    m_timer(0),
    m_iconBlink(false)
{
    BBDEBUG;

    m_instance = this;

    qsrand(time(0));
}

BBApplication::~BBApplication()
{
    BBDEBUG;
}

BBApplication* BBApplication::instance()
{
    return m_instance;
}

void BBApplication::splash()
{
    BBDEBUG;
    QPixmap pixmap(":/images/logo.png");
    QSplashScreen splash(pixmap);
    splash.show();

    for(int i=0; i<20; i++) {
        processEvents();
        usleep(100000);
    }
}

void BBApplication::init()
{
    BBDEBUG;

    systemTray();

    if (!BBSettings::instance()->isValid() &&
        !preferences())
        quit();
    else {
        m_observer = new BBObserver(this);

        scheduleRemoteAction();

        BBActionManager::instance()->actionAdd(BBSettings::instance()->directory());
        BBActionManager::instance()->actionLocalChanges();
    }
}

void BBApplication::systemTray()
{
    BBDEBUG;

    QMenu *menu = new QMenu();

    QFont font;
    font.setBold(true);

    m_actionCommit = new QAction(tr("&Update and Send"), this);
    m_actionCommit->setFont(font);
    menu->addAction(m_actionCommit);
    connect (m_actionCommit,
             SIGNAL(triggered()),
             SLOT(onCommitTriggered()));

    menu->addSeparator();

    m_actionCounter = new QAction(this);
    menu->addAction(m_actionCounter);
    connect(BBActionManager::instance(),
            SIGNAL(actionsQueued(int)),
            SLOT(onActionsQueued(int)));

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

    m_systemTray = new QSystemTrayIcon(this);
    m_systemTray->setContextMenu(menu);

    m_systemTray->setIcon(QIcon(":/images/icon.png"));
    m_systemTray->show();

    connect(m_systemTray,
            SIGNAL(messageClicked()),
            SLOT(onMessageClicked()));

    onActionsQueued(0);
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

void BBApplication::onActionsQueued(int counter)
{
    BBDEBUG << "Counter: " << counter;

    m_actionCounter->setText(tr("Actions in queue: %1").arg(counter));
}

void BBApplication::onMessageClicked()
{
    BBDEBUG;
    m_errorShown = false;
}

void BBApplication::scheduleRemoteAction()
{
    BBDEBUG;

    BBActionManager::instance()->actionRemoteChanges();

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

    if (m_errorShown)
        return;

    if (m_systemTray->supportsMessages()) {
        m_systemTray->showMessage(tr("Error!"), error, QSystemTrayIcon::Warning);
        m_errorShown = true;
    }
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
            m_systemTray->showMessage(tr("Sending and receiving..."), tr("Your changes will be shared soon."), QSystemTrayIcon::Information);
            m_sendReceive->start();
        }

    } else {
        m_systemTray->showMessage(tr("Something new!"), tr("Click to share your changes to other users"), QSystemTrayIcon::Information);
    }
}

void BBApplication::update()
{
    BBDEBUG;
    blink(true);

    m_systemTray->showMessage(tr("Something new!"), tr("Click to have changes from other users"), QSystemTrayIcon::Information);
}

void BBApplication::blink(bool enabled)
{
    BBDEBUG << enabled;

    if (enabled == true && !m_timer) {
        m_timer = startTimer(1000);
    } else if(enabled == false && m_timer) {
        m_systemTray->setIcon(QIcon(":/images/icon.png"));
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
        m_systemTray->setIcon(QIcon(":/images/blink.png"));
    else
        m_systemTray->setIcon(QIcon(":/images/noblink.png"));

    m_iconBlink = !m_iconBlink;
}

void BBApplication::onCommitTriggered()
{
    BBDEBUG;
    blink(false);

    BBOperations operations;
    operations.exec();
}

BBObserver* BBApplication::observer()
{
    return m_observer;
}

void BBApplication::onSendReceiveDone(bool status)
{
    BBDEBUG;
    Q_UNUSED(status);

    if (status == true)
        m_systemTray->showMessage(tr("Sending and receiving done"), tr("Your changes have been shared!"), QSystemTrayIcon::Information);
    blink(false);
}
