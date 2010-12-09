#include "bbsvnstatus.h"

#include "bbdebug.h"

BBSvnStatus::BBSvnStatus(QObject *parent) :
    QObject(parent),
    m_status(StatusUnknown),
    m_locked(false)
{
    BBDEBUG;
}

BBSvnStatus::~BBSvnStatus()
{
    BBDEBUG;
}

void BBSvnStatus::setStatus(BBSvnStatus::Status status)
{
    BBDEBUG << status;
    m_status = status;
}

BBSvnStatus::Status BBSvnStatus::status() const
{
    return m_status;
}

void BBSvnStatus::setLocked(bool locked)
{
    BBDEBUG << locked;
    m_locked = locked;
}

bool BBSvnStatus::locked() const
{
    return m_locked;
}

void BBSvnStatus::setFile(const QString &filename)
{
    BBDEBUG << filename;
    m_filename = filename;
}

const QString& BBSvnStatus::file() const
{
    return m_filename;
}

bool BBSvnStatus::isValid()
{
    BBDEBUG;

    if (m_status == StatusUnknown)
        return false;

    if (m_filename.isEmpty())
        return false;

    return true;
}
