#include "ChatWindow.h"
#include "QXmppClient.h"
#include "QXmppMessage.h"
#include "QXmppUtils.h"
#include <QTime>
#include <QDomDocument>
#include "XmppMessage.h"

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.messageEdit->setFocus();

    connect(ui.sendButton, SIGNAL(clicked()),
            this, SLOT(sendMessage()));
}

void ChatWindow::setClient(QXmppClient *client)
{
    m_client = client;
}

void ChatWindow::setJid(QString jid)
{
    m_jid = jid;
}

void ChatWindow::appendMessage(const QXmppMessage &o_message)
{
    XmppMessage message(o_message);
    changeState(message.state());
    if (!message.body().isEmpty()){
        //QString bareJid = jidToBareJid(message.from()); 
        ui.messageBrowser->append(QString("%1 %2").arg(message.from()).arg(QTime::currentTime().toString()));
        if (message.html().isEmpty()) {
            ui.messageBrowser->append(message.body());
        } else {
            ui.messageBrowser->append(message.html());
        }
    }
}

void ChatWindow::sendMessage()
{
    XmppMessage message(m_client->getConfiguration().jid(),
                        m_jid,
                        ui.messageEdit->toPlainText());
    message.setHtml(ui.messageEdit->toHtml());
    m_client->sendPacket(message);

    QString c_bareJid = m_client->getConfiguration().jidBare();
    ui.messageBrowser->append(QString("%1 %2").arg(c_bareJid).arg(QTime::currentTime().toString()));
    ui.messageBrowser->append(ui.messageEdit->toHtml());
    ui.messageEdit->clear();
}

void ChatWindow::changeState(QXmppMessage::State state)
{
    QString stateStr;
    switch (state) {
        case QXmppMessage::None :
            stateStr = "None";
            break;
        case QXmppMessage::Active :
            stateStr = "Active";
            break;
        case QXmppMessage::Inactive :
            stateStr = "Inactive";
            break;
        case QXmppMessage::Gone :
            stateStr = "Gone";
            break;
        case QXmppMessage::Composing :
            stateStr = "Composing";
            break;
        case QXmppMessage::Paused :
            stateStr = "Paused";
            break;
        default:
            break;
    }
    ui.stateLabel->setText(QString("State: %1").arg(stateStr));
}
