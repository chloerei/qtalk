#include "MainWindow.h"
#include "XmppClient.h"
#include "QXmppRoster.h"
#include "ChatDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_client = new XmppClient(this);
    m_rosterModel = new RosterModel(this);
    
    connect(&m_client->getRoster(), SIGNAL(rosterReceived()),
            this, SLOT(rosterReceived()));

    connect(ui.rosterTreeView, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(openChatWindow(const QModelIndex &)));

    ui.rosterTreeView->setModel(m_rosterModel);

    m_client->connectToServer("talk.google.com", "chloereixmpp", "19871110rei", "gmail.com");
}

void MainWindow::rosterReceived()
{
    m_rosterModel->setRoster(&m_client->getRoster());
}

void MainWindow::openChatWindow(const QModelIndex &index)
{
    ChatDialog *chatDialog = new ChatDialog();
    chatDialog->setWindowTitle(m_rosterModel->data(index, Qt::DisplayRole).toString());
    chatDialog->show();
}
