#include "MainWindow.h"
#include "XmppClient.h"
#include "QXmppRoster.h"
#include "ChatWindow.h"
#include <QCloseEvent>
#include "QXmppMessage.h"
#include "QXmppUtils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_client = new XmppClient(this);
    m_rosterModel = new RosterModel(this);
    
    connect(&m_client->getRoster(), SIGNAL(rosterReceived()),
            this, SLOT(rosterReceived()));
    connect(m_client, SIGNAL(messageReceived(const QXmppMessage&)),
            this, SLOT(messageReceived(const QXmppMessage&)));

    connect(ui.rosterTreeView, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(openChatWindow(const QModelIndex &)));

    ui.rosterTreeView->setModel(m_rosterModel);

    m_client->connectToServer("talk.google.com", "chloereixmpp", "19871110rei", "gmail.com");
}

MainWindow::~MainWindow()
{
}

void MainWindow::rosterReceived()
{
    m_rosterModel->setRoster(&m_client->getRoster());
}

void MainWindow::messageReceived(const QXmppMessage& message)
{
    QString jid = message.from();
    QString bareJid = jidToBareJid(jid);
    if (m_chatWindows[jid] != NULL) {
        m_chatWindows[jid]->appendMessage(message);
    } else if (m_chatWindows[bareJid] != NULL) {
        m_chatWindows[bareJid]->appendMessage(message);
    } else {
        m_messageStore[jid] << message;
    }
}

void MainWindow::openChatWindow(const QModelIndex &index)
{
    RosterModel::ItemType type = m_rosterModel->itemTypeAt(index);
    if (type == RosterModel::contact ||
            type == RosterModel::resource) {
        QString jid = m_rosterModel->jidAt(index);

        ChatWindow *chatWindow;
        if (m_chatWindows[jid] == NULL) {
            chatWindow = new ChatWindow();
            chatWindow->setJid(jid);
            chatWindow->setClient(m_client);
            chatWindow->setAttribute(Qt::WA_DeleteOnClose);

            m_chatWindows[jid] = chatWindow;
            //chatWindow->setWindowTitle(m_rosterModel->data(index, Qt::DisplayRole).toString());
            chatWindow->setWindowTitle(jid);
        } else {
            chatWindow = m_chatWindows[jid];
        }
        chatWindow->show();
        chatWindow->raise();
        chatWindow->activateWindow();
    } else if (type == RosterModel::group) {
        if (ui.rosterTreeView->isExpanded(index)) {
            ui.rosterTreeView->collapse(index);
        } else {
            ui.rosterTreeView->expand(index);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDeleteAll(m_chatWindows);
    event->accept();
}
