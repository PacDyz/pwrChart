#ifndef NAMEBUTTON_H
#define NAMEBUTTON_H
#include <QObject>
#include <QString>

class NameButton : public QObject
{
    Q_OBJECT
        Q_PROPERTY(QString textButton READ getTextButton WRITE setTextButton NOTIFY textButtonChanged)
public:
    void setTextButton(QString newTextButton)
    {
        textButton = newTextButton;
    }
    QString getTextButton() const
    {
        return textButton;
    }
    signals:
        void textButtonChanged();
private:
    QString textButton;
};

#endif // NAMEBUTTON_H
