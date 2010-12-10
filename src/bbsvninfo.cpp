/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbsvninfo.h"

#include "bbdebug.h"

BBSvnInfo::BBSvnInfo(QObject *parent) :
    QObject(parent),
    m_revision(0)
{
    BBDEBUG;
}

BBSvnInfo::~BBSvnInfo()
{
    BBDEBUG;
}

void BBSvnInfo::setURL(const QString & url)
{
    BBDEBUG << url;
    m_URL = url;
}

const QString& BBSvnInfo::URL() const
{
    return m_URL;
}

void BBSvnInfo::setRevision(uint revision)
{
    BBDEBUG << revision;
    m_revision = revision;
}

uint BBSvnInfo::revision() const
{
    return m_revision;
}

