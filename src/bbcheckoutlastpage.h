/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_CHECKOUT_LASTPAGE_H_
#define _BB_CHECKOUT_LASTPAGE_H_

#include <QWizardPage>
#include <QPointer>

class QListWidget;
class QLabel;
class BBSvn;

class BBCheckoutLastPage : public QWizardPage
{
    Q_OBJECT

public:
    BBCheckoutLastPage();
    virtual ~BBCheckoutLastPage();

public:
    bool isComplete() const;
    void initializePage();

private Q_SLOTS:
    void onSvnDone(bool status);
    void onSvnRead();

private:
    QLabel *m_errorLabel;
    QListWidget *m_listWidget;
    QLabel *m_counterLabel;

    QPointer<BBSvn> m_svn;
    bool m_complete;
    uint m_counter;
};

#endif
