#ifndef INFOEVENTSUBSCRIBEREQUEST_H
#define INFOEVENTSUBSCRIBEREQUEST_H

#include <QWidget>

namespace Ui {
    class InfoEventSubscribeRequest;
}

class InfoEventSubscribeRequest : public QWidget {
    Q_OBJECT
public:
    explicit InfoEventSubscribeRequest(QString bareJid, QWidget *parent = 0);
    ~InfoEventSubscribeRequest();

signals:
    void accept(const QString &bareJid);
    void deny(const QString &bareJid);

private slots:
    void acceptSlot();
    void denySlot();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::InfoEventSubscribeRequest *ui;
};

#endif // INFOEVENTSUBSCRIBEREQUEST_H
