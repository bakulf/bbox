#include "bbactioncleanup.h"

#include "bbsvn.h"
#include "bbapplication.h"
#include "bbdebug.h"

BBActionCleanup::BBActionCleanup(QObject *parent) :
    BBAction(parent)
{
    BBDEBUG;
}

BBActionCleanup::~BBActionCleanup()
{
    BBDEBUG;
}

bool BBActionCleanup::compare(const BBAction *action)
{
    BBDEBUG;

    const BBActionCleanup *a = qobject_cast<const BBActionCleanup *>(action);
    if (!a)
        return false;

    BBDEBUG << "Compare: Yes!";
    return true;
}

void BBActionCleanup::run()
{
    BBDEBUG;

    m_svn = new BBSvn(this);
    connect(m_svn,
            SIGNAL(done(bool)),
            SLOT(onSvnDone(bool)));

    m_svn->cleanup();
}

void BBActionCleanup::onSvnDone(bool status)
{
    BBDEBUG << status;

    if (status == false) {
        BBApplication::instance()->addError(tr("Error cleaning the directory."));
    }

    m_svn->deleteLater();
    emit done(status);
}
