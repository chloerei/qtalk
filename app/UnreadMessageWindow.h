#ifndef UNREADMESSAGE
#define UNREADMESSAGE

#include <QMainWindow>
#include "ui_UnreadMessageWindow.h"
#include <QList>

class QXmppMessage;

class UnreadMessageWindow : public QMainWindow
{
    Q_OBJECT
public:
    UnreadMessageWindow(QWidget *parent = 0);
    void setModel(QAbstractListModel *model);

signals:
    void unreadListClicked(const QModelIndex &index);
    void readAll();

private:
    Ui::UnreadMessageWindow ui;
};

#endif
