#ifndef INFOEVENTSUBSCRIBEREQUEST_H
#define INFOEVENTSUBSCRIBEREQUEST_H

#include <QWidget>

namespace Ui {
    class InfoEventSubscribeRequest;
}

class QXmppClient;

class InfoEventSubscribeRequest : public QWidget {
    Q_OBJECT
public:
    explicit InfoEventSubscribeRequest(QString bareJid, QXmppClient *client, QWidget *parent = 0);
    ~InfoEventSubscribeRequest();

signals:
    void needDestory();

private slots:
    void acceptSlot();
    void denySlot();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::InfoEventSubscribeRequest *ui;
    QXmppClient *m_client;
};

#endif // INFOEVENTSUBSCRIBEREQUEST_H
