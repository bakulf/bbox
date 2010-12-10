#include "bbpreferences.h"

#include "bbsettings.h"
#include "bbdebug.h"
#include "bbconst.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>

BBPreferences::BBPreferences()
{
    BBDEBUG;

    resize(640, 0);
    setWindowTitle(QString(BBPACKAGE " - %1").arg("Preferences"));

    QVBoxLayout *box = new QVBoxLayout();
    setLayout(box);

    {
        QHBoxLayout *layout = new QHBoxLayout();
        box->addLayout(layout);

        {
            QLabel *label = new QLabel();
            label->setAlignment(Qt::AlignCenter);
            label->setPixmap(QPixmap::fromImage(QImage(BB_ICON_IMAGE)));
            layout->addWidget(label);
        }

        {
            QLabel *label = new QLabel(tr("Preferences"));
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

    {
        QLabel *label = new QLabel(tr("Directory:"));
        layout->addWidget(label, row, 0);

        QHBoxLayout *box = new QHBoxLayout();
        layout->addLayout(box, row, 1);

        m_directoryWidget = new QLineEdit();
        m_directoryWidget->setText(BBSettings::instance()->directory());
        box->addWidget(m_directoryWidget);

        QPushButton *button = new QPushButton(tr("Browse"));
        box->addWidget(button);
        connect(button,
                SIGNAL(clicked()),
                SLOT(onDirectorySearchClicked()));
    }

    row++;
    {
        QLabel *label = new QLabel(tr("Subversion:"));
        layout->addWidget(label, row, 0);

        QHBoxLayout *box = new QHBoxLayout();
        layout->addLayout(box, row, 1);

        m_svnWidget = new QLineEdit();
        m_svnWidget->setText(BBSettings::instance()->svn());
        box->addWidget(m_svnWidget);

        QPushButton *button = new QPushButton(tr("Browse"));
        box->addWidget(button);
        connect(button,
                SIGNAL(clicked()),
                SLOT(onSVNSearchClicked()));
    }

    row++;
    {
        QLabel *label = new QLabel(tr("Timer for remote check:"));
        layout->addWidget(label, row, 0);

        m_timerWidget = new QComboBox();
        m_timerWidget->setEditable(false);
        layout->addWidget(m_timerWidget, row, 1);

        int index(0);
        m_timerWidget->insertItem(index++, tr("1 minute"),    1);
        m_timerWidget->insertItem(index++, tr("2 minutes"),   2);
        m_timerWidget->insertItem(index++, tr("5 minutes"),   5);
        m_timerWidget->insertItem(index++, tr("10 minutes"), 10);
        m_timerWidget->insertItem(index++, tr("20 minutes"), 20);
        m_timerWidget->insertItem(index++, tr("60 minutes"), 60);

        uint timer = BBSettings::instance()->timerRemoteAction();
        if (timer == 1)
            m_timerWidget->setCurrentIndex(0);
        else if (timer == 2)
            m_timerWidget->setCurrentIndex(1);
        else if (timer == 5)
            m_timerWidget->setCurrentIndex(2);
        else if (timer == 10)
            m_timerWidget->setCurrentIndex(3);
        else if (timer == 20)
            m_timerWidget->setCurrentIndex(4);
        else if (timer == 60)
            m_timerWidget->setCurrentIndex(5);
    }

    row++;
    {
        QLabel *label = new QLabel(tr("AutoCommit on changes:"));
        layout->addWidget(label, row, 0);

        m_autocommitWidget = new QCheckBox(tr("Enabled"));
        m_autocommitWidget->setChecked(BBSettings::instance()->autoCommit());
        layout->addWidget(m_autocommitWidget, row, 1);
    }

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    box->addLayout(buttonLayout);

    buttonLayout->addWidget(new QWidget(), 1, 0);

    {
        QPushButton *button = new QPushButton(tr("&Cancel"));
        buttonLayout->addWidget(button, 0, 0);
        connect(button, SIGNAL(clicked()), SLOT(reject()));
    }

    {
        QPushButton *button = new QPushButton(tr("&Save"));
        buttonLayout->addWidget(button, 0, 0);
        connect(button, SIGNAL(clicked()), SLOT(save()));
    }

    setFocus();
}

BBPreferences::~BBPreferences()
{
    BBDEBUG;
}

void BBPreferences::save()
{
    BBDEBUG;

    if (m_svnWidget->text().isEmpty() ||
        m_directoryWidget->text().isEmpty()) {
        QMessageBox::warning(this,
                             QString(BBPACKAGE " - %1").arg("Preferences"),
                             tr("Please, all the fields are required."));
        return;
    }

    BBSettings::instance()->setSvn(m_svnWidget->text());
    BBSettings::instance()->setDirectory(m_directoryWidget->text());
    BBSettings::instance()->setTimerRemoteAction(m_timerWidget->itemData(m_timerWidget->currentIndex()).toUInt());
    BBSettings::instance()->setAutoCommit(m_autocommitWidget->checkState() == Qt::Checked);

    accept();
}

void BBPreferences::onSVNSearchClicked()
{
    BBDEBUG;

    if (m_svnDialog.isNull()) {
        m_svnDialog = new QFileDialog(this);
        m_svnDialog->setAcceptMode(QFileDialog::AcceptOpen);
        m_svnDialog->setFileMode(QFileDialog::ExistingFile);
        m_svnDialog->setWindowTitle(tr("Select the 'subversion' tool."));
        m_svnDialog->setModal(true);

        connect(m_svnDialog,
                SIGNAL(fileSelected(QString)),
                m_svnWidget,
                SLOT(setText(QString)));
    }

    m_svnDialog->show();
    m_svnDialog->setFocus();
}

void BBPreferences::onDirectorySearchClicked()
{
    BBDEBUG;

    if (m_directoryDialog.isNull()) {
        m_directoryDialog = new QFileDialog(this);
        m_directoryDialog->setAcceptMode(QFileDialog::AcceptOpen);
        m_directoryDialog->setFileMode(QFileDialog::Directory);
        m_directoryDialog->setWindowTitle(tr("Select the subversion directory."));
        m_directoryDialog->setModal(true);

        connect(m_directoryDialog,
                SIGNAL(fileSelected(QString)),
                m_directoryWidget,
                SLOT(setText(QString)));
    }

    m_directoryDialog->show();
    m_directoryDialog->setFocus();
}
