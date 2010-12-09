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
