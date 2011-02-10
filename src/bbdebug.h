/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_DEBUG_H_
#define _BB_DEBUG_H_

#include <QtDebug>

#define BBDEBUG qDebug() << Q_FUNC_INFO

#define BBDEBUG_LOGFILE "BBox.log.txt"

#endif
