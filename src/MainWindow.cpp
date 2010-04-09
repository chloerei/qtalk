#include "MainWindow.h"
#include "XmppClient.h"
#include "QXmppRoster.h"
#include "ChatWindow.h"
#include <QCloseEvent>
#include "QXmppMessage.h"
#include "QXmppUtils.h"
#include <QListView>
#include "UnreadMessageWindow.h"
#include "UnreadMessageModel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setupTrayIcon();
    m_unreadMessageWindow = new UnreadMessageWindow(this);
    m_unreadMessageModel = new UnreadMessageModel(this);
    m_unreadMessageWindow->setModel(m_unreadMessageModel);
    connect(m_unreadMessageWindow, SIGNAL(unreadListClicked(const QModelIndex&)),
            this, SLOT(getUnreadListClicked(const QModelIndex&)));

    m_client = new XmppClient(this);
    m_rosterModel = new RosterModel(this);
    
    connect(&m_client->getRoster(), SIGNAL(rosterReceived()),
            this, SLOT(rosterReceived()));
    connect(m_client, SIGNAL(messageReceived(const QXmppMessage&)),
            this, SLOT(messageReceived(const QXmppMessage&)));

    connect(ui.rosterTreeView, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(rosterDoubleClicked(const QModelIndex &)));

    ui.rosterTreeView->setModel(m_rosterModel);

    m_client->connectToServer("talk.google.com", "chloerei", "1110chloerei", "gmail.com");

    // test
    //m_unreadMessageModel->add(QXmppMessage());
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
    QString resource = jidToResource(jid);
    if (m_chatWindows[jid] != NULL) {
        m_chatWindows[jid]->appendMessage(message);
    } else if (m_chatWindows[bareJid] != NULL) {
        m_chatWindows[bareJid]->appendMessage(message);
    } else {
        // ignore state message
        if (!message.body().isEmpty()) {
            m_unreadMessageModel->add(message);
            m_rosterModel->messageUnread(bareJid, resource);
            m_trayIcon->showMessage("get message", "some");
        }
    }
}

void MainWindow::rosterDoubleClicked(const QModelIndex &index)
{
    RosterModel::ItemType type = m_rosterModel->itemTypeAt(index);
    if (type == RosterModel::contact ||
            type == RosterModel::resource) {
        QString jid = m_rosterModel->jidAt(index);
        openChatWindow(jid);
    } else if (type == RosterModel::group) {
        if (ui.rosterTreeView->isExpanded(index)) {
            ui.rosterTreeView->collapse(index);
        } else {
            ui.rosterTreeView->expand(index);
        }
    }
}

void MainWindow::getUnreadListClicked(const QModelIndex &index)
{
    openChatWindow(m_unreadMessageModel->jidAt(index));
    if (m_unreadMessageModel->rowCount(QModelIndex()) == 0) {
        m_unreadMessageWindow->hide();
    }
}

void MainWindow::openChatWindow(const QString &jid)
{
    ChatWindow *chatWindow;
    if (m_chatWindows[jid] == NULL) {
        chatWindow = new ChatWindow();
        chatWindow->setJid(jid);
        chatWindow->setClient(m_client);
        chatWindow->setAttribute(Qt::WA_DeleteOnClose);

        m_chatWindows[jid] = chatWindow;
        chatWindow->setWindowTitle(jid);

        // load unread message
        if (m_unreadMessageModel->hasUnread(jid)) {
            foreach (QXmppMessage message, m_unreadMessageModel->take(jid)) {
                chatWindow->appendMessage(message);
            }
        }

        // clean unread state
        QString resource = jidToResource(jid);
        if (resource.isEmpty()) {
            m_rosterModel->messageReadedAll(jidToBareJid(jid));
        } else {
            // resource
            m_rosterModel->messageReaded(jidToBareJid(jid), jidToResource(jid));
        }
    } else {
        chatWindow = m_chatWindows[jid];
    }
    chatWindow->show();
    chatWindow->raise();
    chatWindow->activateWindow();
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        m_unreadMessageWindow->show();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDeleteAll(m_chatWindows);
    event->accept();
}

void MainWindow::setupTrayIcon()
{
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/image/tux.png"));

    m_quitAction = new QAction(tr("&Quit"), this);
    connect(m_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(m_quitAction);
    m_trayIcon->setContextMenu(m_trayIconMenu);

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    m_trayIcon->show();
}
