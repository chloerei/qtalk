#include "TransferManagerWindow.h"
#include "ui_TransferManagerWindow.h"
#include "TransferManagerModel.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QFile>

TransferManagerWindow::TransferManagerWindow(QXmppTransferManager *transferManager, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TransferManagerWindow),
    m_transferManager(transferManager)
{
    ui->setupUi(this);
    m_transferManagerModel = new TransferManagerModel(this);
    ui->tableView->setModel(m_transferManagerModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setAlternatingRowColors(true);
    connect(ui->actionStopTransfer, SIGNAL(triggered()),
            this, SLOT(stopTransferJob()) );
    connect(ui->actionCleanList, SIGNAL(triggered()),
            m_transferManagerModel, SLOT(clearJob()) );
    setAttribute(Qt::WA_QuitOnClose, false);
}

TransferManagerWindow::~TransferManagerWindow()
{
    delete ui;
}

void TransferManagerWindow::createTransferJob(const QString &jid, const QString &fileName)
{
    m_transferManagerModel->addJobToList(m_transferManager->sendFile(jid, fileName));
}

void TransferManagerWindow::receivedTransferJob(QXmppTransferJob *job)
{
    if (QMessageBox::information(this,
                                 tr("File recevied"),
                                 QString(tr("Transfer offer from: %1\nFilename: %2 \nDo your want to recevie?"))
                                 .arg(job->jid()).arg(job->fileName()),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        m_transferManagerModel->addJobToList(job);
        QString saveFileName =
                QFileDialog::getSaveFileName(this,
                                             tr("Save File"),
                                             QDesktopServices::storageLocation(QDesktopServices::DesktopLocation) + job->fileName());
        QFile *file = new QFile(saveFileName);
        if (file->open(QIODevice::WriteOnly)) {
            job->accept(file);
            m_fileList << file;
            show();
            raise();
            activateWindow();
        } else {
            QMessageBox::warning(this,
                                 tr("Writing Error"),
                                 tr("Can no writing file. Transfer abort"));
            job->abort();
        }
    } else {
        job->abort();
    }
}

void TransferManagerWindow::stopTransferJob()
{
    foreach(QModelIndex index, ui->tableView->selectionModel()->selectedRows()) {
        m_transferManagerModel->stopJobAtRow(index.row());
    }
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
