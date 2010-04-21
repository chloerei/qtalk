#include "TransferManagerWindow.h"
#include "ui_TransferManagerWindow.h"
#include "TransferManagerModel.h"

TransferManagerWindow::TransferManagerWindow(QXmppTransferManager *transferManager, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TransferManagerWindow),
    m_transferManager(transferManager)
{
    ui->setupUi(this);
    ui->tableView->setModel(new TransferManagerModel(m_transferManager, this));
}

TransferManagerWindow::~TransferManagerWindow()
{
    delete ui;
}

void TransferManagerWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
