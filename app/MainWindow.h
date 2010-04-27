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

class XmppClient;
class RosterModel;
class RosterTreeView;
class QModelIndex;
class ChatWindow;
class QXmppMessage;
class QListView;
class UnreadMessageWindow;
class UnreadMessageModel;
class LoginWidget;
class QTreeView;
class PreferencesDialog;
class CloseNoticeDialog;
class TransferManagerWindow;
class QXmppTransferJob;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    enum TrayIconType
    {
        online,
        newMessage
    };

private slots:
    void readPreferences();
    void writePreferences();
    void login();
    void clientConnected();
    void rosterItemClicked(const QModelIndex &index);
    void getUnreadListClicked(const QModelIndex &index);
    void openChatWindow(const QString &jid);
    void actionStartChat();
    void actionContactInfo();
    void messageReceived(const QXmppMessage&);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void changeTrayIcon(TrayIconType type);
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
    void quit();
    void openTransferWindow();
    void createTransferJob(const QString &jid, const QString &fileName);
    void receivedTransferJob(QXmppTransferJob *offer);
    void rosterContextMenu(const QPoint &position);
    void initTransferWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow ui;
    Preferences m_preferences;
    XmppClient *m_client;
    RosterModel *m_rosterModel;
    QTreeView *m_rosterTreeView;
    QAction *m_actionStartChat;
    QAction *m_actionContactInfo;
    QMap<QString, QPointer<ChatWindow> > m_chatWindows;
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayIconMenu;
    QAction *m_quitAction;
    UnreadMessageModel *m_unreadMessageModel;
    UnreadMessageWindow *m_unreadMessageWindow;
    LoginWidget *m_loginWidget;
    PreferencesDialog *m_preferencesDialog;
    CloseNoticeDialog *m_closeToTrayDialog;
    TransferManagerWindow *m_transferManagerWindow;
    
    void setupTrayIcon();
    void createUnreadMessageWindow();
};

#endif
