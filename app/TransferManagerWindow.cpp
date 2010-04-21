#include "TransferManagerWindow.h"
#include "ui_TransferManagerWindow.h"
#include "TransferManagerModel.h"

TransferManagerWindow::TransferManagerWindow(QXmppTransferManager *transferManager, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TransferManagerWindow),
    m_transferManager(transferManager)
{
    ui->setupUi(this);
    m_transferManagerModel = new TransferManagerModel(this);
    ui->tableView->setModel(m_transferManagerModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    setWindowTitle(QString(tr("Transfer Manager")));
}

TransferManagerWindow::~TransferManagerWindow()
{
    delete ui;
}

void TransferManagerWindow::createTransferJob(const QString &jid, const QString &fileName)
{
    m_transferManagerModel->addJobToList(m_transferManager->sendFile(jid, fileName));
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
