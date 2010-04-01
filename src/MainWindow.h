#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include <QMap>
#include <QPointer>

class XmppClient;
class RosterModel;
class RosterTreeView;
class QModelIndex;
class ChatWindow;
class QXmppMessage;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void rosterReceived();
    void openChatWindow(const QModelIndex &index);
    void messageReceived(const QXmppMessage&);
    void activeContact(const QString &bareJid, const QString &resource = "");
    void unActiveContact(const QString &bareJid, const QString &resource = "");

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow ui;
    XmppClient *m_client;
    RosterModel *m_rosterModel;
    QMap<QString, QPointer<ChatWindow> > m_chatWindows;
    QMap<QString, QList<QXmppMessage> > m_messageStore;
};

#endif
