/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbaction.h"

#include "bbdebug.h"

BBAction::BBAction(QObject *parent) :
    QObject(parent)
{
    BBDEBUG;
}

BBAction::~BBAction()
{
    BBDEBUG;
}
