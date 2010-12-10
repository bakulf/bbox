/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#include "bbconflictfile.h"

#include "bbsvnstatus.h"
#include "bbdebug.h"

#include <QRadioButton>
#include <QLabel>

BBConflictFile::BBConflictFile(QObject *parent) :
    QObject(parent)
{
    BBDEBUG;
}

BBConflictFile::~BBConflictFile()
{
    BBDEBUG;
}

void BBConflictFile::setStatus(BBSvnStatus *status)
{
    BBDEBUG << status;
    m_status = status;
}

const BBSvnStatus* BBConflictFile::status() const
{
    return m_status;
}

void BBConflictFile::setLocalButton(QRadioButton *button)
{
    BBDEBUG << button;
    m_button = button;
}

const QRadioButton* BBConflictFile::localButton() const
{
    return m_button;
}

void BBConflictFile::setLabel(QLabel *label)
{
    BBDEBUG << label;
    m_label = label;
}

QLabel* BBConflictFile::label() const
{
    return m_label;
}
