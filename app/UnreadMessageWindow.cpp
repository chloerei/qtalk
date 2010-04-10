#include "UnreadMessageWindow.h"
#include "QXmppMessage.h"
#include <UnreadMessageModel.h>

UnreadMessageWindow::UnreadMessageWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.listView, SIGNAL(clicked(const QModelIndex&)),
            this, SIGNAL(unreadListClicked(const QModelIndex&)));
    connect(ui.readAllButton, SIGNAL(clicked()),
            this, SIGNAL(readAll()));

}

void UnreadMessageWindow::setModel(QAbstractListModel *model)
{
    ui.listView->setModel(model);
}
