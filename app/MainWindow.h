#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include <QMap>
#include <QPointer>
#include <QSystemTrayIcon>
#include <QStringListModel>

class XmppClient;
class RosterModel;
class RosterTreeView;
class QModelIndex;
class ChatWindow;
class QXmppMessage;
class QListView;
class UnreadMessageWindow;
class UnreadMessageModel;

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
    void rosterReceived();
    void rosterDoubleClicked(const QModelIndex &index);
    void getUnreadListClicked(const QModelIndex &index);
    void openChatWindow(const QString &jid);
    void messageReceived(const QXmppMessage&);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void changeTrayIcon(TrayIconType type);
    void unreadMessageCleared();
    void readAllUnreadMessage();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow ui;
    XmppClient *m_client;
    RosterModel *m_rosterModel;
    QMap<QString, QPointer<ChatWindow> > m_chatWindows;
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayIconMenu;
    QAction *m_quitAction;

    // <bareJid, QList[mess1, mess2 ...]>
    //QMap<QString, QList<QXmppMessage> > m_messageStore;
    UnreadMessageModel *m_unreadMessageModel;
    QStringListModel stringModel;
    
    UnreadMessageWindow *m_unreadMessageWindow;
    void setupTrayIcon();
};

#endif
