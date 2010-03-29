#include "ChatWindow.h"
#include "QXmppClient.h"
#include "QXmppMessage.h"
#include "QXmppUtils.h"
#include <QTime>

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

void ChatWindow::setBareJid(QString bareJid)
{
    m_bareJid = bareJid;
}

void ChatWindow::appendMessage(const QXmppMessage &message)
{
    if (!message.body().isEmpty()){
        QString bareJid = jidToBareJid(message.from()); 
        ui.messageBrowse->append(QString("%1 %2").arg(bareJid).arg(QTime::currentTime().toString()));
        ui.messageBrowse->append(message.body());
    }
}

void ChatWindow::sendMessage()
{
    m_client->sendMessage(m_bareJid, ui.messageEdit->toHtml());
    QString c_bareJid = m_client->getConfiguration().jidBare();
    ui.messageBrowse->append(QString("%1 %2").arg(c_bareJid).arg(QTime::currentTime().toString()));
    ui.messageBrowse->append(ui.messageEdit->toHtml());
    ui.messageEdit->clear();
}
