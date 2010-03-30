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
    QString bareJid = jidToBareJid(message.from());
    if (m_chatWindows[bareJid] == NULL) {
        m_messageStore[bareJid] << message;
    } else {
        m_chatWindows[bareJid]->appendMessage(message);
    }
}

void MainWindow::openChatWindow(const QModelIndex &index)
{
    if (m_rosterModel->itemTypeAt(index) == RosterModel::contact) {
        QString bareJid = jidToBareJid(m_rosterModel->jidAt(index));

        ChatWindow *chatWindow;
        if (m_chatWindows[bareJid] == NULL) {
            chatWindow = new ChatWindow();
            chatWindow->setBareJid(bareJid);
            chatWindow->setClient(m_client);
            chatWindow->setAttribute(Qt::WA_DeleteOnClose);

            m_chatWindows[bareJid] = chatWindow;
            chatWindow->setWindowTitle(m_rosterModel->data(index, Qt::DisplayRole).toString());
        } else {
            chatWindow = m_chatWindows[bareJid];
        }
        chatWindow->show();
        chatWindow->raise();
        chatWindow->activateWindow();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDeleteAll(m_chatWindows);
    event->accept();
}
