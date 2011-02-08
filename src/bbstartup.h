/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_STARTUP_H_
#define _BB_STARTUP_H_

#include <QSettings>

class BBStartup : public QObject
{
    Q_OBJECT

private:
    BBStartup();

public:
    static BBStartup *instance();
    virtual ~BBStartup();

private Q_SLOTS:
    void reset();

private:
    void addStartup();
    void removeStartup();
};

#endif
