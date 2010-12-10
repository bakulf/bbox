/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

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
