#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include "ui_ChatWindow.h"
#include "QXmppMessage.h"
#include "Preferences.h"

class QXmppClient;
class QXmppMessage;
class QTimer;
class QStatusBar;
class QPushButton;
class MessageEdit;

class ChatWindow : public QMainWindow
{
    Q_OBJECT
public:
    ChatWindow(QWidget *parent = 0);
    void setClient(QXmppClient *client);
    void setJid(QString);
    void appendMessage(const QXmppMessage &);
    void readPref(Preferences *pref);

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
    MessageEdit *m_editor;
    QStatusBar *m_statusBar;
    QPushButton *m_sendButton;

    void changeState(QXmppMessage::State);
    void changeSelfState(QXmppMessage::State);
};
#endif
