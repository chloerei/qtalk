#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include <QMap>
class XmppClient;
class RosterModel;
class RosterTreeView;
class QModelIndex;
class ChatDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

private slots:
    void rosterReceived();
    void openChatWindow(const QModelIndex &index);

private:
    Ui::MainWindow ui;
    XmppClient *m_client;
    RosterModel *m_rosterModel;
    QMap<QString, ChatDialog*> m_chatDialogs;
};

#endif
