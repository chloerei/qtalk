/*
 * Copyright (C) 2010 Rei
 *
 * Author:
 *	Rei
 *
 * Source:
 *	http://github.com/chloerei/qtalk
 *
 * This file is a part of QTalk.
 *
 * QTalk is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QTalk is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QTalk.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "MainWindow.h"
#include "QXmppRoster.h"
#include "ChatWindow.h"
#include <QCloseEvent>
#include "QXmppMessage.h"
#include "QXmppUtils.h"
#include <QListView>
#include <QTreeView>
#include "UnreadMessageWindow.h"
#include "UnreadMessageModel.h"
#include "LoginWidget.h"
#include <QSettings>
#include "PreferencesDialog.h"
#include "CloseNoticeDialog.h"
#include "RosterModel.h"
#include <QXmppVCardManager.h>
#include "TransferManagerWindow.h"
#include <QMessageBox>
#include <QDialog>
#include <QListWidget>
#include <QDialogButtonBox>
#include <ContactInfoDialog.h>
#include <QDesktopWidget>
#include <QXmppRosterIq.h>
#include "AddContactDialog.h"
#include "InfoEventStackWidget.h"
#include <QInputDialog>
#include <QTranslator>
#include <QXmppLogger.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_client(new QXmppClient(this)),
    m_rosterModel(new RosterModel(m_client, this)),
    m_rosterTreeView(new QTreeView(this)),
    m_unreadMessageModel(new UnreadMessageModel(this)),
    m_unreadMessageWindow(0),
    m_loginWidget(new LoginWidget(this)),
    m_preferencesDialog(0),
    m_closeToTrayDialog(0),
    m_transferManagerWindow(0),
    m_addContactDialog(0)
{
    ui.setupUi(this);
    readPreferences();

    retranslate();

    QXmppLogger::getLogger()->setLoggingType(QXmppLogger::STDOUT);

    m_client->getConfiguration().setAutoAcceptSubscriptions(false);
    //m_client->getTransferManager().setSupportedMethods(QXmppTransferJob::InBandMethod);
    m_client->getTransferManager().setProxy("proxy.eu.jabber.org");
    //m_client->getTransferManager().setProxyOnly(true);

    setupTrayIcon();
    ui.presenceComboBox->setVisible(false);
    ui.showInfoEventButton->setVisible(false);

    m_infoEventNone  = new QIcon(":/images/preferences-system-power-management.png");
    m_infoEventExist = new QIcon(":/images/ktip.png");

    m_infoEventStackWidget = new InfoEventStackWidget(m_client, this);
    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->addWidget(m_infoEventStackWidget);
    bottomLayout->setMargin(0);
    ui.bottomWrap->setLayout(bottomLayout);

    connect(ui.showInfoEventButton, SIGNAL(clicked()),
            this, SLOT(showEventStack()) );
    connect(m_infoEventStackWidget, SIGNAL(countChanged(int)),
            this, SLOT(infoEventCountChanged(int)) );
    connect(ui.presenceComboBox, SIGNAL(activated(int)),
            this, SLOT(presenceComboxChange(int)) );
    //m_infoEventStackWidget->addSubscribeRequest("test2");

    m_infoEventStackWidget->setVisible(false);
    connect(m_infoEventStackWidget, SIGNAL(infoEventCleared()),
            this, SLOT(updateTrayIcon()) );

    m_rosterTreeView->setHeaderHidden(true);
    m_rosterTreeView->setAnimated(true);
    rosterIconResize();
    m_rosterTreeView->setRootIsDecorated(false);
    m_rosterTreeView->setWordWrap(true);
    m_rosterTreeView->setAlternatingRowColors(true);
    m_rosterTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui.stackedWidget->addWidget(m_loginWidget);
    ui.stackedWidget->addWidget(m_rosterTreeView);


    changeToLogin();
    //setCentralWidget(m_loginWidget);

    connect(m_loginWidget, SIGNAL(login()),
            this, SLOT(login()));

    // unread message
    connect(m_unreadMessageModel, SIGNAL(messageCleared()),
            this, SLOT(unreadMessageCleared()));

    // xmpp client
    connect(m_client, SIGNAL(connected()),
            this, SLOT(clientConnected()));
    connect(m_client, SIGNAL(disconnected()),
            this, SLOT(clientDisconnected()) );
    connect(m_client, SIGNAL(error(QXmppClient::Error)),
            this, SLOT(clientError(QXmppClient::Error)));
    connect(m_client, SIGNAL(messageReceived(QXmppMessage)),
            this, SLOT(messageReceived(QXmppMessage)) );
    connect(m_client, SIGNAL(presenceReceived(QXmppPresence)),
            this, SLOT(presenceReceived(QXmppPresence)) );

    // roster model and view
    connect(m_rosterModel, SIGNAL(parseDone()),
            this, SLOT(changeToRoster()) );
    connect(m_rosterModel, SIGNAL(parseDone()),
            this, SLOT(rosterViewHiddenUpdate()) );
    connect(m_rosterModel, SIGNAL(hiddenUpdate()),
            this, SLOT(rosterViewHiddenUpdate()) );
    connect(m_rosterTreeView, SIGNAL(pressed(const QModelIndex &)),
            this, SLOT(rosterItemClicked(const QModelIndex &)));
    connect(m_rosterTreeView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(rosterContextMenu(const QPoint&)) );

    // action
    connect(ui.actionPreferences, SIGNAL(triggered()),
            this, SLOT(openPreferencesDialog()));
    connect(ui.actionLogout, SIGNAL(triggered()),
            this, SLOT(logout()));
    connect(ui.actionHideOffline, SIGNAL(triggered(bool)),
            this, SLOT(hideOffline(bool)) );
    connect(ui.actionTransferManager, SIGNAL(triggered()),
            this, SLOT(openTransferWindow()) );
    connect(ui.actionAddContact, SIGNAL(triggered()),
            this, SLOT(actionAddContact()) );
    connect(ui.actionQuit, SIGNAL(triggered()),
            this, SLOT(quit()) );
    connect(ui.actionRemoveContact, SIGNAL(triggered()),
            this, SLOT(actionRemoveContact()) );
    connect(ui.actionStartChat, SIGNAL(triggered()),
            this, SLOT(actionStartChat()) );
    connect(ui.actionContactInfo, SIGNAL(triggered()),
            this, SLOT(actionContactInfo()) );
    connect(ui.actionSubscribe, SIGNAL(triggered()),
            this, SLOT(actionSubscribe()) );
    connect(ui.actionUnsubscribe, SIGNAL(triggered()),
            this, SLOT(actionUnsubsribe()) );
    connect(ui.actionDropSubscribe, SIGNAL(triggered()),
            this, SLOT(actionDropSubsribe()) );
    connect(ui.actionAllowSubcribe, SIGNAL(triggered()),
            this, SLOT(actionAllowSubsribe()) );
    connect(ui.actionEditName, SIGNAL(triggered()),
            this, SLOT(actionEditName()) );
    connect(ui.actionMoveToNewGroup, SIGNAL(triggered()),
            this, SLOT(actionMoveToNewGroup()) );
    connect(ui.actionCopyToNewGroup, SIGNAL(triggered()),
            this, SLOT(actionCopyToNewGroup()) );

    // VCard
    connect(&m_client->getVCardManager(), SIGNAL(vCardReceived(const QXmppVCard&)),
            this, SLOT(vCardReveived(const QXmppVCard&)) );

    // transfer manager
    connect(&m_client->getTransferManager(), SIGNAL(fileReceived(QXmppTransferJob*)),
            this, SLOT(receivedTransferJob(QXmppTransferJob*)) );

    m_rosterTreeView->setModel(m_rosterModel);

    if (m_preferences.autoLogin)
        login();
}

MainWindow::~MainWindow()
{
}

void MainWindow::readPreferences()
{
    m_preferences.load();

    // action
    ui.actionHideOffline->setChecked(m_preferences.hideOffline);
    m_rosterModel->readPref(&m_preferences);

    m_loginWidget->readData(&m_preferences);

    if (m_preferences.mainWindowGeometry.isEmpty())
        move(QApplication::desktop()->screenGeometry().center() - geometry().center());
    else
        restoreGeometry(m_preferences.mainWindowGeometry);
    restoreState(m_preferences.mainWindowState);
}

void MainWindow::writePreferences()
{
    if (m_loginWidget->isVisible())
        m_loginWidget->writeData(&m_preferences);

    m_preferences.mainWindowGeometry = saveGeometry();
    m_preferences.mainWindowState = saveState();

    m_preferences.save();
}

void MainWindow::login()
{
    m_loginWidget->lock();
    m_loginWidget->writeData(&m_preferences);
    m_loginWidget->showState(tr("Login ..."));

    ui.presenceComboBox->setCurrentIndex(0);
    m_client->connectToServer(m_preferences.host, m_preferences.jid,
                              m_preferences.password, m_preferences.port);
}

void MainWindow::clientDisconnected()
{
    updateTrayIcon();
}

void MainWindow::clientConnected()
{
    m_loginWidget->showState(tr("Connect successful"));
    updateTrayIcon();
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

            updateTrayIcon();
        }
    }
}

void MainWindow::presenceReceived(const QXmppPresence &presence)
{
    switch (presence.getType()) {
    case QXmppPresence::Subscribe:
        m_infoEventStackWidget->addSubscribeRequest(presence.from());
        m_trayIcon->showMessage(QString(tr("Request")), QString(tr("%1 want to subscribe you")).arg(presence.from()));
        updateTrayIcon();
        break;
    default:
        break;
    }
}

void MainWindow::rosterItemClicked(const QModelIndex &index)
{
    if (QApplication::mouseButtons() == Qt::LeftButton) {
        RosterModel::ItemType type = m_rosterModel->itemTypeAt(index);
        if (type == RosterModel::contact ||
            type == RosterModel::resource) {
            QString jid = m_rosterModel->jidAt(index);
            openChatWindow(jid);
        } else if (type == RosterModel::group) {
            if (m_rosterTreeView->isExpanded(index)) {
                m_rosterTreeView->collapse(index);
            } else {
                m_rosterTreeView->expand(index);
            }
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
        // new chatWindow
        chatWindow = new ChatWindow(jid, m_client, this);

        connect(chatWindow, SIGNAL(sendFile(QString,QString)),
                this, SLOT(createTransferJob(QString,QString)) );
        connect(chatWindow, SIGNAL(viewContactInfo(QString)),
                this, SLOT(openContactInfoDialog(QString)) );

        if (m_rosterModel->hasVCard(jidToBareJid(jid)))
            chatWindow->setVCard(m_rosterModel->getVCard(jidToBareJid(jid)));

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

        // move to screan center
        chatWindow->move(QApplication::desktop()->screenGeometry().center() - chatWindow->geometry().center());
    } else {
        // exist
        chatWindow = m_chatWindows[jid];
    }
    chatWindow->readPref(&m_preferences);

    chatWindow->show();
    chatWindow->raise();
    chatWindow->activateWindow();
}

void MainWindow::actionStartChat()
{
    openChatWindow(m_rosterModel->jidAt(m_rosterTreeView->currentIndex()));
}

void MainWindow::actionContactInfo()
{
    openContactInfoDialog(m_rosterModel->jidAt(m_rosterTreeView->currentIndex()));
}

void MainWindow::actionAddContact()
{
    if (m_addContactDialog == 0) {
        m_addContactDialog = new AddContactDialog(this);
    }

    m_addContactDialog->setGroups(m_rosterModel->getGroups());

    if (m_addContactDialog->exec()) {
        QXmppRosterIq::Item item;
        item.setBareJid(m_addContactDialog->jid());
        if (!m_addContactDialog->name().isEmpty())
            item.setName(m_addContactDialog->name());
        if (!m_addContactDialog->group().isEmpty()) {
            QSet<QString> groups;
            groups << m_addContactDialog->group();
            item.setGroups(groups);
        }
        QXmppRosterIq iq;
        iq.setType(QXmppIq::Set);
        iq.addItem(item);
        m_client->sendPacket(iq);

        QXmppPresence presence(QXmppPresence::Subscribe);
        presence.setTo(m_addContactDialog->jid());
        m_client->sendPacket(presence);
    }
}

void MainWindow::actionRemoveContact()
{
    QString bareJid = jidToBareJid(m_rosterModel->jidAt(m_rosterTreeView->currentIndex()));
    if (QMessageBox::warning(this, QString(tr("Remove Contact")),
                         QString(tr("Are you sure to remove contact: %1 ?")).arg(bareJid),
                         QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::Yes) {
        QXmppRosterIq::Item item;
        item.setBareJid(bareJid);
        item.setSubscriptionType(QXmppRosterIq::Item::Remove);
        QXmppRosterIq iq;
        iq.setType(QXmppIq::Set);
        iq.addItem(item);
        m_client->sendPacket(iq);
    }
}

void MainWindow::actionSubscribe()
{
    QString bareJid = jidToBareJid(m_rosterModel->jidAt(m_rosterTreeView->currentIndex()));
    QXmppPresence presence(QXmppPresence::Subscribe);
    presence.setTo(bareJid);
    m_client->sendPacket(presence);
}

void MainWindow::actionUnsubsribe()
{
    QString bareJid = jidToBareJid(m_rosterModel->jidAt(m_rosterTreeView->currentIndex()));
    QXmppPresence presence(QXmppPresence::Unsubscribe);
    presence.setTo(bareJid);
    m_client->sendPacket(presence);
}

void MainWindow::actionDropSubsribe()
{
    QString bareJid = jidToBareJid(m_rosterModel->jidAt(m_rosterTreeView->currentIndex()));
    QXmppPresence presence(QXmppPresence::Unsubscribed);
    presence.setTo(bareJid);
    m_client->sendPacket(presence);
}

void MainWindow::actionAllowSubsribe()
{
    QString bareJid = jidToBareJid(m_rosterModel->jidAt(m_rosterTreeView->currentIndex()));
    QXmppPresence presence(QXmppPresence::Subscribed);
    presence.setTo(bareJid);
    m_client->sendPacket(presence);
}

void MainWindow::actionEditName()
{
    QString bareJid = jidToBareJid(m_rosterModel->jidAt(m_rosterTreeView->currentIndex()));
    QXmppRoster::QXmppRosterEntry entry = m_client->getRoster().getRosterEntry(bareJid);
    bool ok;
    QString name = QInputDialog::getText(this, QString(tr("Edit Name For: %1")).arg(bareJid),
                                         QString(tr("New Name")), QLineEdit::Normal,
                                         entry.name(), &ok);
    if (ok && !name.isEmpty()) {
        QXmppRosterIq iq;
        iq.setType(QXmppRosterIq::Set);
        entry.setName(name);
        iq.addItem(entry);
        m_client->sendPacket(iq);
    }
}

void MainWindow::actionMoveToNewGroup()
{
    QString bareJid = jidToBareJid(m_rosterModel->jidAt(m_rosterTreeView->currentIndex()));
    bool ok;
    QString group = QInputDialog::getText(this, QString(tr("New Group")),
                                         QString(tr("Group Name")), QLineEdit::Normal,
                                         QString(), &ok);
    if (ok && !group.isEmpty()) {
        QXmppRoster::QXmppRosterEntry entry = m_client->getRoster().getRosterEntry(bareJid);
        QSet<QString> groups = entry.groups();
        groups.remove(m_rosterModel->groupAt(m_rosterTreeView->currentIndex()));
        groups.insert(group);
        entry.setGroups(groups);
        QXmppRosterIq iq;
        iq.setType(QXmppIq::Set);
        iq.addItem(entry);
        m_client->sendPacket(iq);
    }
}

void MainWindow::actionMoveToGroup()
{
    QString bareJid = jidToBareJid(m_rosterModel->jidAt(m_rosterTreeView->currentIndex()));
    QAction *action = qobject_cast<QAction *>(sender());
    QString group = action->text();
    if (!bareJid.isEmpty() && !group.isEmpty()) {
        QXmppRoster::QXmppRosterEntry entry = m_client->getRoster().getRosterEntry(bareJid);
        QSet<QString> groups = entry.groups();
        groups.remove(m_rosterModel->groupAt(m_rosterTreeView->currentIndex()));
        groups.insert(group);
        entry.setGroups(groups);
        QXmppRosterIq iq;
        iq.setType(QXmppIq::Set);
        iq.addItem(entry);
        m_client->sendPacket(iq);
    }
}
void MainWindow::actionCopyToNewGroup()
{
    QString bareJid = jidToBareJid(m_rosterModel->jidAt(m_rosterTreeView->currentIndex()));
    bool ok;
    QString group = QInputDialog::getText(this, QString(tr("New Group")),
                                         QString(tr("Group Name")), QLineEdit::Normal,
                                         QString(), &ok);
    if (ok && !group.isEmpty()) {
        QXmppRoster::QXmppRosterEntry entry = m_client->getRoster().getRosterEntry(bareJid);
        QSet<QString> groups = entry.groups();
        groups.insert(group);
        entry.setGroups(groups);
        QXmppRosterIq iq;
        iq.setType(QXmppIq::Set);
        iq.addItem(entry);
        m_client->sendPacket(iq);
    }
}

void MainWindow::actionCopyToGroup()
{
    QString bareJid = jidToBareJid(m_rosterModel->jidAt(m_rosterTreeView->currentIndex()));
    QAction *action = qobject_cast<QAction *>(sender());
    QString group = action->text();
    if (!bareJid.isEmpty() && !group.isEmpty()) {
        QXmppRoster::QXmppRosterEntry entry = m_client->getRoster().getRosterEntry(bareJid);
        QSet<QString> groups = entry.groups();
        groups.insert(group);
        entry.setGroups(groups);
        QXmppRosterIq iq;
        iq.setType(QXmppIq::Set);
        iq.addItem(entry);
        m_client->sendPacket(iq);
    }
}

void MainWindow::showEventStack()
{
    if (m_infoEventStackWidget->isVisible())
        m_infoEventStackWidget->setAnimeVisible(false);
    else
        m_infoEventStackWidget->setAnimeVisible(true);
}

void MainWindow::openContactInfoDialog(QString jid)
{
    QString bareJid = jidToBareJid(jid);
    ContactInfoDialog *dialog;
    if (m_contactInfoDialogs[bareJid] == NULL) {
        dialog = new ContactInfoDialog();
        m_contactInfoDialogs[bareJid] = dialog;
        QXmppRoster::QXmppRosterEntry entry = m_client->getRoster().getRosterEntry(bareJid);
        dialog->setData(entry.name(), jid, m_rosterModel->getVCard(bareJid));
        dialog->move(QApplication::desktop()->screenGeometry().center() - dialog->geometry().center());
    } else {
        dialog = m_contactInfoDialogs[bareJid];
    }
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (m_unreadMessageWindow == 0) {
        createUnreadMessageWindow();
    }

    if (reason == QSystemTrayIcon::Trigger) {
        if (m_unreadMessageModel->hasAnyUnread()
            && m_unreadMessageWindow->isHidden()) {
            if (m_unreadMessageModel->rowCount(QModelIndex()) == 1)
                // single
                readAllUnreadMessage();
            else
                m_unreadMessageWindow->show();
        } else {
            if (isVisible()) {
                hide();
            } else {
                //hide();
                show();
                raise();
                activateWindow();
            }
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_preferences.closeToTrayNotice) {
        if (m_closeToTrayDialog == 0)
            m_closeToTrayDialog = new CloseNoticeDialog(this);
        m_closeToTrayDialog->readData(&m_preferences);

        if (m_closeToTrayDialog->exec()) {
            m_closeToTrayDialog->writeDate(&m_preferences);
        } else {
            event->ignore();
            return;
        }
    }

    if (m_preferences.closeToTray) {
        hide();
        event->ignore();
    } else {
        writePreferences();
        event->accept();
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::setupTrayIcon()
{
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/images/im-user-offline.png"));

    m_trayIconMenu = new QMenu(this);

    // Status change
    QAction *onlineAction = m_trayIconMenu->addAction(QIcon(":/images/im-user.png"), QString(tr("Online")));
    connect(onlineAction, SIGNAL(triggered()),
            this, SLOT(setPresenceOnline()) );
    QAction *chatAction = m_trayIconMenu->addAction(QIcon(":/images/im-user.png"), QString(tr("Chat")));
    connect(chatAction, SIGNAL(triggered()),
            this, SLOT(setPresenceChat()) );
    QAction *awayAction = m_trayIconMenu->addAction(QIcon(":/images/im-user-away.png"), QString(tr("Away")));
    connect(awayAction, SIGNAL(triggered()),
            this, SLOT(setPresenceAway()) );
    QAction *xaAction = m_trayIconMenu->addAction(QIcon(":/images/im-user-away.png"), QString(tr("Extened Away")));
    connect(xaAction, SIGNAL(triggered()),
            this, SLOT(setPresenceXa()) );
    QAction *busyAction = m_trayIconMenu->addAction(QIcon(":/images/im-user-busy.png"), QString(tr("Do Not Disturb")));
    connect(busyAction, SIGNAL(triggered()),
            this, SLOT(setPresenceDnd()) );
    QAction *offlineAction = m_trayIconMenu->addAction(QIcon(":/images/im-user-offline.png"), QString(tr("Offline")));
    connect(offlineAction, SIGNAL(triggered()),
            this, SLOT(setPresenceOffline()) );

    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(ui.actionTransferManager);
    m_trayIconMenu->addAction(ui.actionPreferences);

    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(ui.actionQuit);
    m_trayIcon->setContextMenu(m_trayIconMenu);

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    m_trayIcon->show();
}

void MainWindow::createUnreadMessageWindow()
{
    m_unreadMessageWindow = new UnreadMessageWindow(this);
    m_unreadMessageWindow->move(m_trayIcon->geometry().center() - m_unreadMessageWindow->geometry().center());
    m_unreadMessageWindow->setModel(m_unreadMessageModel);

    connect(m_unreadMessageWindow, SIGNAL(unreadListClicked(const QModelIndex&)),
            this, SLOT(getUnreadListClicked(const QModelIndex&)));
    connect(m_unreadMessageWindow, SIGNAL(readAll()),
            this, SLOT(readAllUnreadMessage()));
}

void MainWindow::retranslate()
{
    QString tsPath = QCoreApplication::applicationDirPath() + "/translations";
    qApp->removeTranslator(&m_translator);

    if (m_preferences.language != "en") {
        m_translator.load(tsPath + "/qtalk_" + m_preferences.language);
        qApp->installTranslator(&m_translator);
    }
}

void MainWindow::unreadMessageCleared()
{
    updateTrayIcon();
}

void MainWindow::readAllUnreadMessage()
{
    foreach (QString bareJid, m_unreadMessageModel->bareJids()) {
        openChatWindow(bareJid);
    }
    m_unreadMessageWindow->hide();
}

void MainWindow::clientDisconnect()
{
    m_rosterModel->clear();
    foreach (ChatWindow *window, m_chatWindows) {
        if (window != NULL)
            window->close();
    }
    m_chatWindows.clear();
    m_client->disconnect();
}

void MainWindow::clientError(QXmppClient::Error)
{
    m_loginWidget->showState(tr("Connect Error"));
    changeToLogin();
}

void MainWindow::openPreferencesDialog()
{
    if (m_preferencesDialog == 0) {
        m_preferencesDialog = new PreferencesDialog(this);

        // preferences dialog
        connect(m_preferencesDialog, SIGNAL(applied()),
                this, SLOT(preferencesApplied()));
        connect(m_preferencesDialog, SIGNAL(rosterIconSizeChanged(int)),
                this, SLOT(setRosterIconSize(int)) );
        connect(m_preferencesDialog, SIGNAL(rosterIconReseze()),
                this, SLOT(rosterIconResize()) );

    }
    m_preferencesDialog->readData(&m_preferences);
    m_preferencesDialog->show();
}

void MainWindow::preferencesApplied()
{
    QString language = m_preferences.language;
    if (language.isEmpty())
        language = "en";

    m_preferencesDialog->writeData(&m_preferences);

    if (m_preferencesDialog->isLanguageChanged()) {
        retranslate();

    }

    if (m_preferencesDialog->isRosterPrefChanged()) {
        ui.actionHideOffline->setChecked(m_preferences.hideOffline);
        m_rosterModel->readPref(&m_preferences);
        rosterViewHiddenUpdate();
    }

    m_rosterTreeView->setIconSize(QSize(m_preferences.rosterIconSize, m_preferences.rosterIconSize));

    if (m_preferencesDialog->isAccountChanged())
        m_loginWidget->readData(&m_preferences);

    m_preferences.save();
}

void MainWindow::hideOffline(bool hide)
{
    m_preferences.hideOffline = hide;
    m_rosterModel->readPref(&m_preferences);
    rosterViewHiddenUpdate();
}

void MainWindow::changeToLogin()
{
    m_loginWidget->unlock();
    ui.stackedWidget->setCurrentIndex(0);
    ui.presenceComboBox->setVisible(false);
    ui.showInfoEventButton->setVisible(false);
}

void MainWindow::changeToRoster()
{
    ui.stackedWidget->setCurrentIndex(1);
    m_rosterTreeView->expandToDepth(0);
    ui.presenceComboBox->setVisible(true);
    ui.showInfoEventButton->setVisible(true);
}

void MainWindow::setRosterIconSize(int num)
{
    m_rosterTreeView->setIconSize(QSize(num, num));
}

void MainWindow::rosterIconResize()
{
    setRosterIconSize(m_preferences.rosterIconSize);
}

void MainWindow::rosterViewHiddenUpdate()
{
    foreach (QModelIndex contactIndex, m_rosterModel->allIndex()) {
        m_rosterTreeView->setRowHidden(contactIndex.row(),
                                       contactIndex.parent(),
                                       m_rosterModel->isIndexHidden(contactIndex));;
    }
}

void MainWindow::vCardReveived(const QXmppVCard &vCard)
{
    if (m_chatWindows[vCard.from()] != NULL) {
        ChatWindow *window = m_chatWindows[vCard.from()];
        window->setVCard(vCard);
    }
}

void MainWindow::logout()
{
    clientDisconnect();
    changeToLogin();
}

void MainWindow::quit()
{
    writePreferences();
    qApp->quit();
}

void MainWindow::openTransferWindow()
{
    initTransferWindow();
    m_transferManagerWindow->show();
}

void MainWindow::createTransferJob(const QString &jid, const QString &fileName)
{
    initTransferWindow();

    QString newJid = jid;
    if (jidToResource(jid).isEmpty()) {
        QStringList resources = m_client->getRoster().getAllPresencesForBareJid(jid).keys();
        if (resources.isEmpty()) {
            QMessageBox::warning(0, "Contact Offline", "Can not send file to offline contact.");
            return;
        } else if (resources.count() == 1) {
            // auto select the single resource
            newJid = jid + "/" + resources.at(0);
        } else {
            // let usr select witch resource to send
            QDialog dialog;
            QListWidget *listWidget = new QListWidget(&dialog);
            foreach(QString resource, resources) {
                QListWidgetItem *item = new QListWidgetItem(resource);
                listWidget->addItem(item);
            }
            listWidget->setCurrentRow(0);
            QVBoxLayout *layout = new QVBoxLayout;
            QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
            buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
            connect(buttonBox, SIGNAL(accepted()),
                    &dialog, SLOT(accept()) );
            connect(buttonBox, SIGNAL(rejected()),
                    &dialog, SLOT(reject()) );
            QLabel *label = new QLabel();
            label->setText(tr("<b>Note:</b> Contact logined at multi resources, you must select witch resource to send"));
            label->setWordWrap(true);
            layout->addWidget(label);
            layout->addWidget(listWidget);
            layout->addWidget(buttonBox);
            dialog.setLayout(layout);
            dialog.setWindowTitle(tr("Mutil Resources"));
            if (dialog.exec()) {
                // accept
                newJid = jid + "/" + resources.at(listWidget->currentRow());
            } else {
                // reject
                return;
            }
        }
    }

    m_transferManagerWindow->createTransferJob(newJid, fileName);
    m_transferManagerWindow->show();
}

void MainWindow::receivedTransferJob(QXmppTransferJob *offer)
{
    initTransferWindow();
    m_transferManagerWindow->receivedTransferJob(offer);
}

void MainWindow::rosterContextMenu(const QPoint &position)
{
    QModelIndex index = m_rosterTreeView->indexAt(position);
    if (index.isValid()) {
        QMenu menu;
        RosterModel::ItemType type = m_rosterModel->itemTypeAt(index);
        if (type != RosterModel::group) {
            menu.addAction(ui.actionStartChat);
            menu.addAction(ui.actionContactInfo);

            if (type == RosterModel::contact) {
                menu.addSeparator();
                menu.addAction(ui.actionEditName);
                QString currentGroup = m_rosterModel->groupAt(m_rosterTreeView->currentIndex());
                QSet<QString> otherGroups = m_rosterModel->getGroups();
                otherGroups.remove(currentGroup);

                QMenu *moveMenu = menu.addMenu(QString(tr("Move to")));
                moveMenu->addAction(ui.actionMoveToNewGroup);
                moveMenu->addSeparator();
                foreach (QString group, otherGroups) {
                    QAction *moveAction = moveMenu->addAction(group);
                    connect(moveAction, SIGNAL(triggered()),
                            this, SLOT(actionMoveToGroup()) );
                }

                QMenu *copyMenu = menu.addMenu(QString(tr("Copy to")));
                copyMenu->addAction(ui.actionCopyToNewGroup);
                copyMenu->addSeparator();
                foreach (QString group, otherGroups) {
                    QAction *copyAction = copyMenu->addAction(group);
                    connect(copyAction, SIGNAL(triggered()),
                            this, SLOT(actionCopyToGroup()) );
                }

                menu.addSeparator();
                QMenu *subMenu = menu.addMenu(tr("Roster"));
                // * has bug: auto send subcribe presence when type is 'from', hide it.
                QString bareJid = m_rosterModel->jidAt(index);
                QXmppRoster::QXmppRosterEntry entry = m_client->getRoster().getRosterEntry(bareJid);
                switch (entry.subscriptionType()) {
                case QXmppRoster::QXmppRosterEntry::Both:
                    subMenu->addAction(ui.actionUnsubscribe);
                    subMenu->addAction(ui.actionDropSubscribe);
                    break;
                case QXmppRoster::QXmppRosterEntry::To:
                    subMenu->addAction(ui.actionUnsubscribe);
                    subMenu->addAction(ui.actionAllowSubcribe);
                    break;
                case QXmppRoster::QXmppRosterEntry::From:
                    subMenu->addAction(ui.actionSubscribe);
                    subMenu->addAction(ui.actionDropSubscribe);
                    break;
                case QXmppRoster::QXmppRosterEntry::None:
                    subMenu->addAction(ui.actionSubscribe);
                    break;
                default:
                    break;
                }
                subMenu->addSeparator();
                subMenu->addAction(ui.actionRemoveContact);
            }
            menu.exec(m_rosterTreeView->mapToGlobal(position));
        }
    }
}

void MainWindow::initTransferWindow()
{
    if (m_transferManagerWindow == 0) {
        m_transferManagerWindow = new TransferManagerWindow(&m_client->getTransferManager(), this);
        m_transferManagerWindow->move(QApplication::desktop()->screenGeometry().center() - m_transferManagerWindow->geometry().center());
        connect(&m_client->getTransferManager(), SIGNAL(finished(QXmppTransferJob*)),
                m_transferManagerWindow, SLOT(deleteFileHandel(QXmppTransferJob*)) );

    }
}

void MainWindow::infoEventCountChanged(int count)
{
    if (count == 0) {
        ui.showInfoEventButton->setText("");
        ui.showInfoEventButton->setIcon(*m_infoEventNone);
    } else {
        ui.showInfoEventButton->setText(QString("%1").arg(count));
        ui.showInfoEventButton->setIcon(*m_infoEventExist);
    }
}

void MainWindow::presenceComboxChange(int index)
{
    switch (index) {
    case 0:
        setPresenceOnline();
        break;
    case 1:
        setPresenceChat();
        break;
    case 2:
        setPresenceAway();
        break;
    case 3:
        setPresenceXa();
        break;
    case 4:
        setPresenceDnd();
        break;
        /* invisible is no in xmpp core
    case 5:
        m_client->setClientPresence(QXmppPresence::Status::Invisible);
        break;
        */
    case 5:
        setPresenceOffline();
        break;
    }
}

void MainWindow::setPresenceOnline()
{
    if (m_client->getClientPresence().getStatus().getType() == QXmppPresence::Status::Online
        && m_client->getClientPresence().getType() == QXmppPresence::Available)
        return;

    if (ui.presenceComboBox->currentIndex() != 0)
        ui.presenceComboBox->setCurrentIndex(0);
    QXmppPresence presence = m_client->getClientPresence();
    presence.getStatus().setType(QXmppPresence::Status::Online);
    presence.getStatus().setStatusText(QString());
    m_client->setClientPresence(presence);
    reConnect();
    updateTrayIcon();
}

void MainWindow::setPresenceChat()
{
    if (m_client->getClientPresence().getStatus().getType() == QXmppPresence::Status::Chat)
        return;

    if (ui.presenceComboBox->currentIndex() != 1)
        ui.presenceComboBox->setCurrentIndex(1);
    QXmppPresence presence = m_client->getClientPresence();
    presence.getStatus().setType(QXmppPresence::Status::Chat);
    presence.getStatus().setStatusText(QString());
    m_client->setClientPresence(presence);
    reConnect();
    updateTrayIcon();
}

void MainWindow::setPresenceAway()
{
    if (m_client->getClientPresence().getStatus().getType() == QXmppPresence::Status::Away)
        return;

    if (ui.presenceComboBox->currentIndex() != 2)
        ui.presenceComboBox->setCurrentIndex(2);
    QXmppPresence presence = m_client->getClientPresence();
    presence.getStatus().setType(QXmppPresence::Status::Away);
    presence.getStatus().setStatusText(QString());
    m_client->setClientPresence(presence);
    reConnect();
    updateTrayIcon();
}

void MainWindow::setPresenceXa()
{
    if (m_client->getClientPresence().getStatus().getType() == QXmppPresence::Status::XA)
        return;

    if (ui.presenceComboBox->currentIndex() != 3)
        ui.presenceComboBox->setCurrentIndex(3);
    QXmppPresence presence = m_client->getClientPresence();
    presence.getStatus().setType(QXmppPresence::Status::XA);
    presence.getStatus().setStatusText(QString());
    m_client->setClientPresence(presence);
    reConnect();
    updateTrayIcon();
}

void MainWindow::setPresenceDnd()
{
    if (m_client->getClientPresence().getStatus().getType() == QXmppPresence::Status::DND)
        return;

    if (ui.presenceComboBox->currentIndex() != 4)
        ui.presenceComboBox->setCurrentIndex(4);
    QXmppPresence presence = m_client->getClientPresence();
    presence.getStatus().setType(QXmppPresence::Status::DND);
    presence.getStatus().setStatusText(QString());
    m_client->setClientPresence(presence);
    reConnect();
    updateTrayIcon();
}

void MainWindow::reConnect()
{
    if (m_client->getClientPresence().getType() != QXmppPresence::Available) {
        m_client->setClientPresence(QXmppPresence::Available);
        m_client->connectToServer(m_preferences.host, m_preferences.jid,
                                  m_preferences.password, m_preferences.port,
                                  m_client->getClientPresence());
    }
}

void MainWindow::setPresenceOffline()
{
    if (m_client->getClientPresence().getStatus().getType() == QXmppPresence::Status::Offline)
        return;

    if (ui.presenceComboBox->currentIndex() != 5)
        ui.presenceComboBox->setCurrentIndex(5);
    QXmppPresence presence = m_client->getClientPresence();
    presence.getStatus().setType(QXmppPresence::Status::Offline);
    presence.getStatus().setStatusText(QString());
    m_client->setClientPresence(presence);
    clientDisconnect();
    updateTrayIcon();
}


void MainWindow::updateTrayIcon()
{
    if (m_unreadMessageModel->hasAnyUnread()) {
        m_trayIcon->setIcon(QIcon(":/images/mail-unread-new.png"));
        return;
    }

    if (!m_infoEventStackWidget->isEmpty()) {
        m_trayIcon->setIcon(QIcon(":/images/ktip.png"));
        return;
    }

    if (m_client->getClientPresence().getType() == QXmppPresence::Available) {
        switch (m_client->getClientPresence().getStatus().getType()) {
        case QXmppPresence::Status::Away:
        case QXmppPresence::Status::XA:
            m_trayIcon->setIcon(QIcon(":/images/im-user-away.png"));
            return;
        case QXmppPresence::Status::DND:
            m_trayIcon->setIcon(QIcon(":/images/im-user-busy.png"));
            return;
            /*
        case QXmppPresence::Status::Invisible:
            m_trayIcon->setIcon(QIcon(":/images/im-invisible-user.png"));
            return;
            */
        default:
            m_trayIcon->setIcon(QIcon(":/images/im-user.png"));
            return;
        }
    }

    if (m_client->getClientPresence().getType() == QXmppPresence::Unavailable)
        m_trayIcon->setIcon(QIcon(":/images/im-user-offline.png"));
}
