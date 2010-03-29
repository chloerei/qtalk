#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include "ui_ChatWindow.h"

class QXmppClient;
class QXmppMessage;

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    ChatWindow(QWidget *parent = 0);
    void setClient(QXmppClient *client);
    void setBareJid(QString);
    void appendMessage(const QXmppMessage &);

private slots:
    void sendMessage();

private:
    Ui::ChatWindow ui;
    QString m_bareJid;
    QXmppClient *m_client;
};
#endif
