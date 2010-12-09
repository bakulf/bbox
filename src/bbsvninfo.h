#ifndef _BB_SVN_INFO_H_
#define _BB_SVN_INFO_H_

#include <QObject>

class BBSvnInfo : public QObject
{
    Q_OBJECT

public:
    BBSvnInfo(QObject *parent = 0);
    ~BBSvnInfo();

public:
    void setURL(const QString& url);
    const QString& URL() const;

    void setRevision(uint revision);
    uint revision() const;

private:
    QString m_URL;
    uint m_revision;
};

#endif
