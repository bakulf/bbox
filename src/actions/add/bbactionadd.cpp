#include "bbactionadd.h"

#include "bbsvn.h"
#include "bbapplication.h"
#include "bbsettings.h"
#include "bbdebug.h"

#include <QFileInfo>
#include <QDir>

BBActionAdd::BBActionAdd(const QString& dirname, QObject *parent) :
    BBAction(parent),
    m_dirname(dirname)
{
    BBDEBUG;
}

BBActionAdd::~BBActionAdd()
{
    BBDEBUG;
}

bool BBActionAdd::compare(const BBAction *action)
{
    BBDEBUG;

    const BBActionAdd *a = qobject_cast<const BBActionAdd *>(action);
    if (!a)
        return false;

    bool ret(a->dirname() == m_dirname);
    BBDEBUG << "Compare: " << ret;
    return ret;
}

void BBActionAdd::run()
{
    BBDEBUG;

    m_files = addDirectory(m_dirname);
    runAdd();
}

void BBActionAdd::runAdd()
{
    BBDEBUG;

    if (m_files.isEmpty()) {
        emit done(true);
        return;
    }

    m_currentFile = m_files.takeLast();
    BBDEBUG << "Adding " << m_currentFile;

    m_svn = new BBSvn(this);
    connect(m_svn,
            SIGNAL(done(bool)),
            SLOT(onSvnDone(bool)));

    m_svn->addFile(m_currentFile);
}

void BBActionAdd::onSvnDone(bool status)
{
    BBDEBUG << status;

    if (status == false) {
        BBApplication::instance()->addError(tr("Error adding file '%1'.").arg(m_currentFile));
    }

    m_svn->deleteLater();

    runAdd();
}

QStringList BBActionAdd::addDirectory(const QString& dirname)
{
    BBDEBUG << dirname;

    QFileInfo info(dirname);
    if (!info.exists()) {
        return QStringList();
    }

    QStringList list;

    if (dirname != BBSettings::instance()->directory() &&
        checkExtension(info))
        list << dirname;

    if (info.isDir()) {
        QDir dir(dirname);
        QFileInfoList files = dir.entryInfoList();
        foreach (QFileInfo info, files) {
            if (info.fileName().startsWith("."))
                continue;

            list << addDirectory(info.absoluteFilePath());
        }
    }

    return list;
}

bool BBActionAdd::checkExtension(const QFileInfo& info)
{
    BBDEBUG;

    if (info.isDir())
        return true;

    QString ext = info.completeSuffix();
    if (ext == "mine")
        return false;

    if (!ext.startsWith("r"))
        return true;

    ext.remove(0, 1);
    return (ext.toUInt() == 0);
}
