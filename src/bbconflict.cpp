#include "bbconflict.h"

#include "bbsvn.h"
#include "bbsvnstatus.h"
#include "bbconflictfile.h"
#include "bbdebug.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>

QPointer<BBConflict> BBConflict::m_dialog;

void BBConflict::check(QList<BBSvnStatus*> list)
{
    BBDEBUG << list;

    foreach(BBSvnStatus* status, list) {
        if (status->status() != BBSvnStatus::StatusConflicted) {
            list.removeAll(status);
        }
    }

    if (list.isEmpty())
        return;

    if (!BBConflict::m_dialog.isNull())
        return;

    BBConflict::m_dialog = new BBConflict(list);
    BBConflict::m_dialog->exec();
    BBConflict::m_dialog->deleteLater();
}

BBConflict::BBConflict(const QList<BBSvnStatus*> list)
{
    BBDEBUG << list;

    resize(640, 0);
    setWindowTitle(tr(BBPACKAGE " - %1").arg(tr("Conflict detected!")));

    QVBoxLayout *box = new QVBoxLayout();
    setLayout(box);

    {
        QHBoxLayout *layout = new QHBoxLayout();
        box->addLayout(layout);

        {
            QLabel *label = new QLabel();
            label->setAlignment(Qt::AlignCenter);
            label->setPixmap(QPixmap::fromImage(QImage(":images/icon.png")));
            layout->addWidget(label);
        }

        {
            QLabel *label = new QLabel(tr("Manage conflicts"));
            label->setAlignment(Qt::AlignCenter);
            layout->addWidget(label);

            QFont font;
            font.setBold(true);
            font.setPointSize(font.pointSize() * 2);
            label->setFont(font);
        }

        layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    }

    QFrame *frame = new QFrame();
    frame->setFrameStyle(QFrame::Panel);
    box->addWidget(frame);

    QGridLayout *layout = new QGridLayout();
    frame->setLayout(layout);

    int row(0);

    foreach(BBSvnStatus* status, list) {
        BBConflictFile *file = new BBConflictFile(this);
        file->setStatus(status);

        m_list << file;

        QLabel *label = new QLabel(status->file());
        layout->addWidget(label, row, 0);

        QButtonGroup *group = new QButtonGroup();

        {
            QRadioButton *button = new QRadioButton(tr("Local"));
            button->setChecked(true);
            group->addButton(button);
            layout->addWidget(button, row, 1);
            file->setLocalButton(button);
        }

        {
            QRadioButton *button = new QRadioButton(tr("Remote"));
            group->addButton(button);
            layout->addWidget(button, row, 2);
        }

        {
            QLabel *label = new QLabel("");
            label->setAlignment(Qt::AlignRight);
            layout->addWidget(label, row, 3);
            file->setLabel(label);
        }

        row++;
    }

    layout->setColumnStretch(0, 2);
    layout->setColumnStretch(3, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    box->addLayout(buttonLayout);

    buttonLayout->addWidget(new QWidget(), 1, 0);

    {
        QPushButton *button = new QPushButton(tr("&Apply"));
        buttonLayout->addWidget(button, 0, 0);
        connect(button, SIGNAL(clicked()), SLOT(apply()));
    }

    setFocus();
}

BBConflict::~BBConflict()
{
    BBDEBUG;
    BBConflict::m_dialog = 0;
}

void BBConflict::apply()
{
    BBDEBUG;

    m_svn = new BBSvn();
    connect(m_svn,
            SIGNAL(done(bool)),
            SLOT(applyMerge(bool)));

    applyMerge(true /* First status is fake */);
}

void BBConflict::applyMerge(bool status)
{
    BBDEBUG << status;

    if (status == false && !m_conflictFile.isNull()) {
        updateStatus(m_conflictFile->label(), Error);
        m_conflictFile->deleteLater();
        return;
    }

    if (!m_conflictFile.isNull()) {
        updateStatus(m_conflictFile->label(), Done);
        m_conflictFile->deleteLater();
    }

    if (m_list.isEmpty()) {
        accept();
        return;
    }

    m_conflictFile = m_list.takeFirst();
    updateStatus(m_conflictFile->label(), Running);

    m_svn->resolveConflict(m_conflictFile->status()->file(), m_conflictFile->localButton()->isChecked());
}

void BBConflict::updateStatus(QLabel *label, Status status)
{
    BBDEBUG << label << status;

    switch(status) {
        case Empty:
            label->setText(QString());
            break;
        case Running:
            label->setText(tr("In progress"));
            break;
        case Error:
            label->setText(tr("Error"));
            break;
        case Done:
            label->setText(tr("Done"));
            break;
    }
}
