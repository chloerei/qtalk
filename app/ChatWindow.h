#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include "ui_ChatWindow.h"
#include "QXmppMessage.h"

class QXmppClient;
class QXmppMessage;
class QTimer;

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
    void sendComposing();
    void pausedTimeout();
    void inactiveTimeout();
    void goneTimeout();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::ChatWindow ui;
    QString m_jid;
    QXmppClient *m_client;
    QXmppMessage::State m_selfState; // self state, se for send state message
    QTimer *m_pausedTimer;
    QTimer *m_inactiveTimer;
    QTimer *m_goneTimer;

    void changeState(QXmppMessage::State);
    void changeSelfState(QXmppMessage::State);
};
#endif
