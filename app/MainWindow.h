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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include <QMap>
#include <QPointer>
#include <QSystemTrayIcon>
#include <QStringListModel>
#include <QXmppClient.h>
#include <Preferences.h>
#include <QXmppVCard.h>
#include <QTranslator>

class AddContactDialog;
class ChatWindow;
class CloseNoticeDialog;
class ContactInfoDialog;
class InfoEventStackWidget;
class LoginWidget;
class PreferencesDialog;
class QListView;
class QModelIndex;
class QTreeView;
class QXmppMessage;
class QXmppTransferJob;
class RosterModel;
class RosterTreeView;
class TransferManagerWindow;
class UnreadMessageModel;
class UnreadMessageWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void readPreferences();
    void writePreferences();
    void login();
    void clientDisconnected();
    void clientConnected();
    void rosterItemClicked(const QModelIndex &index);
    void getUnreadListClicked(const QModelIndex &index);
    void openChatWindow(const QString &jid);
    void actionStartChat();
    void actionContactInfo();
    void actionAddContact();
    void actionRemoveContact();
    void actionSubscribe();
    void actionUnsubsribe();
    void actionDropSubsribe();
    void actionAllowSubsribe();
    void actionEditName();
    void actionMoveToNewGroup();
    void actionMoveToGroup();
    void actionCopyToNewGroup();
    void actionCopyToGroup();
    void showEventStack();
    void openContactInfoDialog(QString jid);
    void messageReceived(const QXmppMessage&);
    void presenceReceived(const QXmppPresence&);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void unreadMessageCleared();
    void readAllUnreadMessage();
    void clientDisconnect();
    void clientError(QXmppClient::Error);
    void openPreferencesDialog();
    void preferencesApplied();
    void hideOffline(bool hide);
    void changeToLogin();
    void changeToRoster();
    void setRosterIconSize(int);
    void rosterIconResize();
    void rosterViewHiddenUpdate();
    void vCardReveived(const QXmppVCard &vCard);
    void logout();
    void quit();
    void openTransferWindow();
    void createTransferJob(const QString &jid, const QString &fileName);
    void receivedTransferJob(QXmppTransferJob *offer);
    void rosterContextMenu(const QPoint &position);
    void initTransferWindow();
    void infoEventCountChanged(int count);
    void presenceComboxChange(int index);
    void setPresenceOnline();
    void setPresenceChat();
    void setPresenceAway();
    void setPresenceXa();
    void setPresenceDnd();
    void reConnect();
    void setPresenceOffline();
    void updateTrayIcon();

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);

private:
    Ui::MainWindow ui;
    Preferences m_preferences;
    QXmppClient *m_client;
    InfoEventStackWidget *m_infoEventStackWidget;
    QIcon *m_infoEventNone;
    QIcon *m_infoEventExist;
    RosterModel *m_rosterModel;
    QTreeView *m_rosterTreeView;
    QMap<QString, QPointer<ChatWindow> > m_chatWindows;
    QMap<QString, QPointer<ContactInfoDialog> > m_contactInfoDialogs;
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayIconMenu;
    QAction *m_quitAction;
    UnreadMessageModel *m_unreadMessageModel;
    UnreadMessageWindow *m_unreadMessageWindow;
    LoginWidget *m_loginWidget;
    PreferencesDialog *m_preferencesDialog;
    CloseNoticeDialog *m_closeToTrayDialog;
    TransferManagerWindow *m_transferManagerWindow;
    AddContactDialog *m_addContactDialog;
    QTranslator m_translator;

    
    void setupTrayIcon();
    void createUnreadMessageWindow();
    void retranslate();
};

#endif
