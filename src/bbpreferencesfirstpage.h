/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_PREFERENCES_FIRSTPAGE_H_
#define _BB_PREFERENCES_FIRSTPAGE_H_

#include <QWizardPage>
#include <QPointer>

class QFileDialog;
class QLineEdit;
class QComboBox;
class QCheckBox;

class BBPreferencesFirstPage : public QWizardPage
{
    Q_OBJECT

public:
    BBPreferencesFirstPage();
    virtual ~BBPreferencesFirstPage();

public:
    void initializePage();
    bool validatePage();

private Q_SLOTS:
    void onSVNSearchClicked();
    void onDirectorySearchClicked();

private:
    QLineEdit *m_svnWidget;
    QPointer<QFileDialog> m_svnDialog;

    QLineEdit *m_directoryWidget;
    QPointer<QFileDialog> m_directoryDialog;

    QComboBox *m_timerWidget;
    QCheckBox *m_autocommitWidget;
};

#endif
