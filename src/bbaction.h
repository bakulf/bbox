#ifndef _BB_ACTION_H_
#define _BB_ACTION_H_

#include <QObject>

class BBAction : public QObject
{
    Q_OBJECT

public:
    BBAction(QObject *parent = 0);
    ~BBAction();

public:
    virtual void run() = 0;

    virtual bool compare(const BBAction *action) = 0;

Q_SIGNALS:
    void done(bool status);
};

#endif
