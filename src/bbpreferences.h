#ifndef _BB_PREFERENCES_H_
#define _BB_PREFERENCES_H_

#include <QDialog>
#include <QPointer>

class QFileDialog;
class QLineEdit;
class QComboBox;
class QCheckBox;

class BBPreferences : public QDialog
{
    Q_OBJECT

public:
    BBPreferences();
    virtual ~BBPreferences();

private Q_SLOTS:
    void save();

    void onSVNSearchClicked();
    void onDirectorySearchClicked();

private:
    QPointer<QFileDialog> m_svnDialog;
    QLineEdit *m_svnWidget;

    QPointer<QFileDialog> m_directoryDialog;
    QLineEdit *m_directoryWidget;

    QComboBox *m_timerWidget;

    QCheckBox *m_autocommitWidget;
};

#endif
