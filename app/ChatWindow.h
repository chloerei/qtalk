#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include "ui_ChatWindow.h"
#include "QXmppMessage.h"

class QXmppClient;
class QXmppMessage;

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    ChatWindow(QWidget *parent = 0);
    void setClient(QXmppClient *client);
    void setJid(QString);
    void appendMessage(const QXmppMessage &);

private slots:
    void sendMessage();

private:
    Ui::ChatWindow ui;
    QString m_jid;
    QXmppClient *m_client;

    void changeState(QXmppMessage::State);
};
#endif
