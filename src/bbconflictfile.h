#ifndef _BB_CONFLICT_FILE_H_
#define _BB_CONFLICT_FILE_H_

#include <QObject>
#include <QPointer>

class BBSvnStatus;
class QRadioButton;
class QLabel;

class BBConflictFile : public QObject
{
    Q_OBJECT

public:
    BBConflictFile(QObject *parent = 0);
    ~BBConflictFile();

public:
    void setStatus(BBSvnStatus *status);
    const BBSvnStatus* status() const;

    void setLocalButton(QRadioButton *button);
    const QRadioButton* localButton() const;

    void setLabel(QLabel *label);
    QLabel* label() const;

private:
    QPointer<BBSvnStatus> m_status;
    QPointer<QRadioButton> m_button;
    QPointer<QLabel> m_label;
};

#endif
