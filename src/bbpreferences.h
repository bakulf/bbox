/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_PREFERENCES_H_
#define _BB_PREFERENCES_H_

#include <QWizard>

class BBPreferences : public QWizard
{
    Q_OBJECT

public:
    BBPreferences();
    virtual ~BBPreferences();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif
