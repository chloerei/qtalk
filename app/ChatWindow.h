#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include "ui_ChatWindow.h"
#include "QXmppMessage.h"
#include "Preferences.h"
#include "QXmppVCard.h"
#include <QPointer>

class QXmppClient;
class QXmppMessage;
class QTimer;
class QStatusBar;
class QPushButton;
class MessageEdit;
class QXmppVCard;
class ContactInfoDialog;

class ChatWindow : public QMainWindow
{
    Q_OBJECT
public:
    ChatWindow(QString jid, QXmppClient *client, QWidget *parent = 0);
    void appendMessage(const QXmppMessage &);
    void readPref(Preferences *pref);
    void setVCard(QXmppVCard vCard);

signals:
    void sendFile(QString jid, QString fileName);
    void viewContactInfo(const QString &jid);

private slots:
    void sendMessage();
    void sendComposing();
    void pausedTimeout();
    void inactiveTimeout();
    void goneTimeout();
    void openContactInfoDialog();
    void sendFileSlot();

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
    QLabel *m_sendTip;
    QXmppVCard m_vCard;
    QPointer<ContactInfoDialog> m_contactInfoDialog;

    void changeState(QXmppMessage::State);
    void changeSelfState(QXmppMessage::State);
};
#endif
